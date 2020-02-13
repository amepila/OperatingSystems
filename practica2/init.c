/**
  \archivo		init.c   
  \descripcion	Esta funcion representa al proceso init,
  				que se encarga de inicializar 6 procesos
  				getty en terminal Xterm. Cada vez que un
  				procesos o ventana Xterm sea cerrada, se 
  				crea otro proceso, es decir, siempre tiene
  				6 procesos gettys abiertos en Xterm
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		12/Feb/20
 */

/* Librerias*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Macros*/
#define	NUMBER_CHILDS	6	/* Numero de procesos a crear*/

int main(void)
{
	/* Contador de los 6 procesos getty*/
	int counter = 0;

	/* Loop infinito debido a que siempre se vigilara la existencia
		de 6 procesos getty, cuando uno se cierra en la ventana Xterm
		se inicializa otro, manteniendo continua esta tarea
	*/
	for(;;)
	{
		/* Mientras el numero de procesos sean 6, estos se desplegaran en Xterm*/
		while (counter < NUMBER_CHILDS)
		{
			if(fork() == 0) /* Cuando el proceso se crea se ejecuta en Xterm*/
				execl("/usr/bin/xterm","xterm","-hold","-e","./getty","getty",NULL);
			counter++;		/* Contador que controla el numero de procesos*/
		}
		wait(NULL);			/* Se espera a que los procesos creados realicen su trabajo*/
		counter--;			/* En caso de que un proceso haya terminado al cerrarse su ventana
								este decrementara en uno para regresar al while a crear el 
								proceso faltante
							*/
	}
	return 0;
}
