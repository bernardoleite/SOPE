#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char* command = malloc(sizeof(char)*100);
	
	fgets(command,100,stdin);
	
	const char* s=" ";
	
	char* token;
	
	token=strtok(command,s);
	
	while(token!=NULL)
	{
		printf("%s\n",token);
		
		token=strtok(NULL,s);
	}
	
	return 0;
}
