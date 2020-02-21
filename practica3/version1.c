/**
  \archivo		version1.c   
  \descripcion	Esta funcion representa el calculo de pi/4
  				mediante la serie de Leibniz, la cual son 
  				restas y sumas alternadas. En esta version
  				se muestra el calculo de manera serial.
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		20/Feb/20
 */

/* Librerias*/
#include <stdio.h>
#include <sys/time.h>

/* Macros*/
#define LIMIT	2000000000	/* Numero de iteraciones*/
#define DEBUG	0			/* Macro para habilitar bugeo*/

int main(void)
{
	/* Variables para el calculo de tiempo*/
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	int counter;			/* Contador de iteraciones*/
	long double sum = 0;	/* Inicializacion de la suma total*/

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; 	/* Tiempo inicial*/

	/* Ciclo que representa la sumatoria de la serie*/
	for(counter = 0; counter < LIMIT; counter++)
	{
#if DEBUG 
		sum++;	/* Operacion para debugueo*/					
#else
		if(counter % 2 == 0)				/* Alternancia de suma o resta*/
			sum += (1.0/((2*counter) + 1));	/* Si el conteo es divisible entre dos*/
		else 								/*  en entero se suma a la serie*/
			sum -= (1.0/((2*counter) + 1));	/* Si el conteo no es divisible entre dos*/
											/*  es decir, deja residuo diferente de cero, resta*/
#endif
	}
	gettimeofday(&ts, NULL);				
	stop_ts = ts.tv_sec;	/* Tiempo final*/

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);
	printf("SUMA TOTAL = %LG\n", sum);
	return 0;
}