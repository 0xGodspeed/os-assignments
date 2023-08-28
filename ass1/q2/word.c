#include "word.h"

void wordNewline(char *file) {
    int numWord = 0;
    char fileContent[1000];
    int fd = open(file, O_RDONLY);
    read(fd, fileContent, 1000);
    char *fileTokens[1000];
    int tokenCount = 0;
    char *token = strtok(fileContent, " ");
    while (token != NULL) {
        fileTokens[tokenCount] = token;
        tokenCount++;
        token = strtok(NULL, " ");
    }
    for (int i = 0; i < tokenCount; i++) {
        if (strchr(fileTokens[i], '\n') != NULL) {
            continue;
        }
        numWord++;
    }

    printf("numWord: %d\n", numWord);
}

void word(char *file, bool newline_bool) {
    FILE *ptr;
    char ch;

    ptr = fopen(file, "r");

    if (NULL == ptr) {
        printf(RED "[-] Error" RESET "File doesnt exist");
    }

    int words = 0;
    int charr = 0;
    while ((ch = fgetc(ptr)) != EOF) {
        printf("%c\n", ch);
        charr++;
        if (newline_bool) {
            if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0') words++;
        } else {
            if (ch == ' ' || ch == '\t') {
                words++;
            }
        }
    }

    if (charr > 0) {
        words++;
    }
    if (newline_bool) {
        printf("Total words: %d\n", words);
    } else {
        printf("Total words without newline: %d\n", words);
    }
}

void word_cmp(char *file1, char *file2) {
    FILE *ptr;
    FILE *ptr2;
    char ch;

    ptr = fopen(file1, "r");
    ptr2 = fopen(file2, "r");

    if (NULL == ptr || NULL == ptr2) {
        printf(RED "[-] Error" RESET " File doesnt exist");
    }

    int words = 0;
    int words2 = 0;
    while ((ch = fgetc(ptr)) != EOF) {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0') words++;
    }
    while ((ch = fgetc(ptr2)) != EOF) {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0') words2++;
    }

    printf("Differnce of words: %d\n", abs(words - words2));
}

int wordRun(int argNum, char *args[]) {
    bool newline_bool = true;
    char *file;
    if (argNum == 2 && !(args[1][0] == '-')) {
        file = args[1];
        word(file, newline_bool);
    } else if (argNum == 3) {
        if (strcmp("-n", args[1]) == 0) {
            newline_bool = false;
            file = args[2];
            wordNewline(file);
            // word(file, newline_bool);
        } else if (strcmp("-d", args[1]) == 0) {
            printf(RED "[-] Usage:" RESET " word -d [file_name] [file_name2]");
            return EXIT_FAILURE;
        } else if (args[1][0] == '-') {
            printf(RED "[-] Unsupported option: %s\n" RESET, args[1]);
            return EXIT_FAILURE;
        }
    } else if (argNum == 4) {
        if (strcmp("-n", args[1]) == 0) {
            printf(RED "[-] Usage:" RESET " word -n [file_name]");
        } else if (strcmp("-d", args[1]) == 0) {
            char *file1 = args[2];
            char *file2 = args[3];
            word_cmp(file1, file2);
        } else if (args[1][0] == '-') {
            printf(RED "[-] Unsupported option: %s\n" RESET, args[1]);
            return EXIT_FAILURE;
        }
    } else {
        printf(WHT "[-] Usage:" RESET " word [-option] [file_name]");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// int main() {
//     char *cmd[] = {"./word", "-n", "./input.txt"};
//     wordRun(3, cmd);
// }