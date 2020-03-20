#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//Macro que incluye el codigo de la instruccion maquina xchg
#define atomic_xchg(A,B)	_asm_volatile( \ 
									"	lock xchg %1,%0 ;\n"	\
									: "=ir" (A)					\
									: "m" (B), "ir" (A)			\
									);

#define CICLOS 10

typedef struct Semaphore
{
	
} Semaphore_t;

char *pais[3] = {"Peru","Bolivia","Colombia"};
int *g;

void initsem()
{

}

void waitsem()
{

}

void signalsem()
{

}

void proceso(int i)
{
	int k;
	for(k = 0; k<CICLOS; k++)
	{
		waitsem(sem);

		printf("Entra %s ", pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("- %s Sale\n", pais[i]);

		signalsem(sem);

		sleep(rand()%3);
	}
	exit(0);
}

int main()
{
	return 0;
}