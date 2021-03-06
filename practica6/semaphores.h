#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

/**
  \archivo		semaphore.h   
  \descripcion	Este header incluye el las funciones de semaforos
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		29/Mar/20
 */

/* Librerias*/
#include <sys/sem.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 

#define SEM_ID int

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que representa al wait, donde solo permite que un semaforo
  				entre al recurso y el siguiente espera
  \entradas  	int idsem
  \return     	void
 */
void semwait(int idsem);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que representa al signal del semaforo, donde libera al recurso
  				para que otro semaforo pueda entrar
  \entradas  	int idsem
  \return     	void
 */	
void semsignal(int idsem);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que inicializa a un solo semaforo
  \entradas  	int key, int val
  \return     	int
 */	
int initsem(int key, int val);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que borra al semaforo despues de haberlo requerido
  \entradas  	int idsem
  \return     	void
 */
void erasesem(int idsem);

#endif /*SEMAPHORE_H_*/