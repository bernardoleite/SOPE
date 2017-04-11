
#include <dirent.h> 
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <string.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <wait.h>
#include <stdlib.h>


void scan_dir(char* c);

void checkfordir(char* c, int* pid) {
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
	    printf("FORK\n");
	    pid[b] = fork();
    	    b++;
	    scan_dir(v);
	 }
      }
    }
  }
  return;
}

void checkforfiles(char* c) {
 DIR *d;
  d = opendir(c);
  struct dirent *dir;

      printf("%s\n",c);

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
	    printf("FILE\n");
	    printf("%s\n",dir->d_name);
	 }
      }
    }
  }

}

void scan_dir(char* c) {
  int pid[100], status;
  checkfordir(c,pid);
  checkforfiles(c);
  for (int i = 0; i < 100; i++) {
    if(pid[i] > 0) {
	waitpid(pid[i],&status,WNOHANG);
    }
    else {
	printf("EXITING\n");
	exit(0);
    }
  }
  return;
}


int main(void)
{

  scan_dir("/home/jsaraiva/Desktop");

  return(0);
}
