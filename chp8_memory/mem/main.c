#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: mem <ADDRESS>\n");
    return EXIT_FAILURE;
  }

  const unsigned long n = atoi(argv[1]);
  const unsigned long page = n >> 12;            // equal to n / 2^12
  const unsigned long offset = n - (page << 12); // equal to n - (page * 2^12)

  printf("The address %lu contains:\n", n);
  printf("page number = %lu\n", page);
  printf("offset = %lu\n", offset);

  return EXIT_SUCCESS;
}
