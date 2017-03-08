#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


//filho Hello
//pai World!
int main(void){
	
	pid_t pid, pidS;
	int status;
	pid=fork();


	if(pid > 0){//parent
		pidS = wait(&status);
		printf("Hello\n");
	}
	else{
		
		printf("world!\n");
	}

	return 0;
}
	
