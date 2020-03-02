/**
  \archivo		schedulerRR.c   
  \descripcion	Esta funcion representa al algoritmo de Colas de
  				retroalimentacion con q = 1
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		01/Mar/20
 */

/* Librerias*/
#include <scheduler.h>
#include <stdio.h>

/* Variables externas*/
extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

/* Tipo de dato de las queue*/
QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];
QUEUE priority[MAXTHREAD];
QUEUE *current=&priority[0];

/* Variables globales*/
int status_ready;

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;
	int counter;
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		/* Un nuevo hilo de maxima prioridad va a la cola de listos*/
		threads[callingthread].status=READY;
		_enqueue(&priority[0],callingthread);
		status_ready=1; /* Indica que hay hilos*/
	}
	
	if(event==BLOCKTHREAD)
	{
		/* El hilo se va a la lista de espera y se cambia de hilo*/
		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);
		changethread=1;
	}

	if(event==ENDTHREAD)
	{
		/* Al momento que el hilo termina, se cambia a otro*/
		threads[callingthread].status=END;
		changethread=1;
	}
	
	if(event==UNBLOCKTHREAD)
	{
		/* El hilo pasa a la cola de listos*/
		threads[callingthread].status=READY;
		_enqueue(&ready,callingthread);
	}

	if(event==TIMER)
	{
		/* Se verifica que el hilo este corriendo*/
		if((status_ready) && (threads[callingthread].status==RUNNING))
		{
			/* Se verifican los hilos*/
			for(counter = 0; counter < MAXTHREAD; counter++)
			{
				/* Si verifica el de prioridad mas alta disponible*/
				if(_emptyq(&priority[counter])==0)
				{
					/* Se coloca al de siguiente prioridad en listos*/
					threads[callingthread].status=READY;
					//_enqueue(&priority[counter+1],callingthread);	

					if(counter < MAXTHREAD-1)
						_enqueue(&priority[counter+1],callingthread);
					else
						_enqueue(&priority[counter],callingthread);
					current=&priority[counter];
					break;
				}
			}
			changethread=1;
		}
	}

	if(changethread)
	{
		/* Se cambia de hilo al de mayor prioridad*/
		if(threads[callingthread].status==END)
		{
			old=currthread;
			next=_dequeue(&ready);
			threads[next].status=RUNNING;
			_swapthreads(old,next);
		}else
		{
			old=currthread;
			next=_dequeue(current);
			threads[next].status=RUNNING;
			_swapthreads(old,next);
		}
		
	}
}
