#include "6502.h"

int main(int argc, char** argv){

  FILE* p;
  int pS;
  char* currentInstruction; // Current instruction
  int testEnd;

  reg_16* mem = malloc(sizeof(reg_8) * (0x10000));

  if(argc > 1)p = fopen(argv[1],"r");
  else p = fopen("testProgram.txt","r");

  printf("File loaded!\n");
  readInstructions(p, 0x0800, mem);
  memDump(mem, 0x0800, 2);

  printf("Finished\n");

  //fseek(program, 0, SEEK_END);
  //programSize = ftell(program);
  //fseek(program, 0, SEEK_SET);

  //printf("%d\n", programSize);


  fclose(p);
}
