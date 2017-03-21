// PROGRAMA p04a.c
#include<stdlib.h>
#include<stdio.h>
#include<wait.h>
#include<unistd.h>

void sig_ign_handler(int signo){
	pid_t pid;
	int status;
	
	pid=waitpid(-1,&status,WNOHANG);
	if (pid != -1)
	printf("PARENT: child with PID=%d terminated with exit code %d\n",pid,WEXITSTATUS(status)); 
}

int main(void)
{
	pid_t pid;
	int i, n;
	for (i = 1; i <= 10; i++) {
		pid = fork();
		if (pid == 0) {
			printf("CHILD no. %d (PID=%d) working ... \n", i, getpid());
			sleep(15); // child working ...
			printf("CHILD no. %d (PID=%d) exiting ... \n", i, getpid());
			exit(0);
		}
	}

	signal(SIGCHLD,sig_ign_handler);
	for (i = 1; i <= 4; i++) {
		printf("PARENT: working hard (task no. %d) ...\n", i);
		n = 20;
		while ((n = sleep(n)) != 0)   ;
		printf("PARENT: end of task no. %d\n", i);
		printf("PARENT: waiting for child no. %d ...\n", i);
	}
	exit(0);
}