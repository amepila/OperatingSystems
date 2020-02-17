#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define LIMIT	10

int main(void)
{
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;
	int counter;
	float sum;

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Tiempo inicial

	sum = 0;
	for(counter = 0; counter < LIMIT; counter++)
	{
		if(counter == 0)
		{
			sum = sum + (float)(1/((2*counter) + 1));
			printf("SUMA TOTAL = %f\n", sum);
			printf("counter = %d\n", counter);
		}
		else if(counter % 2 == 0){
			sum = sum + (float)(1/((2*counter) + 1));
			printf("SUMA TOTAL = %f\n", sum);
			printf("counter = %d\n", counter);

		}
		else if(counter % 2 != 0){
			sum = sum - (float)(1/((2*counter) + 1));
			printf("SUMA TOTAL = %f\n", sum);
			printf("counter = %d\n", counter);
		}
	}

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // Tiempo final

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);
	printf("SUMA TOTAL = %f\n", sum);
	return 0;
}