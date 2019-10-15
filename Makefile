all: main.o instructions.o
	gcc -o 6502Emulator main.o instructions.o

main.o: main.c 6502.h
	gcc -c main.c

instructions.o: instructions.c 6502.h
	gcc -c instructions.c

clean:
	rm *.o
