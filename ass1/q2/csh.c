#include "word.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

void execute_command(const char* cmd, char* const command[], int argsNum) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (strcmp(cmd, "dir") == 0) {
            execvp("dir", command);
        } else if (strcmp(cmd, "date") == 0) {
            execvp("date", command);
        } else if (strcmp(cmd, "word") == 0) {
            // internal
            wordRun(argsNum, command);
        } else {
            printf(RED "[x] Unsupported command: %s\n" RESET, cmd);
        }

        // perror("execvp");
        exit(errno);
    } else {
        // Parent process
        int status;
        wait(NULL);
        if (strcmp(cmd, "dir") == 0) {
            if (chdir(command[argsNum - 1]) != 0) {
                perror("chdir");
            }
        }
    }
}

int main() {
    char input[50];

    while (1) {
        char* username = getlogin();
        printf(WHT "[%s:" RESET RED "%s" RESET WHT "]$ " RESET, username, getcwd(NULL, 0));
        char* inputArray[50];
        int i = 0;
        fgets(input, sizeof(input), stdin);

        // for empty inputs
        if (strlen(input) == 1 && input[0] == '\n') {
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        char* tokens[20];  // 20 tokens max
        int tokenCount = 0;

        char* token = strtok(input, " ");  

        while (token != NULL) {
            tokens[tokenCount] = token;
            tokenCount++;
            token = strtok(NULL, " ");
        }

        char* cmd = tokens[0];
        // printf("Tokens:\n");
        // for (int i = 0; i < tokenCount; i++) {
        //     printf("Token %d: %s\n", i + 1, tokens[i]);
        // }
        if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
            break;
        }

        tokens[tokenCount] = NULL;
        execute_command(cmd, tokens, tokenCount);
    }

    return 0;
}
