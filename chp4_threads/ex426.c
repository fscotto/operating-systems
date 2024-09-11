#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long int bigint_t;

void *fibonacci(void *);

bigint_t *numbers;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("USAGE: fibonacci NUMBER\n");
    exit(EXIT_FAILURE);
  }

  const int n = atoi(argv[1]);
  numbers = (bigint_t *)calloc(n, sizeof(bigint_t));

  pthread_attr_t attr;
  pthread_t thread;
  pthread_attr_init(&attr);
  pthread_create(&thread, &attr, fibonacci, (void *)&n);
  pthread_join(thread, NULL);

  printf("Fibonacci of %d:\n", n);
  for (int i = 0; i < n; i++) {
    printf("%llu ", numbers[i]);
  }
  printf("\n");

  free(numbers);
  numbers = NULL;

  return EXIT_SUCCESS;
}

void *fibonacci(void *args) {
  const int *n = (int *)args;
  bigint_t first = 0, second = 1;

  if (*n >= 1) {
    numbers[0] = first;
  }
  if (*n >= 2) {
    numbers[1] = second;
  }

  for (int i = 2; i < *n; i++) {
    const bigint_t third = first + second;
    first = second;
    second = third;
    numbers[i] = third;
  }

  pthread_exit(0);
}
