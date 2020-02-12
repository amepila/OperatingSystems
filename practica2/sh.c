#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define LENGTH_TEXT 30
#define ASCII_SPACE	32

int main(void)
{
	char *argv[3] = {NULL,NULL,NULL};
	char command[LENGTH_TEXT] = {0};
	char temp1[LENGTH_TEXT] = {0};
	char temp2[LENGTH_TEXT] = {0};

	int counter1;
	int counter2;
	int len_cmd;
	int flag_space;

	for(;;)
	{
		printf("sh > ");
		fgets(temp1,LENGTH_TEXT,stdin);
		memcpy(command,temp1,strlen(temp1)-1);
		memset(temp1,0,strlen(temp1));

		len_cmd = strlen(command);
		flag_space = 0;

		for(counter1 = 0; counter1 <= len_cmd; counter1++)
		{
			if(command[counter1] == ASCII_SPACE)
				flag_space = 1;
		}

		if(flag_space)
		{
			for(counter1 = 0; command[counter1] != ASCII_SPACE; counter1++)
				temp1[counter1] = command[counter1];
			counter1++;
			counter2 = 0;

			for(;counter1 <= len_cmd; counter1++)
			{
				temp2[counter2] = command[counter1];
				counter2++;
			}
			memset(command,0,strlen(command));
			memcpy(command,temp1,strlen(temp1));
			argv[1] = temp2;

			printf("%s\n", command);
			printf("%s\n", argv[1]);
		}

		if(strcmp(command,"exit") == 0)
		{
			execlp("./getty","getty",NULL);
		}else if(strcmp(command,"shutdown") == 0)
		{
			execl("/usr/bin/killall","killall","./init","./getty","./sh","/usr/bin/xterm",NULL);
		}else
		{
			argv[0] = command;
			if(fork() == 0)
				
				execvp(command,argv);
			else
				wait(NULL);
		}
		
		argv[0] = NULL;
		argv[1] = NULL;
		memset(temp1,0,strlen(temp1));
		memset(temp2,0,strlen(temp2));
		memset(command,0,strlen(command));
	}
	return 0;
}