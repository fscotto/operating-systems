#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 4096
#define SHM_NAME "collatz"

int collatz(int, void *);

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

  // Create memory shared object
  int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
  ftruncate(fd, SIZE);
  void *ptr = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  pid_t pid = fork();
  if (pid == 0) {
    collatz(n, ptr);
  } else if (pid > 0) {
    wait(NULL);

    printf("%s\n", (char *)ptr);
    shm_unlink(SHM_NAME);
  }

  return EXIT_SUCCESS;
}

int collatz(int n, void *shm) {
  char buff[50];
  sprintf(buff, "%d", n);
  sprintf(shm, "%s", buff);
  shm += strlen(buff);

  sprintf(shm, "%s", " ");
  shm += 1;

  if (n == 1) {
    return n;
  } else if (n % 2 == 0) {
    return collatz(n / 2, shm);
  } else {
    return collatz(3 * n + 1, shm);
  }
}
