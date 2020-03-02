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
QUEUE priority_q[MAXTHREAD];

/* Variables globales*/
int status_ready = 1;	/* Estado del listo*/
int current_q = 0;		/* Actual tiempo q*/

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		/* Un nuevo hilo va a la cola de listos*/
		threads[callingthread].status=READY;
		_enqueue(&priority_q[0],callingthread);
		status_ready++;		/* El estado de listo se activa*/
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
		status_ready--;		/*Al terminar el estado de listo vuelve a cero*/
	}
	
	if(event==UNBLOCKTHREAD)
	{
		/* El hilo pasa a la cola de listos*/
		threads[callingthread].status=READY;
		/* Si el tiempo q esta en el maximo o ultimo elemento se desbloquea*/
		if((MAXTHREAD-1) == current_q)
			_enqueue(&priority_q[current_q],callingthread);	/* Se desbloquea el actual*/
		else
			_enqueue(&priority_q[current_q+1],callingthread); /* Sino se desbloquea el siguiente*/
	}

	if(event==TIMER)
	{
		/* Se verifica que el hilo este corriendo*/
		if(threads[callingthread].status==RUNNING)
		{
			threads[callingthread].status=READY;
			/* Si es la unica prioridad se pasa a cola de listos sino el siguiente pasa*/
			if((status_ready == 1)||(MAXTHREAD-1) == current_q)
				_enqueue(&priority_q[current_q],callingthread);
			else
				_enqueue(&priority_q[current_q+1],callingthread);
		}
	}

	if(changethread)
	{
		int counter;
		old=currthread;

		/* Se verifican los hilos*/
		for(counter = 0; counter < MAXTHREAD; counter++)
		{
			/* Si verifica el de prioridad mas alta disponible*/
			if(!_emptyq(&priority_q[counter]))
			{
				/* Se coloca como el siguiente y sale del ciclo*/
				next=_dequeue(&priority_q[counter]);
				current_q = counter;
				break;
			}
		}
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}
}
