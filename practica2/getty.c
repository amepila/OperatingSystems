#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LENGTH_TEXT	30

int main()
{
	FILE *ptr_file;
	char user_name[LENGTH_TEXT];
	char user_password[LENGTH_TEXT];

	printf("USERNAME: \t");
	fgets(user_name,LENGTH_TEXT,stdin);

	printf("PASSWORD: \t");
	fgets(user_password,LENGTH_TEXT,stdin);

	ptr_file = fopen("passwd.txt","r");

	if(ptr_file == NULL)
		printf("CORRUPTED FILE\n");





	return 0;
}