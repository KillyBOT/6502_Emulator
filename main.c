#include "6502.h"

int main(int argc, char** argv){

  FILE* pgm;
  int pS;
  reg_16 programPointerStart;
  reg_8* programPointerStart1 = malloc(sizeof(reg_8));
  reg_8* programPointerStart2 = malloc(sizeof(reg_8));//First 2, then 1. 6502 is little endian
  int testEnd;
  int steps;
  struct processor *p;

  p = initProcessor();

  if(argc > 1)pgm = fopen(argv[1],"r");
  else pgm = fopen("testProgram.o","r");

  //Get the program pointer start

  programPointerStart = 0;
  fread(programPointerStart2, sizeof(reg_8), 1, pgm);
  fread(programPointerStart1, sizeof(reg_8), 1, pgm);
  programPointerStart = *programPointerStart1;
  programPointerStart <<= 8;
  programPointerStart ^= *programPointerStart2;

  free(programPointerStart1);
  free(programPointerStart2);

  printf("Pointer Start: %.4X\n", programPointerStart);

  p->pCount = programPointerStart;

  //Read the rest of the instructions
  readInstructions(pgm, programPointerStart, p->mem);

  printf("File loaded!\n");
  memDump(p->mem, programPointerStart, 2);
  printProcessor(p);

  printf("Starting program\n");

  while (doCycle(p));

  printf("Finished\n");

  //fseek(program, 0, SEEK_END);
  //programSize = ftell(program);
  //fseek(program, 0, SEEK_SET);

  //printf("%d\n", programSize);


  fclose(pgm);
  free(p->mem);
  free(p);
}
