#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
  pid_t pid;
  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    return 1;
  } else if (pid == 0) {
    execlp("/bin/ls", "ls", NULL);
    printf("LINEA J\n");
  } else {
    wait(NULL);
    printf("Child complete\n");
  }

  return 0;
}
