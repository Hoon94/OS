CC = gcc

Thread: Thread.o
	$(CC) -o Thread Thread.o char_stat.o
Thread.o: Thread.c
	$(CC) -c Thread.c
char_stat.o: char_stat.c
	$(CC) -c char_stat.c
clean:
	rm -rf Thread Thread.o char_stat.o
