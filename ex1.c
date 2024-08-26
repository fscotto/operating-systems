#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct MyStruct {
  int value;
};

int value = 5;

int main(void)
{
  struct MyStruct *str;
  str->value = 5;

  int *p = malloc(sizeof(int));
  *p = 100;

  printf("PARENT: %p\n", &value);
  printf("PARENT: %p\n", str);
  printf("PARENT: %p\n", p);

  pid_t pid;
  pid = fork();

  if (pid == 0) {
    value += 15;
    str->value += 15;
    *p += 10;
    printf("CHILD: %p, %d\n", &value, value);
    printf("CHILD: %p, %d\n", &str->value, str->value);
    printf("CHILD: %p, %d\n", p, *p);
    return 0;
  } else if (pid > 0) {
    wait(NULL);
    printf("PARENT: value = %d\n", value);
    printf("PARENT: value = %d\n", str->value);
    printf("PARENT: value = %d\n", *p);

    free(p);

    return 0;
  }
}
