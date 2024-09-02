#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t pid, pid1;

  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork failed");
    return EXIT_FAILURE;
  } else if (pid == 0) {
    pid1 = getpid();
    printf("child: pid = %d\n", pid);   // A
    printf("child: pid1 = %d\n", pid1); // B
  } else {
    pid1 = getpid();
    printf("parent: pid = %d\n", pid);   // C
    printf("parent: pid1 = %d\n", pid1); // D
    wait(NULL);
  }

  return EXIT_SUCCESS;
}
