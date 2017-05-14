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


//Struct de dois inteiros
struct FIFOS {
  int fentrada;
  int frejeitados;
} fifos;


//Struct com as variáveis de um Pedido
struct Pedidos {
  
  char genero;
  int n_pedido;
  float tempo_util;
  int n_rejeit;

} pedido;


//Variaveis Globais
int MAXLINE = 100;
int max_util;
int n_pedidos;
clock_t start, end;
int file;


//Escrita para o ficheiro /tmp/ger.pid (em que pid é a pid do processo)
void write_pedido(struct Pedidos pedido, int type)
{
  double cpu_time_used;
  end = clock();
  cpu_time_used = ((double) (end - start));
  int pid = getpid();

  char line[MAXLINE]; 
  if(type == 0)
    sprintf(line,"%lf -%d -%d : %c -%f -%s\n", cpu_time_used,pid,pedido.n_pedido,pedido.genero,pedido.tempo_util, "PEDIDO"); 
  else if(type == 1)
    sprintf(line,"%lf -%d -%d : %c -%f -%s\n", cpu_time_used,pid,pedido.n_pedido,pedido.genero,pedido.tempo_util, "REJEITADO"); 
  else if(type == 2)
    sprintf(line,"%lf -%d -%d : %c -%f -%s\n", cpu_time_used,pid,pedido.n_pedido,pedido.genero,pedido.tempo_util, "DESCARTADO"); 
 
  write(file, line, strlen(line));

}


//Função que cria um pedido e o envia para a suana
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


//Função que recebe um pedido rejeitado pela sauna
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

 //Número máximo de pedidos
 n_pedidos = atoi(argv[1]);

 //Tempo máximo de utilização
 max_util = atoi(argv[2]);

 //Cria uma thread para criar os pedidos e envia-los para a sauna
 pthread_create(&tpedidos, NULL, thrpedido, &fentrada); 


 //Cria uma estrutura FIFOS
 struct FIFOS fifos;
 fifos.fentrada = fentrada;
 fifos.frejeitados = frejeitados;

 //Cria uma thread para ler os pedidos rejeitados e envia-los novamente se não foram rejeitados mais de 3 vezes
 pthread_create(&trejeitados, NULL, thrrejeitados, (void *) &fifos); 

 pthread_join(tpedidos, NULL); 
 pthread_join(trejeitados, NULL); 
 //Fecha os fifos

 close(file);
 close(fentrada);
 close(frejeitados); 

 return 0; 
} 
