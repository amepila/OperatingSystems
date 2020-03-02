#include <scheduler.h>
#include <stdio.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];
QUEUE priority_q[MAXTHREAD];

int status_ready = 1;
int current_q = 0;

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[callingthread].status=READY;
		_enqueue(&priority_q[0],callingthread);
		status_ready++;
	}
	
	if(event==BLOCKTHREAD)
	{

		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);

		changethread=1;
	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		changethread=1;
		status_ready--;
	}
	
	if(event==UNBLOCKTHREAD)
	{
			threads[callingthread].status=READY;
			if((MAXTHREAD-1) == current_q)
				_enqueue(&priority_q[current_q],callingthread);
			else
				_enqueue(&priority_q[current_q+1],callingthread);
	}

	if(event==TIMER)
	{
		if(threads[callingthread].status==RUNNING)
		{
			threads[callingthread].status=READY;
			if((status_ready ==1)||(MAXTHREAD-1) == current_q)
				_enqueue(&priority_q[current_q],callingthread);
			else
				_enqueue(&priority_q[current_q+1],callingthread);
		}
	}

	if(changethread)
	{
		int counter;
		old=currthread;

		for(counter = 0; counter < MAXTHREAD; counter++)
		{
			if(!_emptyq(&priority_q[counter]))
			{
				next=_dequeue(&priority_q[counter]);
				current_q = counter;
				break;
			}
		}
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}
}
