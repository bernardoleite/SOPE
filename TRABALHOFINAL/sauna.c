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

struct FIFOS {
  int fentrada;
  int frejeitados;
} fifos;

struct Pedidos {
  
  char genero;
  int n_pedido;
  int tempo_util;
  int n_rejeit;

} pedido;


struct PEDIDOEFIFO {

  struct Pedidos pedido;
  int fifo;
}pedidoefifo;

pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;  // mutex p/a sec.critica 
struct Pedidos ped;
int n_lugares;
int ocupados = 0;
char genero = 'N';

 
int readline(int fd, char *str) 
{ 
  int n; 
  do { 
    n = read(fd,str,1); 
  } while (n>0 && *str++ != '\0'); 
  return (n>0); 
} 

void * thrpedido_entrada(void * arg)
{

  struct PEDIDOEFIFO* pedfifo = (struct PEDIDOEFIFO *) arg;
  struct Pedidos pedido = pedfifo->pedido;
  int frejeitados = pedfifo->fifo; 


  printf("Pedido %d a querer entrar %d do genero %c\n",pedido.n_pedido,pedido.tempo_util,pedido.genero);

  while(1) {

  //Sauna Vazia
	  if(genero == 'N') {
    	    pthread_mutex_lock(&mut);
	    genero = pedido.genero;
	    printf("Pedido %d a entrar %d\n",pedido.n_pedido,pedido.tempo_util);
	    ocupados++;
	    pthread_mutex_unlock(&mut); 
	    sleep(pedido.tempo_util);
	    pthread_mutex_lock(&mut);
	    ocupados--;
	    if(ocupados == 0)
		genero = 'N';
	    pthread_mutex_unlock(&mut); 
	    printf("Pedido %d a sair\n",pedido.n_pedido);

		return NULL;
	  }

	  //Pedido do genero igual ao da sauna
	  else if(genero == pedido.genero) {
	    		//Sauna não cheia
		    if(ocupados < n_lugares) {
		      printf("Pedido %d a entrar %d\n",pedido.n_pedido,pedido.tempo_util);
		      pthread_mutex_lock(&mut);
		      ocupados++;
		      pthread_mutex_unlock(&mut); 
		      sleep(pedido.tempo_util);
		      pthread_mutex_lock(&mut);
		      ocupados--;
		      printf("Pedido %d a sair\n",pedido.n_pedido);
	    	      if(ocupados == 0)
			genero = 'N';
		      pthread_mutex_unlock(&mut); 
		      return NULL; 
		    }

	  }

	  //Pedido do genero diferente ao da sauna
	  else {
	    printf("Pedido %d rejeitado\n",pedido.n_pedido);
	    pedido.n_rejeit++;
            write(frejeitados,&pedido,sizeof(pedido));
	    return NULL; 

	  }

  }
  return NULL;
}


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

  while(read(fentrada,&pedidos,sizeof(pedidos)) > 0) {
   printf("%d %c % d\n", pedidos.n_pedido, pedidos.genero, pedidos.tempo_util);
   
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

 //Criar fifo dos rejeitados
 frejeitados=open("/tmp/rejeitados",O_WRONLY);


 //Check se abriu o fifo dos rejeitados
 if(frejeitados == -1) {
   perror("Can't Open Rejeitados Fifo\n");
   exit(3);
 } 
 
 n_lugares = atoi(argv[1]);
 struct FIFOS fifos;
 fifos.fentrada = fentrada;
 fifos.frejeitados = frejeitados;
 pthread_create(&tpedidos_recebidos, NULL, thrpedido_receber, (void *) &fifos); 

 pthread_join(tpedidos_recebidos, NULL); 

 close(fentrada); 
 close(frejeitados); 
 return 0; 
} 
