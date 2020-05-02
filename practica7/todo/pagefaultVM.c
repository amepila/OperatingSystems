/**
  \archivo      pagefaultVM.c  
  \descripcion  Archivo fuente para el intercambio
                memoria fisica y virtual
  \fecha        1/May/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mmu.h>

#define NUMPROCS 4
#define PAGESIZE 4096
#define PHISICALMEMORY 12*PAGESIZE 
#define TOTFRAMES PHISICALMEMORY/PAGESIZE
#define RESIDENTSETSIZE PHISICALMEMORY/(PAGESIZE*NUMPROCS)

extern char *base;
extern int framesbegin;
extern int idproc;
extern int systemframetablesize;
extern int ptlr;

extern struct SYSTEMFRAMETABLE *systemframetable;
extern struct PROCESSPAGETABLE *ptbr;

extern copyframe(int sframe,int dframe);
extern int writeblock(char *buffer, int dblock);
extern int readblock(char *buffer, int sblock);
extern int loadframe(int frame);
extern int saveframe(int frame);

extern int getfreeframe();
int searchvirtualframe();
int getfifo();

int pagefault(char *vaddress)
{
    int i;
    int frame,vframe; 
    long pag_a_expulsar;
    int fd;
    char buffer[PAGESIZE];
    int pag_del_proceso;

    // A partir de la dirección que provocó el fallo, calculamos la página
    pag_del_proceso=(long) vaddress>>12;

    // Si la página del proceso está en un marco virtual del disco
    if((ptbr + pag_del_proceso)->presente == 0 && (ptbr + pag_del_proceso)->framenumber != -1)
    {
		// Lee el marco virtual al buffer
        vframe = (ptbr + pag_del_proceso)->framenumber;
        readblock(buffer,vframe);
        // Libera el frame virtual
        systemframetable[vframe].assigned = 0;
    }

    // Cuenta los marcos asignados al proceso
    i = countframesassigned();

    // Si ya ocupó todos sus marcos, expulsa una página
    if(i >= RESIDENTSETSIZE)
    {
		// Buscar una página a expulsar
        pag_a_expulsar = getfifo();
		
		// Poner el bit de presente en 0 en la tabla de páginas
        (ptbr + pag_del_proceso)->presente = 0;

        // Si la página ya fue modificada, grábala en disco
        if((ptbr + pag_a_expulsar)->modificado == 1)
        {
			// Escribe el frame de la página en el archivo de respaldo y pon en 0 el bit de modificado
            frame = (ptbr + pag_a_expulsar)->framenumber;
            saveframe(frame);
            (ptbr + pag_a_expulsar)->modificado = 0;
        }
		
        // Busca un frame virtual en memoria secundaria
        vframe = searchvirtualframe();
		// Si no hay frames virtuales en memoria secundaria regresa error
        if(vframe == -1)
            return(-1);

        // Copia el frame a memoria secundaria, actualiza la tabla de páginas y libera el marco de la memoria principal
        copyframe(frame,vframe);
        (ptbr + pag_a_expulsar)->framenumber = vframe;
        (ptbr + pag_a_expulsar)->presente = 0;
        systemframetable[frame].assigned = 0;
        systemframetable[vframe].assigned =1;
    }

    // Busca un marco físico libre en el sistema
    frame = getfreeframe();

	// Si no hay marcos físicos libres en el sistema regresa error
    if(frame == -1)
        return(-1); // Regresar indicando error de memoria insuficiente

    // Si la página estaba en memoria secundaria
    if(vframe != -1)
    {
        // Cópialo al frame libre encontrado en memoria principal y transfiérelo a la memoria física
        copyframe(vframe,frame);
    }
   
	// Poner el bit de presente en 1 en la tabla de páginas y el frame 
    (ptbr + pag_del_proceso)->presente = 1;
    (ptbr + pag_del_proceso)->framenumber = frame;

    return(1); // Regresar todo bien
}

int searchvirtualframe()
{
    int counter;
    int found = -1;
    int frametable = systemframetable + framesbegin;

    // Se verifica toda la memoria virtual en busca de un marco virtual disponible
    for(counter = frametable; counter < 2*frametable; counter++)
    {
        // Se captura el primer marco disponible y se sale del ciclo sino se obtiene -1
        if(systemframetable[counter] == 0)
        {   
            systemframetable[counter] = 1;  // Se aparta ese marco virtual
            found = counter;  
            break;
        } 
    }
    return found;   // Regresa el numero de marco sino regresa -1, por no encontrar
}

int getfifo()
{
    int counter;
    int counter_presentes = 0;
    int old;

    //Se busca la pagina con mas tiempo en memoria fisica
    for(counter = 0; counter < ptlr; counter++)
    {
        //Se busca las paginas con mas tiempo con su bit presente
        if((ptlr + counter)->presente == 1)
        {
            if(counter_presentes >= 1)
            {
                // Si la pagina actual tiene mas tiempo se vuelve el mas antiguo
                if((ptlr + old)->tarrived < (ptlr + (counter))->tarrived)
                    old = counter;
            }
            else
            {
                old = counter;      //Guarda el primero en memoria fisica como referencia
            }
            counter_presentes++;    //Este contador es unicamente de referencia 
        }
    }
    return old;
}
