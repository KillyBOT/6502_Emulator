all: main.o instructions.o
	gcc -o 6502Emulator -g main.o instructions.o

main.o: main.c 6502.h
	gcc -c -g main.c

instructions.o: instructions.c 6502.h
	gcc -c -g instructions.c

clean:
	rm *.o
