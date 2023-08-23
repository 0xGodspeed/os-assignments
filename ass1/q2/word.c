#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#define IF_VERBOSE if (newline_bool)

void word(char *file, bool newline_bool)
{

    FILE *ptr;
    char ch;

    ptr = fopen(file, "r");

    if (NULL == ptr)
    {
        printf(RED "[-] Error" RESET "File doesnt exist");
    }

    int words = 0;
    int charr = 0;
    while ((ch = fgetc(ptr)) != EOF)
    {
        charr++;
        if (newline_bool)
        {
            if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0')
                words++;
        }
        else
        {
            if (ch == ' ' || ch == '\t')
            {
                words++;
            }
        }
    }

    if (charr > 0)
    {
        words++;
    }
    if (newline_bool)
    {
        printf("Total words: %d\n", words);
    }
    else
    {
        printf("Total words without newline: %d\n", words);
    }
}

void word_cmp(char *file1, char *file2)
{
    FILE *ptr;
    FILE *ptr2;
    char ch;

    ptr = fopen(file1, "r");
    ptr2 = fopen(file2, "r");

    if (NULL == ptr || NULL == ptr2)
    {
        printf(RED "[-] Error" RESET " File doesnt exist");
    }

    int words = 0;
    int words2 = 0;
    while ((ch = fgetc(ptr)) != EOF)
    {

        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0')
            words++;
    }
    while ((ch = fgetc(ptr2)) != EOF)
    {

        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0')
            words2++;
    }

    printf("Differnce of words: %d\n", abs(words-words2));
    
}

int main(int argc, char *argv[])
{
    bool newline_bool = true;
    char *file;
    if (argc == 2)
    {
        file = argv[1];
        word(file, newline_bool);
    }
    else if (argc == 3)
    {
        if (strcmp("-n", argv[1]) == 0)
        {
            newline_bool = false;
            file = argv[2];
            word(file, newline_bool);
        }
        if (strcmp("-d", argv[1]) == 0)
        { 
            printf(RED "[-] Usage:" RESET " word -d [file_name] [file_name2]");
            return EXIT_FAILURE;
        }
    }
    else if (argc == 4)
    {
        if (strcmp("-n", argv[1]) == 0)
        {
            printf(RED "[-] Usage:" RESET " word -n [file_name]");
        }
        if (strcmp("-d", argv[1]) == 0)
        {
            char *file1 = argv[2];
            char *file2 = argv[3];
            word_cmp(file1, file2);
        }
    }
    else
    {
        printf(RED "[-] Usage:" RESET " word [-option] [file_name]");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}