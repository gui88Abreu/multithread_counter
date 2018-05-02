/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h> /* dinamic memory*/
#include <pthread.h> /* threads*/
#include <unistd.h>

#define N_THREADS 4
#define N_MAX 64

typedef struct {
    int thread;
    unsigned long int *numbers;
    int length_numbers;
    int *prime_numbers_amount;
}dt;

void *thread_function(void *args);
int is_prime(unsigned long int );

int main() {
  pthread_t thread[N_THREADS];
  char c;
  int j;
  dt data[N_THREADS];
  unsigned long int numbers_vector[N_MAX];
  int prime_numbers_amount;

  /* Ler no maximo N_MAX numeros inteiros sem sinal seguidos de um \n*/
  j = 0;
  do{
    scanf("%li", &numbers_vector[j]);
    c = getchar();
    j += 1;
  }while (c != '\n' && j < N_MAX);

  /* Contar quantos numeros primos estao armazenados no vetor prime_number de entradas em N_THREADS processos paralelos*/
  prime_numbers_amount = 0;
  for (int i = 0; i < N_THREADS; i++) {
    data[i].thread = i;
    data[i].numbers = numbers_vector;
    data[i].prime_numbers_amount = &prime_numbers_amount;
    data[i].length_numbers = j;
    pthread_create(&(thread[i]), NULL, thread_function, &(data[i]));
  }

  //sleep(1);
  for(int i = 0; i < N_THREADS; i++){
    pthread_join(thread[i], NULL);
  }

  printf("%d\n", prime_numbers_amount);
  return 0;
}

/* Executa os N_THREADS threads*/
void *thread_function(void *arg){
  dt *data = (dt *)arg;

  for(int k = 0; data->thread + k < data->length_numbers; k += N_THREADS){
    //printf("Executando Thread numero %d\n", data->thread);
    *data->prime_numbers_amount += is_prime(data->numbers[data->thread + k]);
  }
  
  return NULL;
}


/* Determinar se o numero dado eh primo*/
/* return 1 caso seja e 0 caso contrario*/
int is_prime(unsigned long int prime_number){
  int response = 1;
  int i = 2;

  if (prime_number < 2) response = 0;
  
  while(i <= prime_number/2 && response == 1){
    if (prime_number%i == 0)
      response = 0;
    i++;
  }
  
  return response;
}
