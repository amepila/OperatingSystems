#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "queue.h"

typedef struct Semaphore
{
	int 	count;
	int 	block;
	Queue_t block_queue;
} Semaphore_t;

void initsem(Semaphore_t *sem, int val);
void waitsem(Semaphore_t *sem);
void signalsem(Semaphore_t *sem);
void clearsem(Semaphore_t *sem);

#endif /*SEMAPHORE_H_*/