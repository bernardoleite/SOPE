// PROGRAMA p4.c------------------------------------------------------------------- 

#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <stdlib.h>

int main(void) { 
  int pid, status, childpid; 
   write(STDOUT_FILENO,"Hello ", 6);
  pid = fork();
  if(pid != 0) { 
    childpid = wait(&status);

  } else { 
    write(STDOUT_FILENO,"world!\n", 8);
    exit(32); 
  } 
  exit(0); 
} 
