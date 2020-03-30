/**
  \archivo		semaphores.c 
  \descripcion	Este archivo contiene las funciones
  				de semaforos, donde solo se maneja 1
  \fecha   		29/Mar/20
 */

/* Librerias*/
#include <stdio.h>
#include "semaphores.h"

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que representa al wait, donde solo permite que un semaforo
  				entre al recurso y el siguiente espera
  \entradas  	int idsem
  \return     	void
 */
void semwait(int idsem)
{
	struct sembuf s;		/* Se crea un tipo de dato semaforo*/
	s.sem_num=0; 			/* Se elige solo un semaforo*/
	s.sem_op=-1;			/* Se resta para que solo accese un semaforo*/
	s.sem_flg=SEM_UNDO;		
	
	semop(idsem,&s,1);		/* Se pasa la las indicaciones a la primitiva*/
	return;
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que representa al signal del semaforo, donde libera al recurso
  				para que otro semaforo pueda entrar
  \entradas  	int idsem
  \return     	void
 */	
void semsignal(int idsem)
{
	struct sembuf s;		/* Se crea un tipo de dato semaforo*/
	s.sem_num=0;			/* Se elige solo un semaforo*/
	s.sem_op=1;				/* Se suma para que accese se accese un elemento*/
	s.sem_flg=SEM_UNDO;
	
	semop(idsem,&s,1);		/* Se pasa la las indicaciones a la primitiva*/
	return;
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que inicializa a un solo semaforo
  \entradas  	int key, int val
  \return     	int
 */	
int initsem(int key, int val)
{
	int semid;
	semid=semget(key,1,0666|IPC_CREAT); 	/* Se solicita espacio para un semaforo*/
	semctl(semid,0,SETVAL,val);				/* Se solicita el acceso inicial de un semaforo*/
	return semid;
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que borra al semaforo despues de haberlo requerido
  \entradas  	int idsem
  \return     	void
 */
void erasesem(int idsem)
{
	semctl(idsem,0,IPC_RMID,0);		/* Se elimina el semaforo*/
	return;
}
