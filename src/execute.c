#include "shell.h"

#define MAX_JOBS 50


// Global job list
pid_t foreground_pid = -1;
Job jobs[MAX_JOBS];
int job_count = 0;

int execute(char* arglist[]) {
    int background = 0;
    int i = 0;

    // Detect '&' at the end
    while (arglist[i] != NULL) i++;
    if (i > 0 && strcmp(arglist[i - 1], "&") == 0) {
        background = 1;
        arglist[i - 1] = NULL; // remove '&'
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        // --- Child process ---
        // Restore default signal handling for child
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        execvp(arglist[0], arglist);
        perror("Command not found");
        exit(1);
    } else {
        if (background) {
            // --- Background ---
            printf("[BG] PID %d running in background\n", pid);
            if (job_count < MAX_JOBS) {
                jobs[job_count].pid = pid;
                snprintf(jobs[job_count].command, sizeof(jobs[job_count].command),
                         "%s", arglist[0]);
                job_count++;
            }
        } else {
            // --- Foreground ---
            foreground_pid = pid;
            int status;
            waitpid(pid, &status, WUNTRACED);
            foreground_pid = -1;  // reset after completion
        }
    }

    return 0;
}

