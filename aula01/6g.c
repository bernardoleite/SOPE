// PROGRAMA p6a.c

#include <stdio.h>
#include <stdlib.h> //Faltava esta biblioteca para o exit
#include <string.h> //para strerror
#include <errno.h> //para errno

#define BUF_LENGTH 256

int main(int argc, char *argv[ ])
{
	
	FILE *src, *dst;

	char buf[BUF_LENGTH]; //Letras maisculas necessárias
	int max = 10;

	if ( ( src = fopen(argv[1], "r" ) ) == NULL )
	{

	exit(1); //;
	}

	if ( ( dst = fopen(argv[2], "w" ) ) == NULL )
	{
	exit(2); //;
 	} 

  while( ( fgets( buf, max, src ) ) != NULL ) //MAX não declardo
 	{
 	fputs( buf, dst );
 	}

 fclose( src );
 fclose( dst );
 exit(0); // zero é geralmente indicativo de "sucesso"

} 