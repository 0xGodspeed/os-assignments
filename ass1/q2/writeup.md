Solution is split in 5 files.
One file is for making a shell like interface (csh.c) and other are for word, date, dir and Makefile

# 1. Shell (csh.c)
- fork for external commands with different PID
- for internal commands, the files are included and linked with csh.c
- Run the interface in loop 
- `strtok()` is used to break the shell inputs into tokens 

# 2. Date 
- for -d we have implemented the following strings:
"yesterday"
"tomorrow"
"1 month ago"
"2 months ago"
- used this for rfc5322 format `strftime(rfc5322Time, sizeof(rfc5322Time), "%a, %d %b %Y %H:%M:%S %z", timeInfo);`
- filestat.st_mtime for last modified time

# 3. Dir
- Basic error checking
- Used `mkdir()` to create directory and `chdir()` to change directory
- Used `ntfw()` from `ftw.h` (file tree walk) to implement the `-r` option. It recursively goes through all the directories and files and calls a callback function `rmrf()` which deletes each file.

# 4. Word
- word is defined in word.c and is linked with csh.c
- A simple loop that counts the number of words using the "\n", " ", "\0" seperators 
- Simple subtraction for -d option
- `-n` is implemented using `strtok()` which delimits all the words in the file by a space ' ' character. If a `\n` character is found using `strchr()`, the word is ignored, otherwise it is ccounted

# 5. Makefile
- All the binaries are compiled to the `./bin` directory and its path is added to the `PATH` variable in the Makefile
- `csh.c` is compiled with `word.c`
- `make run` can be called to run the shell
- `make clean` can be called to clean and delete the binaries