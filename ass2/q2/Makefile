CC = gcc
all:
	$(CC) schedule.c -o schedule
	$(CC) count.c -o count
clean:
	rm -f schedule
	rm -f count
run:
	taskset --cpu-list 1 "./schedule"
plot:
	python3 plot.py
