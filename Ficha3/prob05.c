// PROGRAMA p4.c------------------------------------------------------------------- 

#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <stdlib.h>

int main(void) { 
  int pid, status, childpid, childchildpid, statuschild; 
   write(STDOUT_FILENO,"Hello ", 6);
  pid = fork();
  if(pid != 0) { 
    childpid = wait(&status);

  } else { 
    childpid = fork();
    if(childpid != 0) { 
      write(STDOUT_FILENO,"my ", 3);
      childchildpid = wait(&statuschild);
      exit(32);
    } else {
      write(STDOUT_FILENO,"friends!\n", 10);
      exit(64); 
    }
  } 
  exit(0); 
} 
