#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t parent_pid = getpid();
  pid_t pid = fork();

  if (pid > 0) {
    printf("PARENT PID = %d, CHILD PID = %d\n", parent_pid, pid);
    sleep(10);
    wait(NULL);
  }

  return EXIT_SUCCESS;
}
