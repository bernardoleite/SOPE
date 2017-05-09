#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
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
} fifos;

struct Pedidos {
  
  char genero;
  int n_pedido;
  float tempo_util;
  int n_rejeit;

} pedido;

int MAXLINE = 100;
int max_util;
int n_pedidos;
clock_t start, end;
int file;

void write_pedido(struct Pedidos pedido, int type)
{
  double cpu_time_used;
  end = clock();
  cpu_time_used = ((double) (end - start));
  int pid = getpid();
  int tid = (unsigned int)pthread_self();

  char line[MAXLINE]; 
  if(type == 0)
    sprintf(line,"%lf -%d  -%d -%d : %c -%f -%s\n", cpu_time_used,pid,tid,pedido.n_pedido,pedido.genero,pedido.tempo_util, "RECEBIDO"); 
  else if(type == 1)
    sprintf(line,"%lf -%d  -%d -%d : %c -%f -%s\n", cpu_time_used,pid,tid,pedido.n_pedido,pedido.genero,pedido.tempo_util, "REJEITADO"); 
  else if(type == 2)
    sprintf(line,"%lf -%d  -%d -%d : %c -%f -%s\n", cpu_time_used,pid,tid,pedido.n_pedido,pedido.genero,pedido.tempo_util, "SERVIDO"); 
 
  write(file, line, strlen(line));

}


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
    ped.tempo_util = (float) (1 + rand() % (max_util)) / 1000;

    ped.n_pedido = j + 1;
    ped.n_rejeit = 0;
   printf("%d ",ped.n_pedido); 
   printf("%c ",ped.genero); 
   printf("%f\n",ped.tempo_util);
   write_pedido(pedido,0);
   write(fentrada,&ped,sizeof(ped));
  }
  return NULL;
}


void * thrrejeitados(void * arg) {
  struct FIFOS* fifos = (struct FIFOS *) arg;

  int fentrada = fifos->fentrada;
  int frejeitados = fifos->frejeitados;
  struct Pedidos pedido;

  while(read(frejeitados,&pedido,sizeof(pedido)) > 0) {
    printf("A entrar rejeitado %d Rejeitado %d\n",pedido.n_pedido,pedido.n_rejeit);
    write_pedido(pedido,1);
    if(pedido.n_rejeit < 3) {
      printf("MANDAR PEDIDO %d\n",pedido.n_pedido);
      write(fentrada,&pedido,sizeof(pedido));
    }
    else {
      printf("MAX REJEITADO PEDIDO %d\n",pedido.n_pedido);
      write_pedido(pedido,2);
    }
  }
  return NULL;
}

int readline(int fd, char *str); 
int main(int argc, char * argv[]) 
{ 

  start = clock();
  char filename[1000] = {};

  int pid = getpid();
  snprintf(filename, sizeof(filename), "/tmp/ger.%d.txt", pid);
  file = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0644); 


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

 struct FIFOS fifos;
 fifos.fentrada = fentrada;
 fifos.frejeitados = frejeitados;
 pthread_create(&trejeitados, NULL, thrrejeitados, (void *) &fifos); 
 pthread_join(tpedidos, NULL); 
 pthread_join(trejeitados, NULL); 
 //Fecha os fifos

close(file);
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
