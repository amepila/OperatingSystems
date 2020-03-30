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
#include <sys/wait.h>
#include "messages.h"

#define CICLOS 		10	/* Numero de veces que se ejecuta el proceso*/
#define MAXPROCESS	3	/* Numero de procesos*/

char *pais[3] = {"Peru","Bolivia","Colombia"};	/* Variable que guarda el nombre de los paises*/
int idmsg;		/* Id de la cola de mensajes*/
Msgbuf_t *msg;	/* Apuntador a mensaje*/

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
		msgreceive(idmsg, msg);			/* Se recibe el mensaje y espera*/
		printf("Entra %s ", pais[i]);	/* Entra a la seccion critica*/
		fflush(stdout);					/* Limpieza del buffer*/
		sleep(rand()%3);				/* Espera de hasta 3 segundos*/
		printf("- %s Sale\n", pais[i]);	/* Salida de la seccion critica*/
		msgsend(idmsg, msg);			/* Senal que envia el mensaje para que otro pueda entrar*/
		sleep(rand()%3);				/* Espera aleatoria de la seccion critica */
	}
	exit(0);	/* Termina el proceso*/
}

int main()
{
	int pid;		/* Id de procesos*/
	int status;		/* Estatus de procesos*/
	int counter;	/* Contador de procesos*/

	idmsg = initmsg(IPC_PRIVATE);		/* Creacion del buzon de mensajes*/
	printf("ID = %d\n",idmsg);
	msgsend(idmsg, msg);				/* Se inicia con el envio de un mensaje*/
	srand(getpid());					/* Obtencion random de id de proceso*/

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

	/* Se borra la cola de mensajes*/
	erasemsg(idmsg);
	return 0;
}