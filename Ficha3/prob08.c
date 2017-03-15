// PROGRAMA p8.c 

#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 

int main(int argc, char *argv[], char *envp[]) 
{ 
  
   pid_t pid; 
   if (argc != 2) { 
     printf("usage: %s dirname\n",argv[0]); 
     exit(1); 
   } 
   pid=fork(); 
   if (pid > 0) 
     printf("My child is going to execute command\"ls -laR %s\"\n", argv[1]); 
   else if (pid == 0){
     /*execvp
     char *arg[]={"ls", "-laR",argv[1],NULL};  
     execvp("ls",arg);*/

     /*execlp 
     execlp("ls","ls", "-laR",argv[1],NULL);*/

     /*execv 
     char *const arg[]={"/bin/ls", "-laR",argv[1],0};
     execv("/bin/ls",arg);*/

     /*execl
     execl("/home/luissaraiva/Desktop/SOPE/prob03/ls","ls", "-laR",argv[1],NULL);*/

     //execve
     char *const arg[]={"/bin/ls", "-laR",argv[1],0};
     execve("/bin/ls",arg,envp);

     printf("Command not executed !\n"); 
     exit(1); 
   }
   exit(0); 
} 
