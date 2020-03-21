#ifndef QUEUE_H_
#define QUEUE_H_

#include <ucontext.h>
#include <stdio.h>
#include <unistd.h>

#define MAXTHREAD 3

typedef struct Queue 
{
	int elements[MAXTHREAD];
	int head;
	int tail;
} Queue_t;

void initqueue(Queue_t *q);
void enqueue(Queue_t *q, int val);
int dequeue(Queue_t *q);
int emptyq(Queue_t *q);

#endif /*QUEUE_H_*/