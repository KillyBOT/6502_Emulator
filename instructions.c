#include "6502.h"
#include <stdio.h>

int readInstruction(FILE* fp){
  char* ins = malloc(sizeof(char));

  if(fread(ins, sizeof(char), 1, fp) < sizeof(char)) return -1;

  
  return 0;
}
