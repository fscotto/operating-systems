#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROUNDUP(a, b) (a + b - 1) / b

void *sort(void *);
void *merge(void *);

typedef struct __THREAD_DATA__ {
  int *array;
  int size;
} thread_data_t;

typedef struct __MERGE_DATA__ {
  thread_data_t tdata1;
  thread_data_t tdata2;
} merge_data_t;

int *original_array;
int *sorted_array;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("USAGE: sortt NUMBERS...\n");
    exit(EXIT_FAILURE);
  }

  const int size = argc - 1;
  original_array = (int *)calloc(size, sizeof(int));
  for (int i = 1; i < argc; i++) {
    original_array[i - 1] = atoi(argv[i]);
  }

  int size1 = ROUNDUP(size, 2);
  int size2 = size - size1;
  int *slice1 = &original_array[0];
  int *slice2 = &original_array[size2 + 1];
  const thread_data_t tdata1 = {.array = slice1, .size = size1};
  const thread_data_t tdata2 = {.array = slice2, .size = size2};

  pthread_attr_t attr;
  pthread_t threads[3];
  pthread_attr_init(&attr);
  pthread_create(&threads[0], &attr, sort, (void *)&tdata1);
  pthread_create(&threads[1], &attr, sort, (void *)&tdata2);
  for (int i = 0; i < 2; i++) {
    pthread_join(threads[i], NULL);
  }

  sorted_array = (int *)calloc(size, sizeof(int));
  merge_data_t mdata = {.tdata1 = tdata1, .tdata2 = tdata2};
  pthread_create(&threads[2], &attr, merge, (void *)&mdata);
  pthread_join(threads[2], NULL);

  for (int i = 0; i < size; i++) {
    printf("%d ", sorted_array[i]);
  }
  printf("\n");

  free(original_array);
  original_array = NULL;

  free(sorted_array);
  sorted_array = NULL;

  return EXIT_SUCCESS;
}

void swap(int *array, int a, int b) {
  int tmp = array[a];
  array[a] = array[b];
  array[b] = tmp;
}

void selection_sort(int *arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    int min_idx = i;
    for (int j = i + 1; j < size; j++) {
      if (arr[j] < arr[min_idx]) {
        min_idx = j;
      }
    }
    swap(arr, min_idx, i);
  }
}

void *sort(void *args) {
  const thread_data_t *tdata = (thread_data_t *)args;
  selection_sort(tdata->array, tdata->size);
  pthread_exit(0);
}

void *merge(void *args) {
  const merge_data_t *mdata = (merge_data_t *)args;
  const thread_data_t tdata1 = mdata->tdata1;
  const thread_data_t tdata2 = mdata->tdata2;
  const int size = tdata1.size + tdata2.size;
  for (int i = 0; i < tdata1.size; i++) {
    sorted_array[i] = tdata1.array[i];
  }
  for (int i = tdata1.size; i < size; i++) {
    sorted_array[i] = tdata2.array[i - tdata1.size];
  }
  selection_sort(sorted_array, size);
  pthread_exit(0);
}
