Com a finalidade de evitar no código desenvolvido situações de competição no acesso a elementos partilhados e para garantir, de forma eficiente, a exclusão mútua de secções críticas de vários threads foram utilizados Mutexes.

Assim, para proteger a secção crítica por parte de um thread utilizámos as seguintes operações de aquisição e libertação (lock e unlock):

      pthread_mutex_lock(&mutex);
          ...  /* secção crítica */
      pthread_mutex_unlock(&mutex);

A inicialização foi efetuada usando uma constante pré-definida em pthread.h denominada PTHREAD_MUTEX_INITIALIZER:

     pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

Comando de invocação:

- Sauna: ./sauna x tal que x é o nr de lugares da sauna.
- Gerador: ./gerador y z tal que y é o nr de pedidos e z é o nr máximo tempo de utilização da sauna.

Saída Padrão:

- STDOUT_FILENO

Estudantes:

Bernardo José Coelho Leite up201404464
Luís Miguel Santos Monteiro Saraiva up201404302
Mónica Ariana Ribeiro Fernandes up201404789
