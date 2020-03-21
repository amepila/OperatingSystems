#include "semaphore.h"

//Macro que incluye el codigo de la instruccion maquina xchg
#define atomic_xchg(A,B) __asm__ __volatile__(	\
						"lock xchg %1,%0 ;\n"	\
						: "=ir" (A)				\
						: "m" (B), "ir" (A)		\
);
extern int *g;
extern int *h;

void initsem(Semaphore_t *sem, int val)
{
	sem->count = val;
	initqueue(&sem->block_queue);
}

void waitsem(Semaphore_t *sem)
{
	int l = 1;
	do { atomic_xchg(l,*h);} while(l!=0);
	
	sem->count--;
	if(sem->count < 0)
	{
		enqueue(&sem->block_queue,getpid());
		kill(getpid(),SIGSTOP);
	}
	*g = 0;
}

void signalsem(Semaphore_t *sem)
{
	int pid;
	int l = 1;
	do { atomic_xchg(l,*g);} while(l!=0);
	sem->count++;

	if(sem->count <= 0)
	{
		pid = dequeue(&sem->block_queue);
		kill(pid, SIGCONT);
	}
	*h = 0;
}