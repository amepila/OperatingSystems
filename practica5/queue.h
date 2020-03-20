#ifndef QUEUE_H_
#define QUEUE_H_

#include <ucontext.h>
#include <stdio.h>
#include <unistd.h>

typedef struct QUEUE 
{
	int elements[MAXTHREAD];
	int head;
	int tail;
} QUEUE;

void initqueue(QUEUE *q);
void enqueue(QUEUE *q, int val);
int dequeue(QUEUE *q);
int emptyq(QUEUE *q);

#endif /*QUEUE_H_*/