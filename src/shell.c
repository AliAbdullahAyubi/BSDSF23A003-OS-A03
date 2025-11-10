#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define HISTORY_SIZE 20
char *history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(const char *cmd) {
    if (cmd == NULL || strlen(cmd) == 0)
        return;

    // If history is full, shift older commands up
    if (history_count == HISTORY_SIZE) {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++)
            history[i - 1] = history[i];
        history_count--;
    }

    history[history_count++] = strdup(cmd);
}


// Function to handle built-in commands
int handle_builtin(char **args) {
    if (args[0] == NULL)
        return 1; // empty command

    // exit
    if (strcmp(args[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }
    //history
    else if (strcmp(args[0], "history") == 0) {
     for (int i = 0; i < history_count; i++) {
         printf("%d  %s\n", i + 1, history[i]);
     }
     return 1;
    }


    // cd
    else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "myshell: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("myshell");
            }
        }
        return 1;
    }

    // help
    else if (strcmp(args[0], "help") == 0) {
        printf("MyShell built-in commands:\n");
        printf("  cd <dir>   - Change directory\n");
        printf("  help       - Show this help message\n");
        printf("  exit       - Exit the shell\n");
        printf("  jobs       - Show running background jobs (not implemented yet)\n");
        return 1;
    }

    // jobs (placeholder)
    else if (strcmp(args[0], "jobs") == 0) {
        printf("Job control not yet implemented.\n");
        return 1;
    }

    // not a built-in
    return 0;
}

char* read_cmd(char* prompt, FILE* fp) {
    // Ignore FILE* parameter (readline reads directly from stdin)
    (void)fp;

    // Use readline() instead of manual input
    char *cmdline = readline(prompt);

    if (cmdline == NULL) {  // Ctrl+D pressed
        return NULL;
    }

    // If the user typed something, add it to Readline's internal history
    if (strlen(cmdline) > 0) {
        add_history(cmdline);
    }

    return cmdline;
}




char** tokenize(char* cmdline) {
    // Edge case: empty command line
    if (cmdline == NULL || cmdline[0] == '\0' || cmdline[0] == '\n') {
        return NULL;
    }

    char** arglist = (char**)malloc(sizeof(char*) * (MAXARGS + 1));
    for (int i = 0; i < MAXARGS + 1; i++) {
        arglist[i] = (char*)malloc(sizeof(char) * ARGLEN);
        bzero(arglist[i], ARGLEN);
    }

    char* cp = cmdline;
    char* start;
    int len;
    int argnum = 0;

    while (*cp != '\0' && argnum < MAXARGS) {
        while (*cp == ' ' || *cp == '\t') cp++; // Skip leading whitespace
        
        if (*cp == '\0') break; // Line was only whitespace

        start = cp;
        len = 1;
        while (*++cp != '\0' && !(*cp == ' ' || *cp == '\t')) {
            len++;
        }
        strncpy(arglist[argnum], start, len);
        arglist[argnum][len] = '\0';
        argnum++;
    }

    if (argnum == 0) { // No arguments were parsed
        for(int i = 0; i < MAXARGS + 1; i++) free(arglist[i]);
        free(arglist);
        return NULL;
    }

    arglist[argnum] = NULL;
    return arglist;
}
