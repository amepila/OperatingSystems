#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define	NUMBER_CHILDS	2

int main(void)
{
	int counter = 0;

	for(;;)
	{
		while (counter < NUMBER_CHILDS)
		{
			if(fork() == 0)
				execl("/usr/bin/xterm","xterm","-hold","-e","./getty","getty",NULL);
			counter++;
		}
		wait(NULL);
		counter--;
	}
	return 0;
}
