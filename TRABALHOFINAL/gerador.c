#include <stdio.h> 
#include <unistd.h> 
#include <termios.h> 
#include <string.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h> 

struct FIFOS {
  int fentrada;
  int frejeitados;
} fifo;

struct Pedidos {
  
  char genero;
  int n_pedido;
  int tempo_util;
  int n_rejeit;

} pedido;

int max_util;
int n_pedidos;

void * thrpedido(void * arg) 
{

  int fentrada = *(int *) arg;
  //Gera genero do utilizador

  for(int j = 0; j < n_pedidos; j++) {
    struct Pedidos ped;
    int i = rand() % 2;
    if(i == 0)
      ped.genero = 'F';
    else
      ped.genero = 'M';
    ped.tempo_util = 1 + rand() % (max_util);

    ped.n_pedido = j + 1;
    ped.n_rejeit = 0;
    write(fentrada,&ped,sizeof(ped));
  }
  return NULL;
}

void * thrrejeitado(void * arg1) 
{

  struct FIFOS fifo = *(struct FIFOS *) arg1;

  struct Pedidos pedidos;
  
  while( read(fifo.frejeitados, &pedidos, sizeof(pedidos)) > 0)
  {
    if (pedidos.n_rejeit < 3)
      write(fifo.fentrada, &pedidos, sizeof(pedidos));
  }

  
  return NULL;
}



int readline(int fd, char *str); 
int main(int argc, char * argv[]) 
{ 

 //Check Numero de argumentos
 if(argc != 3) {
  fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
  exit(1);
 }

 int   fentrada, frejeitados; 


 srand(time(NULL));
 pthread_t tpedidos, trejeitados; 

 //Criar Fifo da entrada
 mkfifo("/tmp/entrada",0660); 
 fentrada=open("/tmp/entrada",O_WRONLY);

 //Check se abriu o fifo de entrada
 if(fentrada == -1) {
   perror("Can't Open Entrada Fifo\n");
   exit(2);
 } 
 
 //Criar fifo dos rejeitados
 mkfifo("/tmp/rejeitados",0660);
 frejeitados=open("/tmp/rejeitados",O_RDONLY);

 //Check se abriu o fifo dos rejeitados
 if(frejeitados == -1) {
   perror("Can't Open Rejeitados Fifo\n");
   exit(3);
 } 

 n_pedidos = atoi(argv[1]);
 max_util = atoi(argv[2]);

 pthread_create(&tpedidos, NULL, thrpedido, &fentrada); 

 struct FIFOS fifo;
 fifo.fentrada = fentrada;
 fifo.frejeitados = frejeitados;
 pthread_create(&trejeitados, NULL, thrrejeitado, &fifo); 

 pthread_join(tpedidos, NULL); 
 pthread_join(trejeitados, NULL); 

 //Fecha os fifos
 close(fentrada);
 close(frejeitados); 
 return 0; 
} 


int readline(int file, char *str) 
{ 
 int n; 
 do { 
   n = read(file,str,1); 
 } while (n>0 && *str++ != '\0'); 
 return (n>0); 
} 
