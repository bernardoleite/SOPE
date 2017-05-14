 #include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <termios.h> 
#include <string.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
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


//Struct com um inteiro e um Pedido
struct PEDIDOEFIFO {

  struct Pedidos pedido;
  int fifo;
} pedidoefifo;


pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;  // mutex p/a sec.critica 

//Variaveis Globais
int MAXLINE = 100;
int n_lugares;
int ocupados = 0;
char genero = 'N';
clock_t start, end;
int file;


//Escrita para o ficheiro /tmp/bal.pid (em que pid é a pid do processo)
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



//Função em que verifica se um pedido pode ou não entrar na sauna
void * thrpedido_entrada(void * arg)
{

  struct PEDIDOEFIFO* pedfifo = (struct PEDIDOEFIFO *) arg;
  struct Pedidos pedido = pedfifo->pedido;
  int frejeitados = pedfifo->fifo; 


  printf("Pedido %d a querer entrar %f do genero %c\n",pedido.n_pedido,pedido.tempo_util,pedido.genero);
  write_pedido(pedido,0);
  while(1) {

  //Se a sauna estiver vazia
    if(genero == 'N') {
      pthread_mutex_lock(&mut);
      genero = pedido.genero;
      printf("Pedido %d a entrar %f\n",pedido.n_pedido,pedido.tempo_util);
      ocupados++;
      pthread_mutex_unlock(&mut); 
      sleep(pedido.tempo_util);
      pthread_mutex_lock(&mut);
      ocupados--;
      if(ocupados == 0)
	genero = 'N';
      printf("Pedido %d a sair\n",pedido.n_pedido);
      write_pedido(pedido,2);
      pthread_mutex_unlock(&mut); 

      return NULL;
    }

    //Pedido do genero igual ao da sauna
    else if(genero == pedido.genero) {
      //Sauna não cheia
      if(ocupados < n_lugares) {
        printf("Pedido %d a entrar %f\n",pedido.n_pedido,pedido.tempo_util);
	pthread_mutex_lock(&mut);
	ocupados++;
	pthread_mutex_unlock(&mut); 
	sleep(pedido.tempo_util);
	pthread_mutex_lock(&mut);
	ocupados--;
	printf("Pedido %d a sair\n",pedido.n_pedido);
	if(ocupados == 0)
	  genero = 'N';
        write_pedido(pedido,2);
	pthread_mutex_unlock(&mut); 

        return NULL; 
      }

    }

    //Pedido do genero diferente ao da sauna
    else {
      printf("Pedido %d rejeitado\n",pedido.n_pedido);
      pedido.n_rejeit++;
      write(frejeitados,&pedido,sizeof(pedido));
      write_pedido(pedido,1);
      return NULL; 
    }

  }
  return NULL;
}


//Função que recebe um pedido do gerador e cria uma thread para cada pedido
void * thrpedido_receber(void * arg) 
{
  struct Pedidos pedidos;
  struct PEDIDOEFIFO pedfifo[100];
  pthread_t tpedidos[100];
  int i = 0;
  struct FIFOS* fifos = (struct FIFOS *) arg;
  int fentrada = fifos->fentrada;
  int frejeitados = fifos->frejeitados;
  //Gera genero do utilizador
  printf("PEDIDOS RECEBIDOS:\n");


  //recebe um pedido e cria uma thread
  while(read(fentrada,&pedidos,sizeof(pedidos)) > 0) {
    printf("%d %c % f\n", pedidos.n_pedido, pedidos.genero, pedidos.tempo_util);
   
    pedfifo[i].pedido = pedidos;
    pedfifo[i].fifo = frejeitados;
    pthread_create(&tpedidos[i], NULL, thrpedido_entrada, (void *) &pedfifo[i]); 
    i++;
  }


  for(int j = 0; j < i; j++) {
    pthread_join(tpedidos[j], NULL); 

  }
  
  return NULL;
}


int main(int argc, char* argv[]) 
{ 

  start = clock();
  char filename[1000] = {};

  int pid = getpid();
  snprintf(filename, sizeof(filename), "/tmp/bal.%d.txt", pid);
  file = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0644); 
  //Check Numero de argumentos
  if(argc != 2) {
    fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
    exit(1);
  }

  int   fentrada, frejeitados; 
  pthread_t tpedidos_recebidos; 

  //Criar Fifo da entrada
  fentrada=open("/tmp/entrada",O_RDONLY);

  //Check se abriu o fifo de entrada
  if(fentrada == -1) {
    perror("Can't Open Entrada Fifo\n");
    exit(2);
  } 

  //Criar fifo dos rejeit ados
  frejeitados=open("/tmp/rejeitados",O_WRONLY);

  //Check se abriu o fifo dos rejeitados
  if(frejeitados == -1) {
    perror("Can't Open Rejeitados Fifo\n");
    exit(3);
  } 
  
  //Variavel global com o numero de lugares disponíveis na sauna
  n_lugares = atoi(argv[1]);

  //Cria estrutura FIFOS
  struct FIFOS fifos;
  fifos.fentrada = fentrada;
  fifos.frejeitados = frejeitados;

  //Cria uma thread para receber os pedidos enviados pelo gerador
  pthread_create(&tpedidos_recebidos, NULL, thrpedido_receber, (void *) &fifos); 

  pthread_join(tpedidos_recebidos, NULL); 
  close(file);
  close(fentrada); 
  close(frejeitados); 

  return 0; 
} 
