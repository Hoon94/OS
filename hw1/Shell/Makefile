CC = g++

SHELL: Shell.o Stat.o Getenv.o cd.o
	$(CC) -o SHELL Shell.o Stat.o Getenv.o cd.o
Shell.o: Shell.c
	$(CC) -c Shell.c
Stat.o: Stat.c
	$(CC) -c Stat.c
Getenv.o: Getenv.c
	$(CC) -c Getenv.c
cd.o: cd.c
	$(CC) -c cd.c
clean:
	rm -rf SHELL Shell.o Stat.o Getenv.o cd.o
