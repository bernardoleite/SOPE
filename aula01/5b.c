#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> 

int main() {
char *buf;
buf=(char *)malloc(10*sizeof(char));
buf=getlogin();
printf(" Hello %s \n",buf);
}