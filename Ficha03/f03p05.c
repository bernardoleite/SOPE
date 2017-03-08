#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


//Hello my friends!

int main(void){
	
	pid_t pid, pidS, pidSS;

	int status;
	pid=fork();


	if(pid > 0){//parent
		pidSS = wait(&status);
		printf("friends!\n");
	}
	else{
		pidS=fork();
		if(pidS > 0){//1son
		pidS = wait(&status);
		printf("my\n");
		}
		else{
		
			printf("Hello\n");
		}
	}	
return 0;
}
	
