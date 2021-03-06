/*
 * Esta tarefa consiste em receber um texto na entrada e contar 
 * quantos números primos existem nela utilizando multithreads.
 */

#include <stdio.h>
#include <stdlib.h> /* dinamic memory*/
#include <pthread.h> /* threads*/

#define N_THREADS 4
#define N_MAX 64

/* Dados necessarios para realizar a thread*/
typedef struct {
    unsigned long int *numbers; /* Vetor de numeros a serem analisados*/
    int *completeds; /* Vetor de tarefas*/
    int length_numbers; /* comprimento dos vetores*/
    int *prime_numbers_amount; /* quantidade de numeros primos total*/
}dt;

void *thread_function(void *); /* funcao thread*/

int is_prime(unsigned long int ); /*Funcao que determina se um numero eh primo*/

pthread_mutex_t key; /*chave utilizada para travar as threads quando for necessario*/

int main() {
  pthread_t thread[N_THREADS];
  char c;
  int j;
  dt data[N_THREADS];

  /* vetor que guarda os numeros lidos e variavel que contem a quantidade de numeros primos lidos*/
  unsigned long int numbers_vector[N_MAX];

  /** vetor que guarda o estado de cada tarefa, da forma: 
   * tarefa[i] == 0 (nao realizada) 
   * tarefa[i] == 1 (realizada) 
   * para i de 0 a N_MAX
   **/
  int completeds[N_MAX];

  /* Guarda a quantidade total de numeros primos encontrados*/
  int prime_numbers_amount;

  /* Le no maximo N_MAX numeros inteiros sem sinal seguidos de um \n*/
  j = 0;
  do{
    scanf("%li", &numbers_vector[j]);
    c = getchar();
    completeds[j] = 0;
    j += 1;
  }while (c != '\n' && j < N_MAX);

  /* Conta quantos numeros primos estao armazenados no vetor prime_number de entradas em N_THREADS threads em paralelo*/
  prime_numbers_amount = 0;
  for (int i = 0; i < N_THREADS; i++) {
    data[i].numbers = numbers_vector;
    data[i].length_numbers = j;
    data[i].completeds = completeds;
    data[i].prime_numbers_amount = &prime_numbers_amount;
    pthread_create(&(thread[i]), NULL, thread_function, &(data[i]));
  }

  /* Espera pelo fim das N_THREADS threads que estao sendo executadas*/
  for(int i = 0; i < N_THREADS; i++){
    pthread_join(thread[i], NULL);
  }

  printf("%d\n", prime_numbers_amount);
  return 0;
}

/* Executa os N_THREADS threads*/
void *thread_function(void *arg){
  dt *data = (dt *)arg;
  int task = 0, result;
  
  pthread_mutex_lock(&key);
  while(1){

    /* Primeira area critica de memoria: Encontra uma tarefa ainda nao realizada*/
    while(data->completeds[task] == 1 && task < data->length_numbers){
      task++;
    }
    
    /* encerra thread caso nao haja mais tarefas a ser realizadas*/
    if (task >= data->length_numbers){
      pthread_mutex_unlock(&key);
      break;
    }

    /* marca tarefa como realizada*/
    data->completeds[task] = 1;
    pthread_mutex_unlock(&key);

    /* realiza tarefa*/
    result = is_prime(data->numbers[task]);

    /* Segunda area critica de memoria: Soma o valor encontrado na funcao is_prime na quantidade total ja encontrada*/
    pthread_mutex_lock(&key);
    *data->prime_numbers_amount += result;
  }

  return NULL;
}


/* Determinar se o numero dado eh primo*/
/* return 1 caso seja e 0 caso contrario*/
int is_prime(unsigned long int number){
  char response = 1;
  unsigned long int i = 2;

  if (number < 2) response = 0;
  
  while(i <= number/2 && response == 1){
    if (number%i == 0)
      response = 0;
    i++;
  }
  
  return (int)response;
}
