// PROGRAMA p9.c 

#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 

void pr_exit(int status)
{
    if (WIFEXITED(status))
      printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status)) {
      printf("abnormal termination, signal number = %d%s\n",WTERMSIG(status), 
		#ifdef WCOREDUMP
                  WCOREDUMP(status) ? " (core file generated)" : "");
		#else
		  "");		
		#endif
    }
    else if (WIFSTOPPED(status))
      printf("child stopped, signal number = %d\n",WSTOPSIG(status));
}

int main(int argc, char *argv[], char *envp[]) 
{ 
   pid_t pid; 
   int childpid, status;

   if (argc != 2) { 
     printf("usage: %s dirname\n",argv[0]); 
     exit(1); 
   } 
   pid=fork(); 
   if (pid > 0) {
     printf("My child is going to execute command\"ls -laR %s\"\n", argv[1]);

     childpid = waitpid(pid,&status,WNOHANG);  

     pr_exit(status); 
   } 
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
