#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


int global=1;

int main(void){
	int local = 2;
	if(fork() > 0){
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
		global++;
		local--;
	}
	else{
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
		global--;
		local++;
}
	printf("PID = %d - global = %d; local = %d\n", getpid(),
global, local);
	 return 0;
}
	
//quando um processo cria outro com a chamada fork
//cria o processo filho com as variaveis do processo pai
//mas ambos ficam cada um com as suas copias das variaves


