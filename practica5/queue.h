#ifndef QUEUE_H_
#define QUEUE_H_

/**
  \archivo		queue.h   
  \descripcion	Este header incluye el tipo de dato de
  				las colas y sus funciones asociadas
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		21/Mar/20
 */

/* Librerias*/
#include <stdio.h>
#include <unistd.h>
#include <ucontext.h>

/** Numero de procesos en el programa*/
#define MAXPROCESS 3

/*! Tipo de dato de una cola*/
typedef struct Queue 
{
	int elements[MAXPROCESS];	/* Numero de elementos en la cola*/
	int head;					/* Primer elemento de la cola*/
	int tail;					/* Ultimo elemento de la cola*/
} Queue_t;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que inicializa la cola, es decir, la pone en 0
  \entradas  	Queue_t *q
  \return     	void 
 */
void initqueue(Queue_t *q);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que mete un elemento a la cola
  \entradas  	Queue_t *q
  \return     	void 
 */
void enqueue(Queue_t *q, int val);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que saca un elemento de la cola
  \entradas  	Queue_t *q
  \return     	Elemento de la cola 
 */
int dequeue(Queue_t *q);

#endif /*QUEUE_H_*/