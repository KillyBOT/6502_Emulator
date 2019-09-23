#include "6502.h"

int main(int argc, char** argv){

  FILE* p;
  int pS;
  char* currentInstruction; // Current instruction
  int testEnd;

  reg_16* mem = malloc(sizeof(reg_16) * (0x10000));

  if(argc > 1)p = fopen(argv[1],"r");
  else p = fopen("testProgram.txt","r");

  printf("File loaded!\n");
  readInstruction(p);

  printf("Finished\n");

  //fseek(program, 0, SEEK_END);
  //programSize = ftell(program);
  //fseek(program, 0, SEEK_SET);

  //printf("%d\n", programSize);


  fclose(p);
}
