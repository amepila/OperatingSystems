#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "messages.h"

#define CICLOS 10

char *pais[3] = {"Peru","Bolivia","Colombia"};

int *g;

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
	int l;
								
	for(k = 0; k<CICLOS; k++)
	{
		printf("Entra %s ", pais[i]);	/* Entra a la seccion critica*/
		fflush(stdout);					/* Limpieza del buffer*/
		sleep(rand()%3);				/* Espera de hasta 3 segundos*/
		printf("- %s Sale\n", pais[i]);	/* Salida de la seccion critica*/
		sleep(rand()%3);				/* Espera aleatoria de la seccion critica */
	}
	exit(0);	/* Termina el proceso*/
}

int main()
{
	int pid;
	int status;
	int args[3];
	int i;

	srand(getpid());
	for(i = 0; i < 3; i++)
	{
		// Crea un nuevo proceso hijo que ejecuta la funcion proceso ()
		pid = fork();
		if(pid == 0)
			procesp(i);
	}

	for(i = 0; i < 3; i++)
		pid = wait(&status);

	return 0;
}