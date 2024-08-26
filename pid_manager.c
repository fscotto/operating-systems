#include "pid_manager.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN_PID 300
#define MAX_PID 5000
#define SIZE (MAX_PID - MIN_PID + 1)

bool pids[SIZE];

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

int main(void) {
  if (allocate_map() != 1) {
    perror("Failed to initialiaze PID Manager\n");
    exit(1);
  }

  for (int i = 0; i < SIZE; i++) {
    printf("Allocate PID = %d\n", allocate_pid());
  }

  int last = allocate_pid();
  printf("Attempt to allocate another PID, result %d\n", last);

  if (last == -1) {
    printf("Release a PID 4000\n");
    release_pid(4000);
  }

  printf("Allocate another PID: %d\n", allocate_pid());

  printf("Release PIDs 300 and 5000\n");
  release_pid(300);
  release_pid(5000);

  printf("Allocate another PID: %d\n", allocate_pid());
  printf("Allocate another PID: %d\n", allocate_pid());

  return 0;
}
