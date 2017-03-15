//Folha 3 p11.c 
//PROBLEMMMMMMMMMMMM
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <termios.h> 
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[]) { 
  int pid, childpid, status; 
  char* arg[argc];
  char* arg2[argc - 2];

  pid=fork(); 
  if(pid > 0) { 
     childpid = waitpid(pid,&status,WNOHANG);  
  }
  else
  { 
    if(argv[-2] != "-o") {
	for(int i = 0; i < argc; i++){
	  arg[i] = argv[i];
    	}
    }
    else {
  	int fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_EXCL, 0644);	
	dup2(fd,STDOUT_FILENO);
	for(int i = 0; i < argc - 2; i++){
	  arg2[i] = argv[i];
    	}
    }

    system(arg); 
//NOTE: system() "expands" s*.h 
    // try also system("cat s01.c > s01_copy.c"); 

//WHY NOT FAILED, in this case, like in exec() 
    exit(0); 
  } 

  return 0; 
}
