#ifndef __HISTORY_H__
#define __HISTORY_H__

#define MAXLEN 80
#define HISTORY_SIZE 10

typedef struct __HISTORY_ENTRY__ {
  int line_number;
  char command[MAXLEN];
} history_entry_t;

// Create new history file if not exists,
// otherwise read last command count and return it
int allocate_history(void);

// Return last ten command in the history file
history_entry_t *get_history(int);

// Write a record in the history file
int add_history(int, char *);

// Read last command in the history file
int last_command(char *);

// Search nth command in the history file,
// if not found nth command return -1.
int nth_command(char *, int);

#endif // __HISTORY_H__
