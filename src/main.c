/*
 * Esta tarefa consiste em receber um texto na entrada e contar quantos números primos existem nela
 * utilizando multithreads.
 */

#include <stdio.h>
#include <stdlib.h> /* dinamic memory*/
#include <pthread.h> /* threads*/

#define N_THREADS 4
#define N_MAX 64

/* Dados necessarios para realizar a thread*/
typedef struct {
    unsigned long int *numbers;
    int *completeds;
    int length_numbers;
    int *prime_numbers_amount;
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

  /* vetor que guarda o estado de cada tarefa, ou seja, nao realizada (0) ou realizada (1)*/
  int completeds[N_MAX];

  int prime_numbers_amount;

  /* Ler no maximo N_MAX numeros inteiros sem sinal seguidos de um \n*/
  j = 0;
  do{
    scanf("%li", &numbers_vector[j]);
    c = getchar();
    completeds[j] = 0;
    j += 1;
  }while (c != '\n' && j < N_MAX);

  /* Contar quantos numeros primos estao armazenados no vetor prime_number de entradas em N_THREADS threads em paralelo*/
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
  
  while(1){

    pthread_mutex_lock(&key);
    while(data->completeds[task] == 1 && task < data->length_numbers){
      task++;
    }
    
    if (task >= data->length_numbers){
      pthread_mutex_unlock(&key);
      break;
    }

    data->completeds[task] = 1;
    pthread_mutex_unlock(&key);

    result = is_prime(data->numbers[task]);

    pthread_mutex_lock(&key);
    *data->prime_numbers_amount += result;
    pthread_mutex_unlock(&key);
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
