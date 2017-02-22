#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* subcommand(char* command)
{
	if(strchr(command,';')!=NULL)
		return ";";
	else
		if(strchr(command,'|')!=NULL)
			return "|";
		else
			return " ";
}

int main()
{
	char* command = malloc(sizeof(char)*100);
	
	fgets(command,100,stdin);
	
	char s[2];
	
	strcpy(s,subcommand(command));
					
	char* subcommands[10];
	
	char* tokens[10];
	
	char* token;
	
	token=strtok(command,s);
		
	subcommands[0]=token;
	
	int i=1;
	
	while(token!=NULL)
	{
		token=strtok(NULL,s);
		subcommands[i]=token;
		i++;
	}
	
	if(strcmp(s," ")==0)
	{
		i=0;
		
		while(subcommands[i]!=NULL)
		{
			printf("%s\n",subcommands[i]);
			i++;
		}
		
		return 0;
	}
	
	strcpy(s," ");
	
	i=0;
	
	int n=0;
	
	while(subcommands[i]!=NULL)
	{
		token=strtok(subcommands[i],s);
		
		tokens[n]=token;
		n++;
		
		while(token!=NULL)
		{
			token=strtok(NULL,s);
			tokens[n]=token;
			n++;
		}
		
		i++;
	}
	
	i=0;
	
	while(i<n)
	{
		printf("%s\n",tokens[i]);
		i++;
	}
	
	return 0;
}