#include "semaphore.h"

//Macro que incluye el codigo de la instruccion maquina xchg
#define atomic_xchg(A,B)	__asm__ __volatile__( \ 
									"	lock xchg %1,%0 ;\n"	\
									: "=ir" (A)					\
									: "m" (B), "ir" (A)			\
									);

extern int *g;
extern int *h;

void initsem(Semaphore_t *sem, int val)
{
	int shmid;

	shmid = shmget(IPC_PRIVATE,sizeof(Semaphore_t *), IPC_CREAT | 0666);
	if(shmid == -1)
		printf("ERROR SHMGET SEMAPHORE\n");
	
	sem = shmat(shmid,NULL,0);
	if(sem == NULL)
		printf("ERROR SHMAT SEMAPHORE\n");

	sem->count = val;
	sem->block = 0;
	initqueue(&sem->block_queue);
}

void waitsem(Semaphore_t *sem)
{
	int l = 1;
	do { atomic_xchg(l, *g);} while(l!=0);
	printf("COUNT = %d\n", sem->count);
	sem->count--;
	//printf("DEBUG\n");
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
	int l;
	do { atomic_xchg(l, *g);} while(l!=0);
	sem->count++;

	if(sem->count <= 0)
	{
		pid = dequeue(&sem->block_queue);
		kill(pid, SIGCONT);
	}
	*h = 0;
}

void clearsem(Semaphore_t *sem)
{
	shmdt(sem);	
}
