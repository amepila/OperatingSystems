#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	NUMBER_CHILDS	1

int main(void)
{
	int counter = 0;
	int status_child;

	do
	{
		status_child = fork();

		if (status_child < 0)
		{
			printf("error\n");
		}else if(status_child == 0)
		{
			execl("/usr/bin/xterm","xterm","-hold","-e","./getty","getty",(char*)NULL);
			counter = counter + 1;
		}
	} while (counter < NUMBER_CHILDS);

	return 0;
}
