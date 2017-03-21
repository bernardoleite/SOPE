#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

int v = 1;
int growing = 1;

void sigusr(int signo)
{
	if (signo == SIGUSR1)
	growing = 1;
	else
	growing = 0;
}

int main()
{
	srand(time(NULL));

	struct sigaction action;
	action.sa_handler = sigusr;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	if (sigaction(SIGUSR1, &action, NULL) < 0) {
		fprintf(stderr, "Signal handler installation has failed...\n");
		exit(1);
	}

	action.sa_handler = sigusr;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	if (sigaction(SIGUSR2, &action, NULL) < 0) {
		fprintf(stderr, "Signal handler installation has failed...\n");
		exit(1);
	}

	int pid=fork();
	int status;
	
	if (pid == 0) {
		for (int i = 0; i < 50; i++){
			pause();
			printf("%d ", v);
			fflush(stdout);
			if(growing? v++ : v--)
		}
	}
	else {
		for ( ; ; )  {
			sleep(1);
			if ((random() % 2) == 1)
			kill(pid, SIGUSR1);
			else
			kill(pid, SIGUSR2);
			
			if((waitpid(-1, &status, WNOHANG))>0)
			exit(0);
		}
	}
}