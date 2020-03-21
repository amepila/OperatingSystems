#include "queue.h"

void initqueue(Queue_t *q)
{
	q->head = 0;
	q->tail = 0;
}

void enqueue(Queue_t *q, int val)
{
	q->elements[q->head] = val;
	// Incrementa al apuntador
	q->head++;
	q->head = q->head%MAXTHREAD;
}

int dequeue(Queue_t *q)
{
	int valret;
	valret = q->elements[q->tail];
	// Incrementa al apuntador
	q->tail++;
	q->tail = q->tail%MAXTHREAD;
	return(valret);
}

int emptyq(Queue_t *q)
{
	return(q->head==q->tail);
}