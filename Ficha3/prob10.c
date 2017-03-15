// PROGRAMA p10.c 

#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <termios.h> 
#include <fcntl.h>
#include <errno.h>

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
   int childpid, status;
   pid_t pid; 
   if (argc > 3 || argc < 2) { 
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

     if(argc == 3) {
  	int fd = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644);	
	dup2(fd,STDOUT_FILENO);
     }

        char *const arg[]={"/bin/ls", "-laR",argv[1],0};
	printf("1\n");
        execve("/bin/ls",arg,envp);
     	printf("Command not executed !\n"); 
     	exit(1); 

   }
   exit(0); 
} 
