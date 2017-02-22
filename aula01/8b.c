#include <stdio.h>
#include <stdlib.h> //Faltava esta biblioteca para o exit
#include <string.h> //para strerror
#include <time.h>//necessário p/ função time()
#include <sys/times.h> //Para medir tempo
#include <unistd.h>

int main( int argc, char *argv[ ] )
{

srand(time(NULL));

int n1 = atoi(argv[1]);
n1--;
int n2 = atoi (argv[2]);
int aleatorio ;
int i = 0;
/***/

 clock_t start, end;
 struct tms t;
 long ticks;

 start = times(&t); /* início da medição de tempo */
 ticks = sysconf(_SC_CLK_TCK); 
/***/

while ( aleatorio != n2)
{
aleatorio = rand()%n1;
printf("Iteracao %d, com nr aleatorio = %d\n", i,  aleatorio);
i++;
}


/***/
printf("\nClock ticks per second: %ld\n", ticks);
 end = times(&t); /* fim da medição de tempo */
/***/


/***/
printf("Clock: %4.2f s\n", (double)(end-start)/ticks);
 printf("User time: %4.2f s\n", (double)t.tms_utime/ticks);
 printf("System time: %4.2f s\n", (double)t.tms_stime/ticks);
 printf("Children user time: %4.2f s\n", (double)t.tms_cutime/ticks);
 printf("Children system time: %4.2f s\n", (double)t.tms_cstime/ticks); 
/***/

}