#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int collatz(int, int *, int);
void print_buffer(int buff[]);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Usage: collatz NUMBER\n");
    return EXIT_FAILURE;
  }

  int n = atoi(argv[1]);
  if (n <= 1) {
    perror("The number MUST be greater than 1\n");
    return EXIT_FAILURE;
  }

  printf("Collatz of %d\n", n);

  int *buff = mmap(NULL, sizeof(int *) * 1024, PROT_READ | PROT_WRITE,
                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  pid_t pid = fork();
  if (pid == 0) {
    collatz(n, buff, 0);
  } else if (pid > 0) {
    wait(NULL);
    print_buffer(buff);
  }

  return EXIT_SUCCESS;
}

int collatz(int n, int *shm, int steps) {
  *(shm + steps) = n;
  if (n == 1) {
    return n;
  } else if (n % 2 == 0) {
    return collatz(n / 2, shm, steps + 1);
  } else {
    return collatz(3 * n + 1, shm, steps + 1);
  }
}

void print_buffer(int buff[]) {
  int i = 0;
  while (buff[i] != 0) {
    printf("%d ", buff[i]);
    i++;
  }
  printf("\n");
}
