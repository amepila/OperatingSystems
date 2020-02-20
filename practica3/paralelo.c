#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NCLONES		4
#define LIMIT		2000000000
#define	STACK_SIZE	(1024*64)

void *leibniz(void *args)
{
	int counter;
	float sum = 0;

	int idclone = *((int *) args);
	int inicio =(LIMIT/NCLONES)*idclone;
	int fin = (LIMIT/NCLONES)*(idclone+1);

	printf("Clon %d inicia en %d y termina en %d\n", idclon,inicio,fin);
	for(counter = inicio; counter < fin; counter++)
	{
		if(counter % 2 == 0)
			sum += (1.0/((2*counter) + 1));
		else
			sum -= (1.0/((2*counter) + 1));
	}
	printf("SUMA TOTAL = %f\n", sum);
}

int main(void)
{
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

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
		pid = clone(leibniz,stack[counter],CLONE_NEWUTS|SIGCHILD,(void *)&param[counter]);
		if(pid == -1)
			printf("ERROR CLONE\n");
		printf("clone() returned %ld\n", (long)pid);
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