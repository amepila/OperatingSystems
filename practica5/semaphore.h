#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

/**
  \archivo		semaphore.h   
  \descripcion	Este header incluye el tipo de dato de
  				las colas y sus funciones asociadas
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		21/Mar/20
 */

/* Librerias*/
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"

/*! Tipo de dato de un semaforo*/
typedef struct Semaphore
{
	int 	count;			/* Contador del semaforo*/
	Queue_t block_queue;	/* Cola de procesos bloqueados*/
} Semaphore_t;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que inicializa el semaforo con un valor de contador inicial
  \entradas  	Semaphore_t *sem, int val
  \return     	void
 */
void initsem(Semaphore_t *sem, int val);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que funge como la espera de un semaforo
  \entradas  	Semaphore_t *sem
  \return     	void
 */
void waitsem(Semaphore_t *sem);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que habilita la senal para despertar un semaforo
  \entradas  	Semaphore_t *sem
  \return     	void
 */
void signalsem(Semaphore_t *sem);

#endif /*SEMAPHORE_H_*/