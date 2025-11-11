#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

// =======================
//   CONSTANTS
// =======================
#define MAX_LEN 1024
#define MAXARGS 20
#define ARGLEN  100
#define PROMPT  "FCIT> "
#define HISTORY_SIZE 20
#define MAX_JOBS 50


// =======================
//   GLOBAL VARIABLES (extern)
// =======================
extern char *history[HISTORY_SIZE];
extern int history_count;
extern pid_t foreground_pid;

typedef struct {
    pid_t pid;
    char command[256];
} Job;

extern Job jobs[MAX_JOBS];

extern int job_count;

// =======================
//   FUNCTION DECLARATIONS
// =======================
char *read_cmd(char *prompt, FILE *fp);
char **tokenize(char *cmdline);
int  execute(char **args);
void add_to_history(const char *cmd);
int handle_builtin(char **args);

void sigint_handler(int sig);
void sigtstp_handler(int sig);

#endif // SHELL_H

