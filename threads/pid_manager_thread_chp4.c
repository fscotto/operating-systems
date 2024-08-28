#include "pid_manager.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MIN_PID 300
#define MAX_PID 5000
#define SIZE (MAX_PID - MIN_PID + 1)

bool pids[SIZE];

void *runner(void *param);

int allocate_map(void) {
  for (int i = 0; i < SIZE; i++) {
    pids[i] = false;
  }
  return 1;
}

int allocate_pid(void) {
  for (int i = 0; i < SIZE; i++) {
    if (!pids[i]) {
      pids[i] = true;
      return i + MIN_PID;
    }
  }
  return -1;
}

void release_pid(int pid) {
  if (pid >= MIN_PID && pid <= MAX_PID) {
    pids[pid - MIN_PID] = false;
  }
}

void *runner(void *param) {
  const pid_t self = gettid();
  const int pid = allocate_pid();
  printf("Run thread %d handle PID %d.\n", self, pid);
  sleep(rand() % 31);
  release_pid(pid);
  printf("Thread %d ended\n", self);
  pthread_exit(0);
}

int main(void) {
  const int thread_count = 100;
  pthread_t threads[thread_count];

  if (!allocate_map()) {
    perror("Failed to allocate PID Manager\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < thread_count; i++) {
    printf("Created thread %d\n", i);
    pthread_create(&threads[i], NULL, runner, NULL);
  }

  for (int i = 0; i < thread_count; i++) {
    printf("Waiting thread %d\n", i);
    pthread_join(threads[i], NULL);
  }

  return EXIT_SUCCESS;
}
