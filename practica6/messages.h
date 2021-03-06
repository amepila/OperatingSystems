#ifndef MESSAGES_H_
#define MESSAGES_H_

/**
  \archivo		messages.h
  \descripcion	Este archivo contiene las funciones
  				de mensajes, donde solo se maneja 1 dato
  				de mensajes ya que no importa el contenido
  				sino la senal para moverse
  \fecha   		29/Mar/20
 */

/* Librerias*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

/* Tipo de dato de mensaje*/
typedef struct 
{
    long mtype;   		/* Tipo de mensaje, debe ser mayor > 0 */
    char mtext[30];		/* Datos del mensajes*/
} Msgbuf_t;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que representa la recepcion de mensajes
  \entradas  	int idmsg, Msgbuf_t *msg
  \return     	void
 */
void msgreceive(int idmsg, Msgbuf_t *msg);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que representa el envio de mensajes
  \entradas  	int idmsg, Msgbuf_t *msg
  \return     	void
 */
void msgsend(int idmsg, Msgbuf_t *msg);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que crea el buzon de mensajes
  \entradas  	int key
  \return     	int
 */
int initmsg(int key);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
  \descripcion	Funcion que elimina el buzon de mensajes
  \entradas  	int idmsg
  \return     	void
 */
void erasemsg(int idmsg);

#endif /*MESSAGES_H_*/