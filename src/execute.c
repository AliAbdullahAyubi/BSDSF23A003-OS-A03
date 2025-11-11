#include "shell.h"

#define MAX_JOBS 50


// Global job list
Job jobs[MAX_JOBS];
int job_count = 0;

int execute(char* arglist[]) {
    int background = 0;
    int i = 0;

    // Detect '&' at the end of the command
    while (arglist[i] != NULL) i++;
    if (i > 0 && strcmp(arglist[i - 1], "&") == 0) {
        background = 1;
        arglist[i - 1] = NULL; // remove '&' so execvp won't see it
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        // --- Child process ---
        execvp(arglist[0], arglist);
        perror("Command not found");  // Only runs if execvp fails
        exit(1);
    } else {
        // --- Parent process ---
        if (background) {
            // Background process → don't wait
            printf("[BG] PID %d running in background\n", pid);
            if (job_count < MAX_JOBS) {
                jobs[job_count].pid = pid;
                snprintf(jobs[job_count].command, sizeof(jobs[job_count].command),
                         "%s", arglist[0]);
                job_count++;
            } else {
                fprintf(stderr, "Job list full!\n");
            }
        } else {
            // Foreground process → wait for it to finish
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

