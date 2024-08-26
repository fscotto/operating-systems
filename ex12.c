#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void add_children(pid_t children[], pid_t pid);

int main(void) 
{
  pid_t parent_pid = getpid();
  pid_t children[1024] = {-1};
  for (int i = 0; i < 4; i++) {
    pid_t pid = fork();
    add_children(children, pid);
  }

  if (parent_pid = getpid()) {
    wait(NULL);
    printf("pids: ");
    for (int i = 0; i < 1024; i++) {
      if (children[i] == -1) {
        printf("\n");
        break;
      }
      printf("%d ", children[i]);
    }
  }

  return 0;
}

void add_children(pid_t children[], pid_t pid)
{
  for (int i = 0; i < 1024; i++) {
    if (children[i] < 0) {
      children[i] = pid;
      break;
    }
  }
}
