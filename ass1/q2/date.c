#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#define IF_VERBOSE if (STRiNG)

void date(char *file, bool STRiNG)
{

    struct stat filestat;
    stat(file, &filestat);
    if (STRiNG)
    {
        printf("File modify time %s",
               ctime(&filestat.st_mtime));
    }
    else
    {
        char rfc5322Time[100];
        struct tm *timeInfo = gmtime(&filestat.st_mtime);
        strftime(rfc5322Time, sizeof(rfc5322Time), "%a, %d %b %Y %H:%M:%S %z", timeInfo);

        printf("Last modified time in RFC 5322 format: %s\n", rfc5322Time);
    }


    
}

int main(int argc, char *argv[])
{
    bool STRiNG = false;
    char *file;
    if (argc < 2)
    {
        printf(RED "[-] Usage:" RESET "date [-option] [file_name]");
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-d", argv[i]) == 0)
        {
            STRiNG = true;
        }
        if (strcmp("-R", argv[i]) == 0)
        {
            STRiNG = false;
        }
        else
        {
            file = argv[i];
        }
    }
    if (file == NULL)
    {
        printf(RED "[-] Usage:" RESET "date [-option] [file_name]");
        return EXIT_FAILURE;
    }
    date(file, STRiNG);
    return EXIT_SUCCESS;
}