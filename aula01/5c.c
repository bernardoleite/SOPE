#include <stdio.h>
#include <stdlib.h>
#include<unistd.h> 

int main ()
{

	char *buf;
	buf=(char *)malloc(10*sizeof(char));
	buf=getlogin();

	printf(" Hello %s \n",buf);


	printf("PATH : %s\n", getenv("PATH"));
	printf("HOME : %s\n", getenv("HOME"));
	printf("ROOT : %s\n", getenv("ROOT"));
	printf("USER: %s\n", getenv("USER")); //Pedido

   return(0);
}

