#include "6502.h"
#include <stdio.h>

void readInstructions(FILE* fp, reg_16 programPointerStart, reg_16* mem){
  char* buffer = malloc(sizeof(char));
  reg_16 currentPlace = programPointerStart;
  while(fread(buffer, sizeof(char), 1, fp) > 0){
    *(mem + currentPlace) = *buffer;
    currentPlace++;
  }

  printf("Program loaded!\n");
}

void memDump(reg_16* mem, reg_16 start, reg_16 size){
  for(reg_16 x = 0; x < size * 8; x+=8){
    printf("%x: ", start + x);
    for(int y = 0; y < 8; y++){
      printf("%.2X ", *(mem + y + start + x));
    }
    printf("\n");
  }
}

void printCycle(cycle* c){
  printf("PC   AC  X  Y  SR SP NV-BDIZC\n");
  printf("%.4X %.2X %.2x %.2X %.2X %.2X %.2X\n", c->pCount, c->a, c->X, c->Y, c->stReg, c->stPtr, c->status);
}
