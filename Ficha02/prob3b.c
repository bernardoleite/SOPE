//PROBLEMA 3b DA FOLHA 2
//REDIRECIONA A SAIDA PARA O SEGUNDO FICHEIRO (SE HOUVER)
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
 int fd1, nr, nw;
 unsigned char buffer[BUFFER_SIZE];


 if (argc > 3 || argc == 0) {
  printf("Usage: %s <source> <destination>\n", argv[0]);
  return 1;
 }
 fd1 = open(argv[1], O_RDONLY);
 if (fd1 == -1) {
  perror(argv[1]);
  return 2;
 }
 if (argc == 3) {
  int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644);
   if (fd1 == -1) {
    perror(argv[1]);
    return 3;
  }
  dup2(fd2,STDOUT_FILENO);
}
 while ((nr = read(fd1, buffer, BUFFER_SIZE)) > 0)
  if ((nw = write(STDOUT_FILENO, buffer, nr)) <= 0 || nw != nr) {
   perror(argv[1]);
   close(fd1);
   return 4;
  }
  close(fd1);
  return 0;
}
