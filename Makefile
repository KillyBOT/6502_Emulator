ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: main.o instructions.o
	$(CC) -o 6502Emulator main.o instructions.o

test: testLinkedList.o 

main.o: main.c 6502.h
	$(CC) -c main.c

instructions.o: instructions.c 6502.h
	$(CC) -c instructions.c

clean:
	rm *.o

memcheck:
	valgrind --leak-check=yes ./6502Emulator
