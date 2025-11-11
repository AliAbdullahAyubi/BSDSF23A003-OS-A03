#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main() {
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);

    char* cmdline;
    char** arglist;

    while ((cmdline = read_cmd(PROMPT, stdin)) != NULL) {
        if (cmdline[0] == '!') {
         int cmd_num = atoi(&cmdline[1]); // get number after '!'
         if (cmd_num > 0 && cmd_num <= history_count) {
          free(cmdline);
          cmdline = strdup(history[cmd_num - 1]);
          printf("Re-executing: %s\n",cmdline);
         } else {
          printf("No such command in history.\n");
          continue;
         }
        }

        if ((arglist = tokenize(cmdline)) != NULL) {
             add_to_history(cmdline);

	   if(handle_builtin(arglist) == 0){
             execute(arglist);
	   }

            // Free the memory allocated by tokenize()
            for (int i = 0; arglist[i] != NULL; i++) {
                free(arglist[i]);
            }
            free(arglist);
        }
        free(cmdline);
    }

    printf("\nShell exited.\n");
    return 0;
}
