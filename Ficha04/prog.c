#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int main(){
	int i=0;
	
	for(; i<6; i++)
	{
		printf("I'm alive!\n");
		sleep(5);
	}
}