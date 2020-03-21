#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"

typedef struct Semaphore
{
	int 	count;
	Queue_t block_queue;
} Semaphore_t;

void initsem(Semaphore_t *sem, int val);
void waitsem(Semaphore_t *sem);
void signalsem(Semaphore_t *sem);
void clearsem(Semaphore_t *sem);

#endif /*SEMAPHORE_H_*/