/**
  \archivo		sh.c   
  \descripcion	Este archivo representa al proceso sh que funge
  				como shell o interpretador de comandos, en este 
  				caso solo puede aceptar comandos con un solo 
  				argumento, el comando EXIT, sale de sesion y el
  				comando SHUTDOWN, cierra todos los procesos
  \autores 		Jose Andres Hernandez Hernandez 
  				Andrea Miriam Perez Huizar 
  \fecha   		12/Feb/20
 */

/* Librerias*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Macros*/
#define LENGTH_TEXT 30	/* Longitud de los strings	*/
#define ASCII_SPACE	32	/* Codigo ASCII de espacio	*/

int main(void)
{
	char *argv[3] = {NULL,NULL,NULL};	/* Arreglo de strings para comandos y parametros*/
	char command[LENGTH_TEXT] = {0};	/* String donde se guardara el comando introducido*/
	char temp1[LENGTH_TEXT] = {0};		/* Variable temporal para comando*/
	char temp2[LENGTH_TEXT] = {0};		/* Variable temporal para argumentos de comandos*/

	int counter1;						/* Contador para identificar comando*/
	int counter2;						/* Contador para identificar argumentos*/
	int len_cmd;						/* Longitud de comandos con sus argumentos*/
	int flag_space;						/* Bandera que indica la existencia de espacios*/
	const char amp[LENGTH_TEXT] = "&";	/* Constante que almacena al amperson*/

	/* Ciclo infinito ya que este siempre estara pidiendo comandos al usuario*/
	for(;;)
	{
		printf("sh > ");
		fgets(temp1,LENGTH_TEXT,stdin);			/* Peticion de comando a usuario*/
		memcpy(command,temp1,strlen(temp1)-1);	/* Almacenamiento en string comando*/
		memset(temp1,0,strlen(temp1));			/* Vaciado total de variable temporal*/

		len_cmd = strlen(command);				/* Longitud del comando ingresado*/
		flag_space = 0;							/* Inicializacion de bandera de espacio*/

		/* Deteccion de espacio en comando, que indica que tiene argumentos*/
		for(counter1 = 0; counter1 <= len_cmd; counter1++)
		{
			if(command[counter1] == ASCII_SPACE)
				flag_space = 1;					/* Si hay espacio, la bandera se activa*/
		}
		
		/* Si existe espacio se ejecuta la siguiente rutina para obtener argumentos*/
		if(flag_space)
		{
			/* Se guarda el comando en la variable temporal 1*/
			for(counter1 = 0; command[counter1] != ASCII_SPACE; counter1++)
				temp1[counter1] = command[counter1];
			counter1++;		/* Se aumenta 1 para evitar tomar el espacio*/
			counter2 = 0;	/* Se inicializa el contador 2*/

			/* El contador 1 continua con lo ingresado por el usuario*/
			for(;counter1 <= len_cmd; counter1++)
			{
				temp2[counter2] = command[counter1];	/* Se guarda el argumento*/
				counter2++;								/* Guarda en el siguiente espacio*/
			}
			memset(command,0,strlen(command));			/* Vaciado en cero de comando*/
			memcpy(command,temp1,strlen(temp1));		/* Asignacion de comando separado*/
			argv[1] = temp2;							/* Asignacion de argumento separado*/
		}

		if(strcmp(command,"exit") == 0)					/* Comparacion con comando EXIT*/
		{
			execlp("./getty","getty",NULL);				/* Regresa a proceso getty*/
		}else if(strcmp(command,"shutdown") == 0)		/* Comparacion con comando SHUTDOWN*/
		{												/* Se elimina todo los procesos*/
			execl("/usr/bin/killall","killall","./init","./getty","./sh","/usr/bin/xterm",NULL);
		}else
		{
			argv[0] = command;				/* Primer string de argumentos es el comando*/
			if(fork() == 0)					
				execvp(command,argv);		/* Se ejecuta lo pedido por el comando*/
			else if(strcmp(temp2, amp))		/* Si en los argumentos existe &, entonces no espera*/
				wait(NULL);					/* Solo se espera en primer plano*/
		}

		argv[0] = NULL;						/* Vaciado de primer string de argumentos*/
		argv[1] = NULL;						/* Vaciado de segundo string de argumentos*/
		memset(temp1,0,strlen(temp1));		/* Vaciado de variable temporal 1*/
		memset(temp2,0,strlen(temp2));		/* Vaciado de variable temporal 2*/
		memset(command,0,strlen(command));	/* Vaciado de variale comando */
	}
	return 0;
}