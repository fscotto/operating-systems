#include "history.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

struct command_t {
  char **args;
  bool bg_flag;
};

struct command_t parse_command(char *);
int digits(char *);

int main(void) {

  // Create and/or read history file
  int history_count = allocate_history();
  if (history_count < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  int should_run = 1;
  while (should_run) {
    char raw[MAXLEN / 2 + 1];
    printf("osh> ");
    fflush(stdout);
    fgets(raw, MAXLEN / 2 + 1, stdin);

    // check if user press newline
    if (strcmp(raw, "\n") == 0) {
      continue;
    }

    // FIXME: handle ^D shortcut
    if (strcmp(raw, "\000") == 0) {
      exit(EXIT_SUCCESS);
    }

    char *c;
    if (strcmp(raw, "!!\n") == 0) {
      if (nth_command(raw, history_count) != -1) {
        printf("%s\n", raw);
      } else {
        fprintf(stderr, "History is empty\n");
        continue;
      }
    } else if ((c = strchr(raw, '!')) != NULL) {
      const int n = digits(c);
      if (nth_command(raw, n) != -1) {
        fprintf(stderr, "%s\n", raw);
      } else {
        printf("Command not found in position %d\n", n);
        continue;
      }
    }

    // remove carriage return
    raw[strlen(raw) - 1] = '\0';

    // save command in the history file
    history_count = add_history(history_count, raw);

    struct command_t cmd = parse_command(raw);
    if (strcmp(cmd.args[0], "exit") == 0) {
      should_run = 0;
    } else if (strcmp(cmd.args[0], "history") == 0) {
      // print on the stdout last 10 commands in reverse way
      history_entry_t *history = get_history(history_count);
      for (int i = HISTORY_SIZE - 1; i >= 0; i--) {
        history_entry_t entry = history[i];
        if (entry.line_number != 0) {
          printf("%5d\t%s", entry.line_number, entry.command);
        }
      }

      free(history);
      history = NULL;
    } else {
      pid_t pid = fork();
      switch (pid) {
      case -1:
        perror("Fork\n");
        exit(EXIT_FAILURE);
      case 0:
        should_run = 0;
        if (execvp(cmd.args[0], cmd.args)) {
          perror("Error");
        }
        break;
      default:
        if (!cmd.bg_flag) {
          wait(NULL);
        }

        break;
      }
    }

    free(cmd.args);
    cmd.args = NULL;
  }

  return EXIT_SUCCESS;
}

struct command_t parse_command(char *raw) {
  int i = 0;
  bool bg_flag = false;
  char **args = (char **)calloc(MAXLEN, sizeof(char *));
  char *tok = strtok(raw, " ");
  while (tok != NULL) {
    if (strcmp(tok, "&") == 0) {
      bg_flag = true;
    } else {
      args[i] = tok;
    }
    tok = strtok(NULL, " ");
    i++;
  }

  struct command_t cmd = {.args = args, .bg_flag = bg_flag};
  return cmd;
}

int digits(char *s) {
  char *sub = malloc(strlen(s) * sizeof(s));
  char c;
  int i = 0, j = 0;
  while ((c = s[i]) != '\n') {
    if (isdigit(c)) {
      sub[j++] = c;
    }
    i++;
  }

  int n = atoi(sub);

  free(sub);
  sub = NULL;

  return n;
}
