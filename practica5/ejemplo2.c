
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

// Macro que incluye el código de la instrucción máquina xchg
#define atomic_xchg(A,B) __asm__ __volatile__(	\
						"lock xchg %1,%0 ;\n"	\
						: "=ir" (A)				\
						: "m" (B), "ir" (A)		\
);

#define CICLOS 10

char *pais[3] = {"Peru","Bolivia","Colombia"};

int *g;

void proceso(int i)
{	
	int k;
	int l;
	
	for(k = 0; k < CICLOS;k++)
	{
		l = 1;
		do
		{ atomic_xchg(l,*g); } while(l != 0);

		printf("Entra %s",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("- %s Sale\n",pais[i]);
 
 		l=1;
		*g=0;

		/* Espera aleatoria fuera de la seccion critica*/
		sleep(rand()%3);
	}
	/* Termina el proceso*/
	exit(0);
}

int main(int argc, char **argv)
{
	int pid;
	int status;
	int shmid;
	int args[3];
	int i;
	void *thread_result;
	
	// Solicitar memoria compartida
	shmid=shmget(0x1234,sizeof(g),0666|IPC_CREAT);
	if(shmid==-1)
	{
		perror("Error en la memoria compartida\n");
		exit(1);
	}
	// Conectar la variable a la memoria compartida
	g=shmat(shmid,NULL,0);

	if(g==NULL)
	{
		perror("Error en el shmat\n");
		exit(2);
	}

	*g=0;
	
	srand(getpid());
	
	for(i=0;i<3;i++)
	{
		// Crea un nuevo proceso hijo que ejecuta la función proceso()
		pid=fork();
		if(pid==0)
			proceso(i);
	}
	for(i=0;i<3;i++)
		pid = wait(&status);
	
	// Eliminar la memoria compartida
	shmdt(g);
}