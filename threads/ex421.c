#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define OPERATIONS 3
#define MIN(a, b) a > b ? b : a
#define MAX(a, b) a < b ? b : a

void *min(void *);
void *avg(void *);
void *max(void *);

typedef void *(*pthread_fn_t)(void *);

typedef struct __THREAD_DATA__ {
  int *numbers;
  int size;
} thread_data_t;

int minimum = INT_MAX;
int maximum = INT_MIN;
int average = 0;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Usage: statistic NUMBERS...\n");
    exit(EXIT_FAILURE);
  }

  const int size = argc - 1;
  int *numbers = (int *)calloc(size, sizeof(int));
  for (int i = 1; i < argc; i++) {
    numbers[i - 1] = atoi(argv[i]);
  }

  const thread_data_t tdata = {.numbers = numbers, .size = size};
  const pthread_fn_t ops[OPERATIONS] = {min, max, avg};

  pthread_attr_t attr;
  pthread_t threads[OPERATIONS];
  pthread_attr_init(&attr);
  for (size_t i = 0; i < OPERATIONS; i++) {
    pthread_create(&threads[i], &attr, ops[i], (void *)&tdata);
  }

  for (int i = 0; i < OPERATIONS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Summary:\n");
  printf("The average value is %d\n", average);
  printf("The minimum value is %d\n", minimum);
  printf("The maximum value is %d\n", maximum);

  free(numbers);
  numbers = NULL;

  return EXIT_SUCCESS;
}

void *min(void *args) {
  const thread_data_t *tdata = (thread_data_t *)args;
  const int *numbers = tdata->numbers;
  const int size = tdata->size;

  for (int i = 0; i < size; i++) {
    minimum = MIN(minimum, numbers[i]);
  }

  pthread_exit(0);
}

void *max(void *args) {
  const thread_data_t *tdata = (thread_data_t *)args;
  const int *numbers = tdata->numbers;
  const int size = tdata->size;

  for (int i = 0; i < size; i++) {
    maximum = MAX(maximum, numbers[i]);
  }

  pthread_exit(0);
}

int sum(const int *values, const int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += values[i];
  }
  return sum;
}

void *avg(void *args) {
  const thread_data_t *tdata = (thread_data_t *)args;
  const int *numbers = tdata->numbers;
  const int size = tdata->size;
  const int total = sum(numbers, size);
  average = total / size;
  pthread_exit(0);
}
