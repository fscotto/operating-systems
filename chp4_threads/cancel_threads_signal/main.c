#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 10

void *expensive_task(void *);
void send_finish_signal(void *);
void handler(int);

pthread_t main_thread;
pthread_t threads[THREAD_COUNT];

int main(void) {
  main_thread = pthread_self();

  // Register handler for SIGCHLD signal
  signal(SIGCHLD, handler);

  pthread_attr_t attr;
  if (pthread_attr_init(&attr) != 0) {
    perror("Error");
    return EXIT_FAILURE;
  }

  int n[THREAD_COUNT];
  for (int i = 0; i < THREAD_COUNT; i++) {
    n[i] = i;
    printf("Create thread number %d\n", i);
    pthread_create(&threads[i], &attr, expensive_task, (void *)&n[i]);
  }

  for (int i = 0; i < THREAD_COUNT; i++) {
    printf("Waiting thread number %d...\n", i);
    pthread_join(threads[i], NULL);
    printf("Thread %d joined\n", i);
  }

  return EXIT_SUCCESS;
}

void handler(int sig) {
  printf("Caught signal %d\n", sig);
  for (int i = 0; i < THREAD_COUNT; i++) {
    pthread_cancel(threads[i]);
  }
}

void send_finish_signal(void *args) {
  int *id = (int *)args;
  printf("Thread %d send SIGCHLD signal\n", *id);
  pthread_kill(main_thread, SIGCHLD);
  printf("Thread %d cleanup handle is done\n", *id);
}

void *expensive_task(void *args) {
  pthread_cleanup_push(send_finish_signal, args);
  const int tid = *((int *)args);
  printf("Thread %d are running...\n", tid);
  sleep(rand() % 31);
  printf("Thread %d terminated!!!\n", tid);
  pthread_cleanup_pop(1);
  pthread_exit(0);
}
