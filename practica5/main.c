#include "semaphore.h"

#define CICLOS 10

char *pais[3] = {"Peru","Bolivia","Colombia"};
int *g = 0;
int *h = 0;
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

		//fflush(stdout);
		signalsem(Semp);
		sleep(rand()%3);
	}
	exit(0);
}

int main(void)
{
	int shmid_g, shmid_h;
	int pid;
	int status;
	int counter;

	shmid_g = shmget(0x5678,sizeof(g), IPC_CREAT | 0666);
	if(shmid_g == -1)
		printf("ERROR SHMGET G\n");

	shmid_h = shmget(0x9012,sizeof(h), IPC_CREAT | 0666);
	if(shmid_h == -1)
		printf("ERROR SHMGET H\n");
	
	g = shmat(shmid_g,NULL,0);
	if(g == NULL)
		printf("ERROR SHMAT G");

	h = shmat(shmid_h,NULL,0);
	if(h == NULL)
		printf("ERROR SHMAT G");

	*g = 0;
	*h = 0;
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