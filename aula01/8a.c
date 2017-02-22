#include <stdio.h>
#include <stdlib.h> //Faltava esta biblioteca para o exit
#include <string.h> //para strerror
#include <errno.h> //para errno
#include <time.h>//necessário p/ função time()

int main( int argc, char *argv[ ] )
{

srand(time(NULL));

int n1 = atoi(argv[1]);
n1--;
int n2 = atoi (argv[2]);
int aleatorio ;
int i = 0;


while ( aleatorio != n2)
{
aleatorio = rand()%n1;
printf("Iteracao %d, com nr aleatorio = %d\n", i,  aleatorio);
i++;
}


}