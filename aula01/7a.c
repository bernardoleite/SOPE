#include <stdio.h>
#include <stdlib.h>

static void exithand1(void)
{
 printf("Executing exit handler 1\n");
exit(0);
}

static void exithand2(void)
{
 printf("Executing exit handler 2\n");
}

//int atexit(void (*func) (void));

int main()
{
atexit(exithand1);
atexit(exithand1);
atexit(exithand2);

//abort();
printf("Main done!\n");
	return 0;
}