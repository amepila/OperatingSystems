/**
  \archivo		schedulerRR2.c   
  \descripcion	Esta funcion representa al algoritmo de Round Robin
  				con q = 2
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		01/Mar/20
 */

/* Librerias*/
#include <scheduler.h>

/* Variables externas*/
extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

/* Tipo de dato de las queue*/
QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

/* Variables globales*/
int timeq;	/* Tiempo de q*/

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion RR con q=2, que se activa cuando el hilo lo solicita
 				de manera que su entrada, los primeros 8 bit son el id del 
 				hilo y los otros 8 bits son el evento por el cual manda a llamar
 				este algoritmo de planificacion
  \entradas  	arguments
  \return     	void
 */
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
		_enqueue(&ready,callingthread);
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
		if(threads[callingthread].status==RUNNING)
		{
			/* Tiempo q aumenta en uno*/
			timeq++;

			/*Si q llega a 2, el hilo se pasa a la cola de listos*/
			if(timeq == 2)
			{
				threads[callingthread].status=READY;
				_enqueue(&ready,callingthread);
				changethread=1;		/* Se cambia de hilo*/
				timeq = 0;			/* Tiempo q se reinicia*/
			}
		}
	}
	
	if(changethread)
	{
		/* Se cambia de hilo al siguiente de la cola de listos*/
		old=currthread;
		next=_dequeue(&ready);
		
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}
}
