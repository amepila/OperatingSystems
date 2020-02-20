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

double sum_total;

int leibniz(void *args)
{
	int counter;
	double sum = 0;

	int idclone = *((int *) args);
	int begin =(LIMIT/NCLONES)*idclone;
	int end = (LIMIT/NCLONES)*(idclone+1);

	for(counter = begin; counter < end; counter++)
	{
		if(counter % 2 == 0)
			sum += (1.0/((2*counter) + 1));
		else
			sum -= (1.0/((2*counter) + 1));
	}
	sum_total = sum_total + sum;
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
	pid_t pid[NCLONES];
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
		pid[counter] = clone(&leibniz,(char *)stack[counter] + STACK_SIZE,
			SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM,(void *)&param[counter]);
		if(pid[counter] == -1)
			printf("ERROR CLONE\n");
	}

	for(counter = 0; counter < NCLONES; counter++)
	{
		pid[counter] = wait(&status);
		if(pid[counter] == -1)
			printf("ERROR WAIT\n");
	}

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);
	printf("SUMA TOTAL = %lf\n", sum_total);

	for(counter = 0; counter < NCLONES; counter++)
		free(stack[counter]);

	return 0;
}