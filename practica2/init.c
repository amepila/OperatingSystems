#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	NUMBER_CHILDS	6

int main(void)
{
	int counter = 0;
	int status_child;

	for(;;)
	{
		do
		{
			status_child = fork();

			if (status_child < 0)
				printf("ERROR\n");
			else (status_child == 0)
			{
				exelc("/usr/bin/xterm","xterm","-hold","-e","./getty","getty",(char*)NULL);
				counter += 1;
			}
		} while (counter < NUMBER_CHILDS)

	};
	return 0;
};
