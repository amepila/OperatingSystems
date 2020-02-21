/**
  \archivo		version3.c   
  \descripcion	Esta funcion representa el calculo de pi/4
  				mediante la serie de Leibniz, la cual son 
  				restas y sumas alternadas. En esta version
  				se muestra el calculo con la clonacion de procesos
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		20/Feb/20
 */

/* Librerias*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>

/* Macros*/
#define NCLONES		4			/* Numero de clones*/
#define LIMIT		2000000000	/* Numero de iteraciones*/
#define	STACK_SIZE	(1024*64)	/* Tamano del stack*/

double sum_total;				/* Suma total de la serie*/


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que realiza el calculo de PI/4 mediante compartiendo clones,
  				es decir, cada clon calcula cierta parte equitativa
  \entradas  	void *args
  \return     	void 
 */
void leibniz(void *args)
{
	int counter;	/* Contador de iteraciones*/
	double sum = 0;	/* Suma local la serie*/

	int idclone = *((int *) args);			/* Numero de clon*/
	int begin =(LIMIT/NCLONES)*idclone;		/* Inicio de calculo del clon*/
	int end = (LIMIT/NCLONES)*(idclone+1);	/* FIn de calculo del clon*/

	/* Ciclo de sumatoria de Leibniz*/
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

	int status;			/* Status del proceso clonado*/
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