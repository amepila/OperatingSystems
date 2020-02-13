/**
  \archivo		getty.c   
  \descripcion	Este archivo representa al proceso getty
  				el cual valida el usuario y contrasenas
  				con un documento passwd.txt, con la finalidad
  				de iniciar con el proceso sh
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
#define LENGTH_TEXT 30	/* Longitud de los strings		*/
#define ASCII_COLON	58	/* Codigo ASCII del simbolo ':' */


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que extrae el usuario y contrasena del documento passwd.txt
  				de dos usuarios en strings por referencia
  \entradas  	char *user1, char *user2
  \return     	void
 */
void getText(char *user1, char *user2)
{
	FILE *ptr_file;			/* Puntero del archivo									*/
	int counter = 0;		/* Contador de lineas de texto en el archivo			*/
	char *lineptr = NULL;	/* Puntero que guarda la primera posicion de lo leido	*/
	size_t length = 0;		/* Variable de longitud de la lineas 					*/

	ptr_file = fopen("passwd.txt","r");	/* Apertura del archivo en modo lectura		*/

	if(ptr_file == NULL)	/* Si no se detecta un archivo esta corrupto			*/
		printf("CORRUPTED FILE\n");

	/* Obtencion de los datos del archivo linea por linea, en este caso solo 2 lineas*/
	while((getline(&lineptr, &length, ptr_file) != -1) || (counter < 2))
	{
		/* Se extrae los datos de las lineas y se guardan en el usuario correspondiente*/
		switch (counter)
		{
		case 0:
			strcpy(user1,lineptr);	/* Extraccion de primera linea para datos de usuario 1*/
			break;
		case 1:
			strcpy(user2,lineptr);	/* Extraccion de segunda linea para datos de usuario 2*/ 
			break;
		default:
			break;
		}
		counter++;		/* Contador que avanza de linea*/
	}
	fclose(ptr_file);	/* Se cierra el archivo al ya no utilizarlo mas*/
}


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que separa el nombre de usuario y la contrasena en respectivas
  				variables, ya que recibe los datos de usuario de manera concatenada
  \entradas  	char *data_user, char *name, char *password
  \return     	void
 */
void getInfo(char *data_user, char *name, char *password)
{
	int counter1 = 0;	/* Contador del nombre de usuarios 		*/
	int counter2 = 0;	/* Contador de la contrasena de usuarios*/

	/* Extraccion del nombre de usuario hasta encontrar el simbolo ':' */
	while(data_user[counter1] != ASCII_COLON)
	{
		name[counter1] = data_user[counter1];	/* Recoleccion del nombre de usuario*/
		counter1++;								/* Aumento del contador de nombre	*/
	}

	counter1++;	/* Aumento del contador de nombre para evitar extraer el simbolo ':'*/
	
	/* Extraccion de la contrasena de usuario hasta encontrar el salto de linea 	
		donde el contador de nombre se vuelve un contador general que continua
		leyendo lo que sigue del simbolo ':', de manera que el contador de contrasena
		inicia en cero para realizar una recoleccion en el arreglo de manera coherente
	*/
	while(data_user[counter1] != '\n')
	{
		password[counter2] = data_user[counter1];	/* Recoleccion de contrasena 	 	*/
		counter2++;									/* Aumento de contador de contrasena*/
		counter1++;									/* Aumento de contador de nombre 	*/
	}
}


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que valida el nombre de usuario cpn alguno de los nombres
  				de usuario recolectado del archivo
  \entradas  	char *user_name, char *name1, char *name2
  \return     	1 para usuario 1, 2 para usuario 2 y 0 para indicar que no es ninguno
 */
int name_authentication(char *user_name, char *name1, char *name2)
{
	if(strcmp(user_name, name1) == 0)		/* Comparacion con nombre de usuario 1	*/
		return 1;							/* Retorno de entero 1 para usuario 1	*/
	else if(strcmp(user_name, name2) == 0)	/* Comparacion con nombre de usuario 2	*/
		return 2;							/* Retorno de entero 2 para usuario 2	*/
	else
		return 0;							/* Si no se parece a ninguno, regresa 0 */
}


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que valida la contrasena de usuario respecto al usuario
  				ingresado anteriormente
  \entradas  	char *user_passwd, char *passwd1, char *passwd2, int num_name
  \return     	1 para exito en la autentificacion y 0 por una contrasena incorrecta
 */
int password_authentication(char *user_passwd, char *passwd1, char *passwd2, int num_name)
{
	switch(num_name)	/* Depende del usuario se compara con su respectiva contrasena 	*/
	{
	case 1:
		if(strcmp(user_passwd, passwd1) == 0)
			return 1;	/* Retorno de entero 1 de caso exitoso con el usuario 1			*/
		else
			return 0;	/* Retorno de entero 0 de caso incorrecto con el usuario 1  	*/
		break;
	case 2:
		if(strcmp(user_passwd, passwd2) == 0)
			return 1;	/* Retorno de entero 1 de caso exitoso con el usuario 2 		*/
		else
			return 0;	/* Retorno de entero 0 de caso incorrecto con el usuario 2 		*/
		break;
	default:
		return 0;		/* Retorno de entero 0 al no corresponder a ningun usuario 		*/
		break;
	}
}


int main(void)
{
	int user;			/* Variable que guarda el numero de usuario sea 1 o 2		*/
	int success = 0;	/* Variable que indica que la autentificacion fue exitosa 	*/

	char temp[LENGTH_TEXT] = {0};			/* String temporal para Recoleccion 	*/
	char user_name[LENGTH_TEXT] = {0};		/* Nombre de usuario tecleado 			*/
	char user_password[LENGTH_TEXT] = {0};	/* Contrasena de usuario tecleado 		*/

	char real_name1[LENGTH_TEXT] = {0};		/* Nombre verdadero de usuario 1		*/
	char real_password1[LENGTH_TEXT] = {0}; /* Contrasena verdadera de usuario 1	*/

	char real_name2[LENGTH_TEXT] = {0};		/* Nombre verdadero de usuario 2		*/
	char real_password2[LENGTH_TEXT] = {0};	/* Contrasena verdadera de usuario 2 	*/

	char data_user1[2*LENGTH_TEXT];			/* Datos recolectados de usuario 1 		*/
	char data_user2[2*LENGTH_TEXT];			/* Datos recolectados de usuario 2 		*/

	/* Mientras no se autentifique el usuario no se ejecuta el shell*/
	while(success == 0)	
	{
		printf("USERNAME: ");
		fgets(temp,LENGTH_TEXT,stdin);				/* Entrada de nombre por parte de usuario*/
		memcpy(user_name,temp,strlen(temp)-1);		/* Vaciado de dato para eliminar caracter nulo*/


		printf("PASSWORD: ");
		fgets(temp,LENGTH_TEXT,stdin);				/* Entrada de contrasena por parte de usuario*/
		memcpy(user_password,temp,strlen(temp)-1);	/* Vaciado de dato para eliminar caracter nulo*/	

		/* Obtencion de datos de usuarios del archivo de texto*/
		getText(data_user1,data_user2);		

		/* Separacion de nombre y contrasena en respectivos campos de usuarios	*/
		getInfo(data_user1,real_name1,real_password1);
		getInfo(data_user2,real_name2,real_password2);	

		/* Autentificacion de nombre y contrasena de usuarios*/
		user = name_authentication(user_name,real_name1,real_name2);
		success = password_authentication(user_password,real_password1,real_password2,user);

		/* Vaciado de las variables utilizadas durante el proceso*/
		memset(temp,0,strlen(temp));
		memset(user_name,0,strlen(user_name));
		memset(user_password,0,strlen(user_password));
		memset(real_name1,0,strlen(real_name1));
		memset(real_password1,0,strlen(real_password1));
		memset(real_name2,0,strlen(real_name2));
		memset(real_password2,0,strlen(real_password2));
	}

	/* Si la autentificacion es exitosa, se ejecuta el shell*/
	if(fork() == 0)
		execlp("./sh","sh",NULL);
	else 
		wait(NULL);	/* Se espera a que se ejecute el shell	*/
	return 0;
}