#include "6502.h"

int main(int argc, char** argv){

  FILE* p;
  int pS;
  char* currentInstruction; // Current instruction
  int testEnd;
  struct cycle *c;

  c = initCycle();

  if(argc > 1)p = fopen(argv[1],"r");
  else p = fopen("testProgram.o","r");

  printf("File loaded!\n");
  readInstructions(p, 0x0800, c->mem);
  memDump(c->mem, 0x0800, 2);
  printCycle(c);

  printf("Finished\n");

  //fseek(program, 0, SEEK_END);
  //programSize = ftell(program);
  //fseek(program, 0, SEEK_SET);

  //printf("%d\n", programSize);


  fclose(p);
}
