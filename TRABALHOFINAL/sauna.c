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


struct Pedidos {
  
  char genero;
  int n_pedido;
  int tempo_util;
  int n_rejeit;
} pedido;

pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;  // mutex p/a sec.critica 
struct Pedidos ped[100];
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

void * thrpedido_entrada(void * arg1, void * arg2)
{

  struct Pedidos pedido = *(struct Pedidos *) arg1;
  int frejeitados = *(int *) arg2;
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
		genero == 'N';
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
	    	      if(ocupados == 0)
			genero == 'N';
		      printf("Pedido %d a sair\n",pedido.n_pedido);
		      pthread_mutex_unlock(&mut); 
		      return NULL; 
		    }
	    		//Sauna Cheia

	  }

	  //Pedido do genero diferente ao da sauna
	  else {
	    printf("Pedido %d rejeitado\n",pedido.n_pedido);
	    pedido.n_rejeit++;
	    write(fsaida,&pedido,sizeof(pedido));
	    return NULL; 

	  }

  }
  return NULL;
}


void * thrpedido_receber(void * arg1, void * arg2) 
{
  struct Pedidos pedidos[100];
  pthread_t tpedidos[100];
  int i = 0;
  int fentrada = *(int *) arg1;
  int frejeitados = *(int *) arg2;
  //Gera genero do utilizador
  printf("PEDIDOS RECEBIDOS:\n");
  while(read(fentrada,&pedidos[i],sizeof(ped[0])) > 0) {
   printf("%d ",pedidos[i].n_pedido); 
   printf("%c ",pedidos[i].genero); 
   printf("%d\n",pedidos[i].tempo_util);
   i++;
  }
  
  for(int j = 0; j < i; j++) {
    pthread_create(&tpedidos[j], NULL, thrpedido_entrada, &pedidos[j], &saida); 

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

 pthread_create(&tpedidos_recebidos, NULL, thrpedido_receber, &fentrada); 

 pthread_join(tpedidos_recebidos, NULL); 

 close(fentrada); 
 close(frejeitados); 
 return 0; 
} 
