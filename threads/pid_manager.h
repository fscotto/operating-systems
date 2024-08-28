#if !defined(__PID_MANGER_H__)
#define __PID_MANGER_H__
#define _GNU_SOURCE

// Crea e inizializza una struttura per rappresentare i pid;
// restituisce -1 in caso di insuccesso e 1 in caso di successo.
int allocate_map(void);

// Alloca e restituisce un pid, restituisce -1 se non è possibile
// assegnare un PID (tutti i pid sono in uso).
int allocate_pid(void);

// Rilascia un pid
void release_pid(int pid);

#endif
