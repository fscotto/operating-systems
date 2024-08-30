#define _GNU_SOURCE
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool prime(int n);
void *primes(void *);

const int thread_err = 1;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Usage: prime NUMBER\n");
    exit(EXIT_SUCCESS);
  }

  const int n = atoi(argv[1]);

  pthread_attr_t attr;
  pthread_t thread;
  pthread_attr_init(&attr);
  pthread_create(&thread, &attr, primes, (void *)&n);
  pthread_join(thread, NULL);

  return EXIT_SUCCESS;
}

void *primes(void *args) {
  printf("Thread %d calculate primes number\n", gettid());
  const int *n = (int *)args;

  if (*n <= 1) {
    perror("Is not possible calculate prime numbers for number less or equal "
           "than 1\n");
    pthread_exit((void *)&thread_err);
  }

  for (int i = 2; i <= *n; i++) {
    if (prime(i)) {
      printf("%d ", i);
    }
  }
  printf("\n");
  pthread_exit(0);
}

bool prime(int n) {
  for (int i = 2; i < n; i++) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}
