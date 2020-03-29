/**
  \archivo		solucionmsg.c  
  \descripcion	Este archivo fuente representa al main
  				donde se ejecutan los procesos con sistema
  				de sincronizacion con mensajes
  \fecha   		29/Mar/20
 */

/* Librerias*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "semaphores.h"

#define CICLOS 		10	/* Numero de veces que se ejecuta el proceso*/
#define MAXPROCESS	3	/* Numero de procesos*/

char *pais[3] = {"Peru","Bolivia","Colombia"};	/* Variable que guarda el nombre de los paises*/
int *idsem;	/* Apuntador a semaforo*/

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que simula un proceso entrando a la zona critica
  \entradas  	int i
  \return     	void
 */
void proceso(int i)
{
	int k;	/* Variable contador*/
								
	for(k = 0; k<CICLOS; k++)
	{
		semwait(*idsem);
		printf("Entra %s ", pais[i]);	/* Entra a la seccion critica*/
		fflush(stdout);					/* Limpieza del buffer*/
		sleep(rand()%3);				/* Espera de hasta 3 segundos*/
		printf("- %s Sale\n", pais[i]);	/* Salida de la seccion critica*/
		semsignal(*idsem);
		sleep(rand()%3);				/* Espera aleatoria de la seccion critica */
	}
	exit(0);	/* Termina el proceso*/
}

int main()
{
	int shmid;		/* Id de memoria compartida del semaforo*/
	int pid;		/* Id de procesos*/
	int status;		/* Estatus de procesos*/
	int counter;	/* Contador de procesos*/

	/**CONFIGURACION PARA COMUNICACION DE LA MEMORIA COMPARTIDA*/
	
	/* Configuracion para que los procesos puedan comunicarse con el semaforo*/
	shmid = shmget(IPC_PRIVATE,sizeof(idsem), IPC_CREAT | 0666);
	if(shmid == -1)		/* Verificacion de error en configuracion*/
		printf("ERROR SHMGET SEMAPHORE\n");

	/** ASIGNACION DE LA MEMORIA COMPARTIDA*/

	/* Asignacion de la memoria compartida del semaforo*/
	idsem = shmat(shmid,NULL,0);
	if(idsem == NULL)	/* Verificacion de error en asignacion*/
		printf("ERROR SHMAT SEMAPHORE\n");

	*idsem = initsem(IPC_PRIVATE,1);	/*Inicializacion de semaforo en 1*/
	srand(getpid());					/*Obtencion random de id de proceso*/

	/* Creacion de procesos*/
	for(counter = 0; counter < MAXPROCESS; counter++)
	{
		/* Crea un nuevo proceso hijo que ejecuta la funcion proceso ()*/
		pid = fork();
		if(pid == 0)
			proceso(counter);
	}

	/* Espera de procesos*/
	for(counter = 0; counter < MAXPROCESS; counter++)
		pid = wait(&status);

	/* Se borra y libera la memoria compartida*/
	shmdt(idsem);
	erasesem(*idsem);
	return 0;
}