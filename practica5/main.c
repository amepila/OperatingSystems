/**
  \archivo		semaphore.h   
  \descripcion	Este archivo fuente representa al main
  				donde se ejecutan los procesos, para
  				poder entrar a la zona critica 
  				Andrea Miriam Perez Huizar 
  \fecha   		21/Mar/20
 */

/* Librerias*/
#include "semaphore.h"

/* Definicion de numero de veces que se ejecuta el proceso*/
#define CICLOS 10

/* Variable que guarda el nombre de los paises*/
char *pais[MAXPROCESS] = {"Peru","Bolivia","Colombia"};
int *g = 0;			/* Variable auxiliar para funcion atomica en wait*/
int *h = 0;			/* Variable auxiliar para funcion atomica en semaforo*/
Semaphore_t *Semp;	/* Apuntador a semaforo*/

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
	int k;								/* Variable contador*/
	for(k = 0; k<CICLOS; k++)
	{
		waitsem(Semp);					/* El semaforo espera*/
		printf("Entra %s ", pais[i]);	/* Impresion de entrada del proceso*/
		fflush(stdout);					/* Limpieza del buffer*/
		sleep(rand()%3);				/* Espera de hasta 3 segundos*/
		printf("- %s Sale\n", pais[i]);	/* Impresion de salida del proceso*/
		fflush(stdout);					/* Limpieza del buffer*/
		signalsem(Semp);				/* Senal para habilitar a un semaforo*/
		sleep(rand()%3);				/* Espera de hasta 3 segundos*/
	}
	exit(0);
}

int main(void)
{
	int shmid;		/* Id de memoria compartida del semaforo*/
	int shmid_g;	/* Id de memoria compartida de variable g*/
	int shmid_h;	/* Id de memoria compartida de variable h*/
	int pid;		/* Id de procesos*/
	int status;		/* Estatus de procesos*/
	int counter;	/* Contador de procesos*/

	/**CONFIGURACION PARA COMUNICACION DE LA MEMORIA COMPARTIDA*/
	
	/* Configuracion para que los procesos puedan comunicarse con el semaforo*/
	shmid = shmget(0x1234,sizeof(Semaphore_t *), IPC_CREAT | 0666);
	if(shmid == -1)		/* Verificacion de error en configuracion*/
		printf("ERROR SHMGET SEMAPHORE\n");

	/* Configuracion para que los procesos puedan comunicarse con la variable g*/
	shmid_g = shmget(0x5678,sizeof(g), IPC_CREAT | 0666);
	if(shmid_g == -1)	/* Verificacion de error en configuracion*/
		printf("ERROR SHMGET G\n");

	/* Configuracion para que los procesos puedan comunicarse con la variable h*/
	shmid_h = shmget(0x9012,sizeof(h), IPC_CREAT | 0666);
	if(shmid_h == -1)	/* Verificacion de error en configuracion*/
		printf("ERROR SHMGET H\n");
	
	/** ASIGNACION DE LA MEMORIA COMPARTIDA*/

	/* Asignacion de la memoria compartida del semaforo*/
	Semp = shmat(shmid,NULL,0);
	if(Semp == NULL)	/* Verificacion de error en asignacion*/
		printf("ERROR SHMAT SEMAPHORE\n");

	/* Asignacion de la memoria compartida de variable g*/
	g = shmat(shmid_g,NULL,0);
	if(g == NULL)		/* Verificacion de error en asignacion*/
		printf("ERROR SHMAT G");

	/* Asignacion de la memoria compartida de variable h*/
	h = shmat(shmid_h,NULL,0);
	if(h == NULL)		/* Verificacion de error en asginacion*/
		printf("ERROR SHMAT G");

	initsem(Semp,1);	/*Inicializacion de semaforo en 1*/

	srand(getpid());	/*Obtencion random de id de proceso*/
	
	/* Creacion de procesos*/
	for(counter = 0; counter < MAXPROCESS; counter++)
	{
		pid = fork();
		if(pid == 0)
			proceso(counter);
	}

	/* Espera de procesos*/
	for(counter = 0; counter < MAXPROCESS; counter++)
		pid = wait(&status);
	
	/* Se borra y libera la memoria compartida*/
	shmdt(Semp);
	shmdt(g);
	shmdt(h);

	return 0;
}