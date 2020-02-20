#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

#define NTHREADS 	4
#define LIMIT		2000000000
#define DEBUG		0

double sum_total;

void *leibniz(void *args)
{
	double sum = 0;
	int counter;
	int idhilo = *((int *) args);
	int begin =(LIMIT/NTHREADS)*idhilo;
	int end = (LIMIT/NTHREADS)*(idhilo+1);

	for(counter = begin; counter < end; counter++)
	{
		if(counter % 2 == 0)
			sum += (1.0/((2*counter) + 1));
		else
			sum -= (1.0/((2*counter) + 1));;
	}
	sum_total = sum_total + sum;
}

int main(void)
{
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	int counter;
	int param[NTHREADS];
	pthread_t tid[NTHREADS];

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Tiempo inicial

	for (counter = 0; counter < NTHREADS; counter++)
	{
		param[counter] = counter;
		pthread_create(&tid[counter],NULL,leibniz,(void *)&param[counter]);
	}

	//Esperar a que los hilos terminen
	for (counter = 0; counter < NTHREADS; counter++)
		pthread_join(tid[counter],NULL);

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);
	printf("SUMA TOTAL = %lf\n", sum_total);
	return 0;
}