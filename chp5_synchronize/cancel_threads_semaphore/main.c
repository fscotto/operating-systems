#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 10

void *expensive_task(void *);

sem_t semaphore;

int main(void) {

  sem_init(&semaphore, 0, 0);

  pthread_attr_t attr;
  if (pthread_attr_init(&attr) != 0) {
    perror("Error");
    return EXIT_FAILURE;
  }

  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  int n[THREAD_COUNT];
  pthread_t threads[THREAD_COUNT];
  for (int i = 0; i < THREAD_COUNT; i++) {
    n[i] = i;
    printf("Create thread number %d\n", i);
    pthread_create(&threads[i], &attr, expensive_task, (void *)&n[i]);
  }

  printf("Waiting semaphore...\n");
  sem_wait(&semaphore);
  printf("Unlock semaphore\n");

  for (int i = 0; i < THREAD_COUNT; i++) {
    printf("Cancel thread %d\n", i);
    pthread_cancel(threads[i]);
  }

  sem_destroy(&semaphore);

  return EXIT_SUCCESS;
}

void *expensive_task(void *args) {
  const int tid = *((int *)args);
  printf("Thread %d are running...\n", tid);
  sleep(rand() % 31);
  printf("Thread %d terminated!!!\n", tid);
  sem_post(&semaphore);
  pthread_exit(0);
}
