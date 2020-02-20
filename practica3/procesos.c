#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define NPROCESS	4
#define LIMIT		2000

double *sum_total;

int leibniz(void *args)
{
	int counter;
	double sum = 0;

	int idprocess = *((int *) args);
	int begin =(LIMIT/NPROCESS)*idprocess;
	int end = (LIMIT/NPROCESS)*(idprocess+1);

	printf("Proceso %d inicia en %d y termina en %d\n", idprocess,begin,end);
	for(counter = begin; counter < end; counter++)
	{
		/*
		if(counter % 2 == 0)
			sum += (1.0/((2*counter) + 1));
		else
			sum -= (1.0/((2*counter) + 1));
			*/
		sum++;
	}

	*sum_total += sum;
	printf("sum = %lf\n", *sum_total);

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
	int shmid;
	pid_t pid;
	int counter;
	int param[NPROCESS];

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Tiempo inicial

	shmid = shmget(IPC_PRIVATE,sizeof(sum_total), IPC_CREAT | 0666);
	sum_total = shmat(shmid,NULL,0);
	for (counter = 0; counter < NPROCESS; counter++)
	{
		if(fork() == 0)
		{
			leibniz((void *)&counter);
		}
		else
		{
			wait(NULL);
			shmdt(sum_total);
		}
	}

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);
	printf("SUMA TOTAL = %lf\n", *sum_total);
		
	return 0;
}