#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define NPROCESS	4
#define LIMIT		2000000000

long double *memshared;

void leibniz(int idprocess)
{
	int counter;
	long double sum = 0;
	int begin =(LIMIT/NPROCESS)*idprocess;
	int end = (LIMIT/NPROCESS)*(idprocess+1);

	for(counter = begin; counter < end; counter++)
	{
		
		if(counter % 2 == 0)
			sum += (1.0/((2*counter) + 1));
		else
			sum -= (1.0/((2*counter) + 1));
		
	}
	*memshared += sum;
	exit(0);
}

int main(void)
{
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	int shmid;
	int counter;
	long double sum_total = 0;
	struct shmid_ds buff;

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Tiempo inicial

	shmid = shmget(IPC_PRIVATE,NPROCESS*sizeof(double), IPC_CREAT | 0666);
	memshared = shmat(shmid,NULL,0);
	for (counter = 0; counter < NPROCESS; counter++)
	{
		if(fork() == 0)
			leibniz(counter);
	}

	for(counter = 0; counter < NPROCESS; counter++)
		wait(NULL);

	sum_total = *memshared;
	shmdt(memshared);
	shmctl(shmid,IPC_RMID,&buff);

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);
	printf("SUMA TOTAL = %LG\n", sum_total);
		
	return 0;
}