
Solution is split in 5 files.
one file is for making a shell like interface and other are for word date dir and last is for makefile

# 1. Shell like interface
- fork for external commands with different PID
- for internal commands run it in the same file by calling it.
- Run the interface in loop 

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
- used mkdir to create directory 


# 4. Word
- A simple loop that counts the number of words using the "\n", " ", "\0" seperators 
- Simple subtraction for -d option
- -n 

# 5. Makefile
- Added commands to path using export 
- compling and running all files using it 