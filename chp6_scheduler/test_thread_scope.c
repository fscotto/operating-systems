#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int scope;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

  if (pthread_attr_getscope(&attr, &scope) != 0) {
    fprintf(stderr, "Scope not found");
    return EXIT_FAILURE;
  } else {
    printf("The current scope is %d\n", scope);
  }

  return EXIT_SUCCESS;
}
