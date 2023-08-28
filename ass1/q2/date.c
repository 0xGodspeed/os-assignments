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

void date(char *file, bool STRiNG, bool rfc, char *thestring)
{

    struct stat filestat;

    if (stat(file, &filestat) != 0)
    {
        printf(RED "[-] Error" RESET "File doesnt exist\n");
        return;
    }
    if (STRiNG)
    {
        struct tm tm_info;

        if (strptime(thestring, "%Y-%m-%d %H:%M:%S", &tm_info) == NULL)
        {
            printf("Invalid date format\n");
            return;
        }

        // Print the parsed date and time
        char formattedTime[100];
        strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", &tm_info);
        printf("Parsed Date and Time: %s\n", formattedTime);
    }
    else if (rfc)
    {
        char rfc5322Time[100];
        struct tm *timeInfo = gmtime(&filestat.st_mtime);
        strftime(rfc5322Time, sizeof(rfc5322Time), "%a, %d %b %Y %H:%M:%S %z", timeInfo);
        printf("Last modified time in RFC 5322 format: %s\n", rfc5322Time);
    }
    else
    {
        printf("File modify time %s",
               ctime(&filestat.st_mtime));
    }
}

int main(int argc, char *argv[])
{
    bool STRiNG = false;
    bool rfc = false;
    char *file;
    char *thestring;
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
        else if (strcmp("-R", argv[i]) == 0)
        {
            rfc = true;
        }
        else if (STRiNG && i == 2)
        {
            thestring = argv[i];
        }
        else if (argv[i][0] == '-')
        {
            printf(RED "[-] Unsupported option: %s\n" RESET, argv[i]);
            return EXIT_FAILURE;
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
    date(file, STRiNG, rfc, thestring);
    return EXIT_SUCCESS;
}