#include "6502.h"
#include <stdio.h>
#include <stdlib.h>

struct cycle* initCycle(){
  struct cycle* c = malloc(sizeof(struct cycle));
  c->mem = malloc(sizeof(reg_8) * (0x10000));
  c->pCount = 0x800;
  c->stPtr = 0x00;
  c->stReg = 0x00;
  c->a = 0x00;
  c->x = 0x00;
  c->y = 0x00;
  c->status = 0x00;

  return c;
}

void readInstructions(FILE* fp, reg_16 programPointerStart, reg_8* mem){
  reg_8* buffer = malloc(sizeof(reg_8));
  reg_16 currentPlace = programPointerStart;

  while(fread(buffer, sizeof(reg_8), 1, fp) > 0){
    *(mem + currentPlace) = *buffer;
    currentPlace++;
    //printf("%.2X\n", *buffer);
  }

  printf("Program loaded!\n");
}

void memDump(reg_8* mem, reg_16 start, reg_16 size){
  for(reg_16 x = 0; x < size * 8; x+=8){
    printf("%X: ", start + x);
    for(int y = 0; y < 8; y++){
      printf("%.2X ", *(mem + y + start + x));
    }
    printf("\n");
  }
}

void printCycle(struct cycle* c){
  printf("PC   A  X  Y  SR SP NV-BDIZC\n");
  printf("%.4X %.2X %.2x %.2X %.2X %.2X ", c->pCount, c->a, c->x, c->y, c->stReg, c->stPtr);

  for(int x = 7; x >= 0; x--) printf("%.1X", c->status >> x);
  printf("\n");
}

int doCycle(struct cycle* c){
  reg_8 currentInstruction;
  reg_8 zPAddress;
  reg_8 absoluteAddress;

  reg_8 toAdd = 2;

  currentInstruction = *(c->mem + c->pCount);


  switch(currentInstruction){
    case LDA_im:
      printf("LDA_im\n");
      c->a = *(c->mem + c->pCount + 1);
    default:
      printf("ERROR! You shouldn\'t be seeing this.\n");
      c->status ^= 16;
      return 0;
  }

  c->pCount += toAdd;

  return 1;
}

reg_16 getFlipped(struct cycle* c, reg_8 pos1, reg_8 pos2){
  reg_16 final = 0;
  reg_8 p1, p2;

  p1 = *(c->mem + pos1);
  p2 = *(c->mem + pos2);
  final = p2;
  final <<= 8;
  final ^= p1;
  return final;
}
