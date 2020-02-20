#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>

#define NCLONES		4
#define LIMIT		2000000000
#define	STACK_SIZE	(1024*64)

int leibniz(void *args)
{
	int counter;
	float sum = 0;

	int idclone = *((int *) args);
	int inicio =(LIMIT/NCLONES)*idclone;
	int fin = (LIMIT/NCLONES)*(idclone+1);

	printf("Clon %d inicia en %d y termina en %d\n", idclone,inicio,fin);
	for(counter = inicio; counter < fin; counter++)
	{
		if(counter % 2 == 0)
			sum += (1.0/((2*counter) + 1));
		else
			sum -= (1.0/((2*counter) + 1));
	}
	printf("SUMA TOTAL = %f\n", sum);
	return 0;
}

int main(void)
{
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	int status;
	pid_t pid;
	int counter;
	int param[NCLONES];
	void *stack[NCLONES];

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Tiempo inicial

	for (counter = 0; counter < NCLONES; counter++)
	{
		stack[counter] = malloc(STACK_SIZE);
		if(stack[counter] == NULL)
			printf("ERROR STACK\n");

		param[counter] = counter;
		pid = clone(&leibniz,(char *)stack[counter] + STACK_SIZE,
			SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM,(void *)&param[counter]);
		if(pid == -1)
			printf("ERROR CLONE\n");

		pid = wait(&status);
		if(pid == -1)
			printf("ERROR WAIT\n");
	}

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);

	for(counter = 0; counter < NCLONES; counter++)
		free(stack[counter]);

	return 0;
}