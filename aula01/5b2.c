#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char** environ;

int main()
{
	int i=0;
	char* user=malloc(sizeof(char)*100);
	
	while(environ[i]!=NULL)
	{
		if (strncmp("USER", environ[i], 4)==0)
			strcpy(user,&environ[i][5]);
		
		i++;	
	}
	
		printf("Hello %s!\n",user);

	return 0;
}
