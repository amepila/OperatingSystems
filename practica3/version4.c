/**
  \archivo		version4.c   
  \descripcion	Esta funcion representa el calculo de pi/4
  				mediante la serie de Leibniz, la cual son 
  				restas y sumas alternadas. En esta version
  				se muestra el calculo con procesos paralelos
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		20/Feb/20
 */

/* Librerias*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

/* Macros*/
#define NPROCESS	4			/* Numero de procesos*/
#define LIMIT		2000000000	/* Iteraciones de serie*/
#define DEBUG		0			/* Macro para habilitar bugeo*/

long double *memshared;			/* Memoria compartida entre procesos*/

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que realiza el calculo de PI/4 mediante procesos paralelos,
  				es decir, cada procesos realiza al mismo tiempo las operaciones
  \entradas  	int idprocess
  \return     	void 
 */
void leibniz(int idprocess)
{
	int counter;								/* Contador de iteraciones en la serie*/
	long double sum = 0;						/* Acumulador local de la sumatoria*/
	int begin =(LIMIT/NPROCESS)*idprocess;		/* Inicio del procesos en la sumatoria*/
	int end = (LIMIT/NPROCESS)*(idprocess+1);	/* Fin del procesos en la sumatoria*/

	/* Ciclo que realiza la sumatoria de la serie de Leibniz*/
	for(counter = begin; counter < end; counter++)
	{
#if DEBUG 
		sum++;	/* Operacion para debugueo*/					
#else
		if(counter % 2 == 0)				/* Alternancia de suma o resta*/
			sum += (1.0/((2*counter) + 1));	/* Si el conteo es divisible entre dos*/
		else 								/*  en entero se suma a la serie*/
			sum -= (1.0/((2*counter) + 1)); /* Si el conteo no es divisible entre dos*/
											/*  es decir, deja residuo diferente de cero, resta*/
#endif
	}
	*memshared += sum;						/* Acumulacion en memoria compartida*/
	exit(0);								/* El proceso sale*/
}

int main(void)
{
	/* Variables para el calculo de tiempo*/
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	int shmid;					/* Variable que guarda la configuracion para los procesos*/
	int counter;				/* Contador utilizado para los procesos*/
	long double sum_total = 0;	/* Suma total que guarda el valor final de la sumatoria*/
	struct shmid_ds buff;		/* Buffer de control para procesos con recursos compartidos*/

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; 		/* Tiempo inicial*/

	/* Se configura que los procesos puedan comunicarse entre si*/
	shmid = shmget(IPC_PRIVATE,NPROCESS*sizeof(double), IPC_CREAT | 0666);
	/* La memoria compartida es el intermediario entre esa comunicacion*/
	memshared = shmat(shmid,NULL,0);

	/* Ciclo encargado de crear procesos que ejecutaran la sumatoria*/
	for (counter = 0; counter < NPROCESS; counter++)
	{
		if(fork() == 0)			/* Creacion de procesos que ejecutaran la serie*/
			leibniz(counter);
	}

	/* Se esperan a que los procesos terminen*/
	for(counter = 0; counter < NPROCESS; counter++)
		wait(NULL);

	sum_total = *memshared;			/* Se pasa el valor final de la memoria compartida*/
	shmdt(memshared);				/* La memoria compartida se desconecta*/
	shmctl(shmid,IPC_RMID,&buff);	/* Se borra la memoria compartida*/

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; 			/* Tiempo final*/

	elapsed_time = stop_ts - start_ts;
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);
	printf("SUMA TOTAL = %LG\n", sum_total);
		
	return 0;
}