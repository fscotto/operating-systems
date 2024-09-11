#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HISTORY_FILENAME "osh_history"
#define W_MODE "w"
#define R_MODE "r"
#define A_MODE "a+"

int allocate_history(void) {
  FILE *file;
  if (access(HISTORY_FILENAME, F_OK) == -1) {
    if ((file = fopen(HISTORY_FILENAME, W_MODE)) == NULL) {
      perror("Error");
      return -1;
    }
    fclose(file);
  }

  if ((file = fopen(HISTORY_FILENAME, R_MODE)) == NULL) {
    perror("Error");
    return -1;
  }

  int count = -1;
  char line[MAXLEN];
  while (!feof(file)) {
    fgets(line, sizeof(line), file);
    count++;
  }

  fclose(file);
  return count;
}

history_entry_t *get_history(int history_count) {
  FILE *file = fopen(HISTORY_FILENAME, R_MODE);
  if (!file) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  int num = 0;
  char line[MAXLEN];
  const int offset = history_count - HISTORY_SIZE;
  history_entry_t *history = calloc(HISTORY_SIZE, sizeof(history_entry_t));
  while (fgets(line, sizeof(line), file)) {
    if (num >= offset) {
      history_entry_t entry;
      entry.line_number = num + 1;
      strcpy(entry.command, line);
      history[num - offset] = entry;
    }
    num++;
  }
  fclose(file);
  return history;
}

int add_history(int count, char *cmd) {
  FILE *file = fopen(HISTORY_FILENAME, A_MODE);
  if (!file) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  fprintf(file, "%s\n", cmd);
  fclose(file);
  return count + 1;
}

int last_command(char *last) {
  FILE *file = fopen(HISTORY_FILENAME, A_MODE);
  if (!file) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  char line[MAXLEN];
  while (!feof(file)) {
    fgets(line, sizeof(line), file);
  }

  fclose(file);
  strcpy(last, line);
  return 0;
}

int nth_command(char *s, int n) {
  FILE *file = fopen(HISTORY_FILENAME, R_MODE);
  if (!file) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  int num = 0;
  char line[MAXLEN];
  while (fgets(line, sizeof(line), file)) {
    if ((num + 1) == n) {
      strcpy(s, line);
      return 0;
    }
    num++;
  }
  fclose(file);
  return -1;
}
