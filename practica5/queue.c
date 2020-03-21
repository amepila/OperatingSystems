/**
  \archivo		queue.c   
  \descripcion	Este archivo fuente contiene las funciones
  				declaradas en el header asociadas a las colas
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		21/Mar/20
 */

/* Librerias*/
#include "queue.h"

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que inicializa la cola, es decir, la pone en 0
  \entradas  	Queue_t *q
  \return     	void 
 */
void initqueue(Queue_t *q)
{
	q->head = 0;	/* Inicializa el primer elemento de la cola en 0*/
	q->tail = 0;	/* Inicializa el ultimo elemento de la cola en 0*/
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que mete un elemento a la cola
  \entradas  	Queue_t *q
  \return     	void 
 */
void enqueue(Queue_t *q, int val)
{
	q->elements[q->head] = val;		/* Mete el elemento en la primera posicion*/
	q->head++;						/* Incrementa el apuntador del primer elemento*/
	q->head = q->head%MAXPROCESS;
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que saca un elemento de la cola
  \entradas  	Queue_t *q
  \return     	Elemento de la cola 
 */
int dequeue(Queue_t *q)
{
	int valret;						/* Variable para contener al elemento extraido*/
	valret = q->elements[q->tail];	/* Se extrae el ultimo elemento*/
	q->tail++;						/* Incrementa el apuntador del ultimo elemento*/
	q->tail = q->tail%MAXPROCESS;
	return(valret);					/* Regresa el ultimo elemento*/
}