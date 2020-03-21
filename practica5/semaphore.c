/**
  \archivo		semaphore.c   
  \descripcion	Este archivo fuente define las funciones 
  				asociadas con los semaforos declarados en el header
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		21/Mar/20
 */

/* Librerias*/
#include "semaphore.h"

/* Macro que incluye el codigo de la instruccion maquina xchg*/
#define atomic_xchg(A,B) __asm__ __volatile__(	\
						"lock xchg %1,%0 ;\n"	\
						: "=ir" (A)				\
						: "m" (B), "ir" (A)		\
);

/* Variables extraidas del main.c con extern*/
extern int *g;
extern int *h;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que inicializa el semaforo con un valor de contador inicial
  \entradas  	Semaphore_t *sem, int val
  \return     	void
 */
void initsem(Semaphore_t *sem, int val)
{
	sem->count = val;				/* Inicializacion del contador del semaforo*/
	initqueue(&sem->block_queue);	/* Inicializacion de la cola de bloqueados*/
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que funge como la espera de un semaforo
  \entradas  	Semaphore_t *sem
  \return     	void
 */
void waitsem(Semaphore_t *sem)
{
	int l = 1;								/* Variable para que entra a la funcion atomica*/
	do { atomic_xchg(l,*h);} while(l!=0);	/* Funcion atomica*/
	
	sem->count--;							/* Decremento de contador					*/
	if(sem->count < 0)						/* Si el contador del semaforos 			*/
	{										/*  es menor a cero indica que 				*/
		enqueue(&sem->block_queue,getpid());/*  alguien esta en la zona critica 		*/
		kill(getpid(),SIGSTOP);				/*  el proceso va a la cola de bloqueados	*/
	}										/*  es decir, se para el proceso 			*/
	*g = 0;									
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que habilita la senal para despertar un semaforo
  \entradas  	Semaphore_t *sem
  \return     	void
 */
void signalsem(Semaphore_t *sem)
{
	int pid;					
	int l = 1;
	do { atomic_xchg(l,*g);} while(l!=0);
	sem->count++;

	if(sem->count <= 0)
	{
		pid = dequeue(&sem->block_queue);
		kill(pid, SIGCONT);
	}
	*h = 0;
}