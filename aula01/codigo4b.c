#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[ ] )
{
   int num = atoi(argv[2]); //Importante revisão de atoi

   for (int i = 0 ; i < num; i++)
   		printf("Hello %s\n", argv[1]);
    
    return 0;
}