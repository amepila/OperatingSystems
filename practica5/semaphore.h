#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdio.h>

typedef struct Semaphore
{
	int count;
	int 
	
} Semaphore_t;

void initsem();
void waitsem();
void signalsem();

#endif /*SEMAPHORE_H_*/