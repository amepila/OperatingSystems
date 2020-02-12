#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LENGTH_TEXT 30
#define ASCII_COLON	58

void getText(char *user1, char *user2)
{
	FILE *ptr_file;
	int counter = 0;
	char *lineptr = NULL;
	size_t length = 0;

	ptr_file = fopen("passwd.txt","r");

	if(ptr_file == NULL)
		printf("CORRUPTED FILE\n");

	while((getline(&lineptr, &length, ptr_file) != -1) || (counter < 2))
	{
		switch (counter)
		{
		case 0:
			strcpy(user1,lineptr);
			break;
		case 1:
			strcpy(user2,lineptr);
			break;
		default:
			break;
		}
		counter++;
	}
	fclose(ptr_file);
}

void getInfo(char *data_user, char *name, char *password)
{
	int counter1 = 0;
	int counter2 = 0;

	while(data_user[counter1] != ASCII_COLON)
	{
		name[counter1] = data_user[counter1];
		counter1++;
	}

	counter1++;
	while(data_user[counter1] != '\n')
	{
		password[counter2] = data_user[counter1];
		counter2++;
		counter1++;
	}
}

int name_authentication(char *user_name, char *name1, char *name2)
{
	if(strcmp(user_name, name1) == 0)
		return 1;
	else if(strcmp(user_name, name2) == 0)
		return 2;
	else
		return 0;
}

int password_authentication(char *user_passwd, char *passwd1, char *passwd2, int num_name)
{
	switch(num_name)
	{
	case 1:
		if(strcmp(user_passwd, passwd1) == 0)
			return 1;
		else
			return 0;
		break;
	case 2:
		if(strcmp(user_passwd, passwd2) == 0)
			return 1;
		else
			return 0;
		break;
	default:
		return 0;
		break;
	}
}

int main(void)
{
	int user;
	int success = 0;

	char temp[LENGTH_TEXT] = {0};
	char user_name[LENGTH_TEXT] = {0};
	char user_password[LENGTH_TEXT] = {0};

	char real_name1[LENGTH_TEXT] = {0};
	char real_password1[LENGTH_TEXT] = {0};

	char real_name2[LENGTH_TEXT] = {0};
	char real_password2[LENGTH_TEXT] = {0};

	char data_user1[2*LENGTH_TEXT];
	char data_user2[2*LENGTH_TEXT];

	while(success == 0)
	{
		printf("USERNAME: ");
		fgets(temp,LENGTH_TEXT,stdin);
		memcpy(user_name,temp,strlen(temp)-1);

		printf("PASSWORD: ");
		fgets(temp,LENGTH_TEXT,stdin);
		memcpy(user_password,temp,strlen(temp)-1);

		getText(data_user1,data_user2);

		getInfo(data_user1,real_name1,real_password1);
		getInfo(data_user2,real_name2,real_password2);

		user = name_authentication(user_name,real_name1,real_name2);
		success = password_authentication(user_password,real_password1,real_password2,user);

		memset(temp,0,strlen(temp));
		memset(user_name,0,strlen(user_name));
		memset(user_password,0,strlen(user_password));
		memset(real_name1,0,strlen(real_name1));
		memset(real_password1,0,strlen(real_password1));
		memset(real_name2,0,strlen(real_name2));
		memset(real_password2,0,strlen(real_password2));
	}

	if(!fork())
		exelcp("./sh","sh",(char*)NULL);
	
	return 0;
}