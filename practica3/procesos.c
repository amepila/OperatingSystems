#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define NPROCESS	4
#define LIMIT		2000000000

double sum_total;

int leibniz(void *args)
{
	int counter;
	double sum = 0;

	int idprocess = *((int *) args);
	int begin =(LIMIT/NPROCESS)*idprocess;
	int end = (LIMIT/NPROCESS)*(idprocess+1);

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
	pid_t pid[NPROCESS];
	int counter;
	int param[NPROCESS];

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Tiempo inicial

	shmget(IPC_PRIVATE,sizeof(sum_total), IPC_CREATE);
	shmat();

	for (counter = 0; counter < NPROCESS; counter++)
	{
		pid[counter] = fork();
		if(pid[counter] == 0)
			leibniz((void *)&counter);
		else
			printf("ERROR FORK\n");
	}

	for(counter = 0; counter < NPROCESS; counter++)
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

	return 0;
}