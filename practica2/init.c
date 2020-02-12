#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	NUMBER_CHILDS	1

int main(void)
{
	int counter = 0;

	while (counter < NUMBER_CHILDS);
	{

		if (fork() == 0)
		{
			execl("/usr/bin/xterm","xterm","-hold","-e","./getty","getty",(char*)NULL);
			counter++;
		}else
		{
			printf("error\n");

		}
	}

	return 0;
}
