
#include <dirent.h> 
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <string.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <wait.h>
#include <stdlib.h>
#include <signal.h> 


void sigint_handler(int signo) 
{ 
  
  char c;
  printf("ARE YOU SURE? (Y/N)\n"); 
  scanf("%c",&c);
  if(c == 'Y' || c == 'y') {
	kill(-getpgrp(),SIGUSR1);
  }
	 
} 

void scan_dir(char* c, char* p, int l);

void checkfordir(char* c, int* pid, char * p, int l) {
 DIR *d;
  d = opendir(c);
  struct dirent *dir;
  int b = 0;

  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      char  v[100];
      strcpy(v,c);
      char  a[] = {"/"};
      strcat(v,a);
      if(dir->d_name[strlen(dir->d_name)-1] == '.')
	continue;
      strcat(v,dir->d_name);

      struct stat s;
      if( stat(v,&s) == 0 )
      {
         if( s.st_mode & S_IFDIR )
         {
            //it's a directory

	    pid[b] = fork();
    	    b++;
	    if(pid[b-1] <= 0) {
		 struct sigaction action; 
 		 action.sa_handler = SIG_IGN; 
		 sigemptyset(&action.sa_mask); 
		 action.sa_flags = 0; 
		 sigaction(SIGINT,&action,NULL);
		 scan_dir(v,p,l);
	    }
	 }
      }
    }
  }
  closedir(d);
  return;
}

void checkforfiles(char* c, char *p, int l) {
  DIR *d;
  d = opendir(c);
  struct dirent *dir;

  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {

      char  v[100];
      strcpy(v,c);
      char  a[] = {"/"};
      strcat(v,a);
      if(dir->d_name[strlen(dir->d_name)-1] == '.')
	continue;
      strcat(v,dir->d_name);
	

      struct stat s;
      if( stat(v,&s) == 0 )
      {
         if(!(s.st_mode & S_IFDIR) )
         {
            //it's a directory
	    if(strcmp(dir->d_name, p) == 0) {
	   	if(l == 1) 
		   printf("%s\n",v);
	        else if(l == 0) {
		   int i = remove(v);
		   if(i == 0)
			printf("%s\n",p);
		   else
			printf("COULDNt REMOVE\n");		
		}
	    }
	 }
      }
    }
  }
  closedir(d);
}

void scan_dir(char* c, char *p, int l) {
  int pid[100], status;
  checkfordir(c,pid,p,l);
  sleep(30);
  checkforfiles(c,p,l);
  for (int i = 0; i < 100; i++) {
    if(pid[i] > 0) {
	waitpid(pid[i],&status,WNOHANG);
    }
    else {
	exit(0);
    }
  }
  return;
}


int main(int argc, char *argv[])
{
 struct sigaction action; 
 action.sa_handler = sigint_handler; 
 sigemptyset(&action.sa_mask); 
 action.sa_flags = 0; 
 if (sigaction(SIGINT,&action,NULL) < 0) 
  { 
    fprintf(stderr,"Unable to install SIGINT handler\n"); 
    exit(1); 
  } 
if (strcmp(argv[1], "-name") == 0 && strcmp(argv[3], "-delete"))
  scan_dir("/home/jsaraiva/Desktop", argv[2],1);

else if (strcmp(argv[1], "-name") == 0 && strcmp(argv[3], "-print"))
  scan_dir("/home/jsaraiva/Desktop", argv[2],0);


  return(0);
}
