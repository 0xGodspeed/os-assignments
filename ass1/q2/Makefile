# export PATH:=/home/gs/code/clg/os/ass1/q2/bin:$(PATH)
export PATH:=$(PWD)/bin:$(PATH)
CC=gcc
all:
	mkdir -p bin
	$(CC) csh.c word.c -o ./bin/csh
	$(CC) dir.c -o ./bin/dir
	$(CC) date.c -o ./bin/date

run:
	./bin/csh  

clean:
	rm -rf bin