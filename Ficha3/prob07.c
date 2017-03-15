// PROGRAMA p7.c
 
#include <stdio.h>  
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 
int main(int argc, char *argv[]) 
{ 
   char prog[20]; 
   sprintf(prog,"%s.c",argv[1]); 
   execlp("gcc","gcc",prog,"-Wall","-o",argv[1],NULL); 
   printf("Compila ficheiro %s\n",argv[1]); 
   exit(0); 
} 
