#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<limits.h>

pid_t pid;

void sigalrm(int signo){
	kill(pid,SIGKILL);
	printf("Time limit has passed!\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	int i=0;
	int status;
	int tempo=atoi(argv[1]);

	pid=fork();
	
	if(pid==0)
	{
		if((execvp(argv[2],&argv[2])==-1)){printf("Not executed!\n");}
	}
	else
	{
		alarm(tempo);
		signal(SIGALRM,sigalrm);
		while(pid!=waitpid(pid,&status,WNOHANG)){};
		
		printf("Normal termination!\n");
	}
	
	exit(0);
}