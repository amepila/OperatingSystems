/**
  \archivo		  messages.c 
  \descripcion	Este archivo contiene las funciones
  				      de mensajes, donde solo se maneja 1 dato
  				      de mensajes ya que no importa el contenido
  				      sino la senal para moverse
  \fecha   		  29/Mar/20
 */

/* Librerias*/
#include "messages.h"

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que representa la recepcion de mensajes
  \entradas  	int idmsg, Msgbuf_t *msg
  \return     	void
 */
void msgreceive(int idmsg, Msgbuf_t *msg)
{
	msgrcv(idmsg, msg, sizeof(Msgbuf_t),1,0);	/* Recepcion de mensaje*/
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que representa el envio de mensajes
  \entradas  	int idmsg, Msgbuf_t *msg
  \return     	void
 */
void msgsend(int idmsg, Msgbuf_t *msg)
{
  msg->mtype = 1;				        /* Priorida o tipo del mensaje*/
	strcpy(msg->mtext,"hola");	  /*Cualquier mensaje*/
	msgsnd(idmsg, msg, sizeof(Msgbuf_t), IPC_NOWAIT); /* NO espera a que sea recibido*/
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que crea el buzon de mensajes
  \entradas  	int key
  \return     	int
 */
int initmsg(int key)
{
	int msgid;			/* Cola de mensajes*/
	msgid = msgget(key,0666|IPC_CREAT);	/* Creacion de buzon para mensajes*/
	if(msgid == -1)		/* Verificacion de error al crear el buzon*/
		printf("ERROR DE CREACION DE BUZON\n");
	return msgid;
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que elimina el buzon de mensajes
  \entradas  	int idmsg
  \return     	void
 */
void erasemsg(int idmsg)
{
	msgctl(idmsg, IPC_RMID, NULL);	/* Se elimina el mensaje*/
}