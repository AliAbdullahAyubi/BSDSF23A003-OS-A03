#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// =======================
//   CONSTANTS
// =======================
#define MAX_LEN 1024
#define MAXARGS 20
#define ARGLEN  100
#define PROMPT  "FCIT> "
#define HISTORY_SIZE 20

// =======================
//   GLOBAL VARIABLES (extern)
// =======================
extern char *history[HISTORY_SIZE];
extern int history_count;

// =======================
//   FUNCTION DECLARATIONS
// =======================
char *read_cmd(char *prompt, FILE *fp);
char **tokenize(char *cmdline);
int  execute(char **args);
void add_to_history(const char *cmd);
int handle_builtin(char **args);

#endif // SHELL_H

