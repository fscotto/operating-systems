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

typedef struct __PID_REGISTRY__ {
  bool *pids;
  pthread_mutex_t mtx;
} pid_registry_t;

void *runner(void *param);

pid_registry_t registry;

int allocate_map(void) {
  registry.pids = (bool *)calloc(SIZE, sizeof(bool));
  if (pthread_mutex_init(&registry.mtx, NULL) != 0) {
    perror("Cannot create mutex\n");
    return -1;
  }
  return 1;
}

int deallocate_map(void) {
  if (registry.pids == NULL) {
    perror("Cannot deallocate unallocated struct\n");
    return 1;
  }
  free(registry.pids);
  registry.pids = NULL;

  return pthread_mutex_destroy(&registry.mtx);
}

int allocate_pid(void) {
  int pid = -1;
  for (int i = 0; i < SIZE && pid == -1; i++) {
    pthread_mutex_lock(&registry.mtx);
    if (!registry.pids[i]) {
      registry.pids[i] = true;
      pid = i + MIN_PID;
    }
    pthread_mutex_unlock(&registry.mtx);
  }
  return pid;
}

void release_pid(int pid) {
  if (pid >= MIN_PID && pid <= MAX_PID) {
    pthread_mutex_lock(&registry.mtx);
    registry.pids[pid - MIN_PID] = false;
    pthread_mutex_unlock(&registry.mtx);
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

  if (!deallocate_map()) {
    perror("Failed to deallocate PID Manager\n");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
