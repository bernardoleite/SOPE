
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


int convertDecimalToOctal(int decimalNumber)
{
    int octalNumber = 0, i = 1;

    while (decimalNumber != 0)
    {
        octalNumber += (decimalNumber % 8) * i;
        decimalNumber /= 8;
        i *= 10;
    }

    return octalNumber;
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void sigint_handler(int signo) 
{ 
  
  char c;
  printf("ARE YOU SURE? (Y/N)\n"); 
  scanf("%c",&c);
  if(c == 'Y' || c == 'y') {
	kill(-getpgrp(),SIGUSR1);
  }
	 
} 

void scan_dir(char* c, int argc, char *argv[]);

void checkfordir(char* c, int* pid, int argc, char *argv[]) {
 DIR *d;
  d = opendir(c);
  struct dirent *dir;
  int b = 0;

  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      char  v[250];
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
	    if(strcmp(argv[3], "-type") == 0 && strcmp(argv[4], "d") == 0) {
			if(strcmp(argv[5], "-print") == 0)
				printf("%s\n",v);
	    }
	    pid[b] = fork();
    	    b++;
	    if(pid[b-1] <= 0) {
		 struct sigaction action; 
 		 action.sa_handler = SIG_IGN; 
		 sigemptyset(&action.sa_mask); 
		 action.sa_flags = 0; 
		 sigaction(SIGINT,&action,NULL);
		 scan_dir(v, argc, argv);
	    }
	 }
      }
    }
  }
  closedir(d);
  return;
}

void checkforfiles(char* c, int argc, char *argv[]) {
  DIR *d;
  d = opendir(c);
  struct dirent *dir;

  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {

      char  v[250];
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
	    if(strcmp(argv[3], "-name") == 0) {
	    	if(strcmp(dir->d_name, argv[4]) == 0) {
	   		if(strcmp(argv[5], "-print") == 0) 
		   		printf("%s\n",v);
	        	else if(strcmp(argv[5], "-delete") == 0) {
		   		int i = remove(v);
			}
			else if(strcmp(argv[5], "-exec") == 0) {
				char *arg[argc- 5];
				for(int i = 0; i < argc - 5 - 1; i++) {
					if(strcmp(argv[i + 6] , ";") == 0)
						break;
					arg[i] = argv[i + 6];
				} 
				arg[argc - 5 - 2] = NULL;

				if(strcmp(arg[argc - 5 - 3], "{}") == 0) {
					arg[argc - 5 - 3] = v;
				}
				execvp(argv[6],arg);
			}
		}
	    }
	    else  if(strcmp(argv[3], "-type") == 0 && strcmp(argv[4], "f") == 0) {
		if(s.st_mode & S_IFREG) {
			if(strcmp(argv[5], "-print") == 0)
				printf("%s\n",v);
		}
	    }
	    else  if(strcmp(argv[3], "-type") == 0 && strcmp(argv[4], "l") == 0) {
		if(!(s.st_mode & S_IFREG)) {
			if(strcmp(argv[5], "-print") == 0)
				printf("%s\n",v);
		}
	    }
	    else  if(strcmp(argv[3], "-perm") == 0) {
		struct stat statRes;
		if(stat(v, &statRes) < 0)return;
		int statchmod = statRes.st_mode & (0000 | S_IRWXO | S_IRWXU | S_IRWXG);
		int octalnumb = convertDecimalToOctal(statchmod);
		char* aux = itoa(octalnumb,aux);
		if(strcmp(argv[4], aux) == 0 ){
			if(strcmp(argv[5], "-print") == 0)
				printf("%s\n",v);
		}
	    }
	 }
      }
    }
  }
  closedir(d);
}

void scan_dir(char* c, int argc, char *argv[]) {
  int pid[100], status;
  checkfordir(c,pid, argc, argv);
  sleep(30);
  checkforfiles(c, argc, argv);
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
  if(strcmp(argv[1], "sfind") != 0) {
	printf("COMMAND DOESN'T EXIST\n");
	exit(1);
  }


  scan_dir("/home/jsaraiva", argc, argv);

  return(0);
}
