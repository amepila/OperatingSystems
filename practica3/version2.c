/**
  \archivo		version2.c   
  \descripcion	Esta funcion representa el calculo de pi/4
  				mediante la serie de Leibniz, la cual son 
  				restas y sumas alternadas. En esta version
  				se muestra el calculo con hilos POSIX
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		20/Feb/20
 */

/* Librerias*/
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

/* Macros*/
#define NTHREADS 	4			/* Numero de hilos*/
#define LIMIT		2000000000	/* Numero de iteraciones*/
#define DEBUG		0			/* Macro para habilitar bugeo*/

double sum_total;				/* Suma total global de la serie*/

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que realiza el calculo de PI/4 mediante compartiendo hilos
  				POSIX, es decir, cada hilo calcula cierta parte equitativa
  \entradas  	void *args
  \return     	void *
 */
void *leibniz(void *args)
{
	double sum = 0;							/* Acumulador de suma*/
	int counter;							/* Contador de iteraciones*/
	int idhilo = *((int *) args);			/* Numero de hilo*/
	int begin =(LIMIT/NTHREADS)*idhilo;		/* Inicio de iteracion del hilo*/
	int end = (LIMIT/NTHREADS)*(idhilo+1);	/* Fin de iteracion del hilo*/

	/* Ciclo que realiza la sumatoria de la serie de Leibniz*/
	for(counter = begin; counter < end; counter++)
	{
		if(counter % 2 == 0)				/* Alternancia de suma o resta*/
			sum += (1.0/((2*counter) + 1));	/* Si el conteo es divisible entre dos*/
		else 								/*  en entero se suma a la serie*/
			sum -= (1.0/((2*counter) + 1));	/* Si el conteo no es divisible entre dos*/
	}										/*  es decir, deja residuo diferente de cero, resta*/
	sum_total = sum_total + sum;			/* Acumulacion de las sumas en variable global*/
}

int main(void)
{
	/* Variables para el calculo de tiempo*/
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	int counter;				/* Contador de procesos*/
	int param[NTHREADS];		/* Parametros de procesos a la funcion*/
	pthread_t tid[NTHREADS];	/* ID de los hilos*/

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; 		/* Tiempo inicial*/

	/* Ciclo de la creacion y paso de parametros de hilos*/
	for (counter = 0; counter < NTHREADS; counter++)
	{
		param[counter] = counter;	/* Parametro es el numero de hilo*/
		pthread_create(&tid[counter],NULL,leibniz,(void *)&param[counter]);
	}

	/* Ciclo de espera de los hilos creados*/
	for (counter = 0; counter < NTHREADS; counter++)
		pthread_join(tid[counter],NULL);

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; /* Tiempo final*/

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);
	printf("SUMA TOTAL = %lf\n", sum_total);
	return 0;
}