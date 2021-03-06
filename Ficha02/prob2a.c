// PROBLEMA 2a DA FOLHA 2


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
 int nr, nw;
 unsigned char buffer[BUFFER_SIZE];
 FILE *fd1, *fd2;
 if (argc != 3) {
  printf("Usage: %s <source> <destination>\n", argv[0]);
  return 1;
 }
 fd1 = fopen(argv[1], "r");
 if (fd1 == NULL) {
  perror(argv[1]);
  return 2;
 }
 fd2 = fopen(argv[2], "w");
 if (fd2 == NULL) {
  perror(argv[2]);
  fclose(fd1);
  return 3;
 }
 while ((nr = fread(buffer, BUFFER_SIZE, 1, fd1)) > 0)
  if ((nw = fwrite(buffer, 1, BUFFER_SIZE, fd2)) <= 0 || nw != nr) {
   perror(argv[2]);
   fclose(fd1);
   fclose(fd2);
   return 4;
  }
  fclose(fd1);
  fclose(fd2);
  return 0;
}

