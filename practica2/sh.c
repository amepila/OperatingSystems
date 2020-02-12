#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define LENGTH_TEXT 30

int main(void)
{
	char *argv[2] = {NULL,NULL};
	char command[LENGTH_TEXT] = {0};
	char temp[LENGTH_TEXT] = {0};

	for(;;)
	{
		printf("sh > ");
		fgets(temp,LENGTH_TEXT,stdin);
		memcpy(command,temp,strlen(temp)-1);

		if(strcmp(command,"exit") == 0)
		{
			execlp("./getty","getty",(char*)NULL);
		}else if(strcmp(command,"shutdown") == 0)
		{
			execl("/usr/bin/killall","killall","./init,","./getty","./sh",(char *)NULL);
		}else
		{
			argv[0] = command;
			if(fork() == 0)
				
				execvp(command,argv);
			else
				wait(NULL);
		}
		memset(temp,0,strlen(temp));
		memset(command,0,strlen(command));
	}
	return 0;
}