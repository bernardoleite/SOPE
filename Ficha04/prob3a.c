// PROGRAMA p02a.c 
#include <stdio.h> 
#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <time.h>
#include <sys/types.h> 

int sentidoPos = 1;
void sig_usr(int signo) 
{ 
  if (signo == SIGUSR1)
    sentidoPos = 1;
  if (signo == SIGUSR2)
    sentidoPos = 0;
} 


int main(void) 	
{ 
 int v = 0;
 int i = 0;
 pid_t pid, childpid;
 int status;
 struct sigaction action; 
 action.sa_handler = sig_usr; 
 sigemptyset(&action.sa_mask); 
 action.sa_flags = 0; 
 time_t t;
 srand((unsigned) time(&t));
 if (sigaction(SIGUSR1,&action,NULL) < 0) 
 { 
       fprintf(stderr,"Unable to install SIGUSR1 handler\n"); 
       exit(1); 
 }
 if (sigaction(SIGUSR2,&action,NULL) < 0) 
 { 
       fprintf(stderr,"Unable to install SIGUSR2 handler\n"); 
       exit(1); 
 } 
 pid = fork();
 if(pid == 0) {
 while(1) {
   sleep(3);
   kill(pid, rand()%2 ? SIGUSR1 : SIGUSR2); 
   childpid = waitpid(pid,&status,WNOHANG);
   if(status == 32)
     exit(0);      
}
}
 else {
 for(i = 0; i < 50; i++){
   printf("%d\n",v);
   sleep(1);
   if(sentidoPos != 0)
     v++;
   else
     v--;
  }
  exit(32);	
 } 
} 
