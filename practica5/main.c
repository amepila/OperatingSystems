#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "semaphore.h"

#define CICLOS 10

char *pais[3] = {"Peru","Bolivia","Colombia"};
int *g;
int *h;
Semaphore_t *Semp;

void proceso(int i)
{
	int k;
	for(k = 0; k<CICLOS; k++)
	{
		waitsem(Semp);

		printf("Entra %s ", pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("- %s Sale\n", pais[i]);

		signalsem(Semp);
		sleep(rand()%3);
	}
	exit(0);
}

int main()
{
	int shmid_g, shmid_h;
	int pid;
	int status;
	int counter;

	smid_g = shmget(IPC_PRIVATE,sizeof(int *), IPC_CREAT | 0666);
	if(shmid_g == -1)
		printf("ERROR SHMGET G\n");

	smid_h = shmget(IPC_PRIVATE,sizeof(int *), IPC_CREAT | 0666);
	if(shmid_h == -1)
		printf("ERROR SHMGET H\n");

	g = shmat(shmid_g,NULL,0);
	if(g == NULL)
		printf("ERROR SHMAT G");

	h = shmat(shmid_h,NULL,0);
	if(h == NULL)
		printf("ERROR SHMAT G");

	initsem(Semp,1);

	srand(getpid());
	for(counter = 0; counter < MAXTHREAD; counter++)
	{
		pid = fork();
		if(pid == 0)
			proceso(counter);
	}

	for(counter = 0; counter < MAXTHREAD; counter++)
		pid = wait(&status);

	clearsem(Semp);
	shmdt(g);
	shmdt(h);

	return 0;
}