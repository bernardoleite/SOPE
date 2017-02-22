// PROGRAMA p6a.c

#include <stdio.h>
#include <stdlib.h> //Faltava esta biblioteca para o exit
#include <string.h> //para strerror
#include <errno.h> //para errno

#define BUF_LENGTH 256

int main(void)
{
	
	FILE *src, *dst;

	char buf[BUF_LENGTH]; //Letras maisculas necessárias
	int max = 10;

/***Novas***/
	printf("Algum erro:  %s\n", strerror(errno)); 
	perror("Algum erro: ");
/*****/

	if ( ( src = fopen( "infile.txt", "r" ) ) == NULL )
	{
	printf("Algum erro:  %s\n", strerror(errno)); //NOVA
	exit(1); //;
	}

	if ( ( dst = fopen( "outfile.txt", "w" ) ) == NULL )
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