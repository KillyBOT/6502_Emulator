#include "6502.h"
#include <stdio.h>
#include <stdlib.h>

struct processor* initProcessor(){
  struct processor* p = malloc(sizeof(struct processor));
  p->mem = calloc(sizeof(reg_8), 0x10000);
  p->pCount = 0x800;
  p->stPtr = 0x00;
  p->stReg = 0x00;
  p->a = 0x00;
  p->x = 0x00;
  p->y = 0x00;
  p->status = 0x00;
  p->cycles = 0;

  return p;
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

void printProcessor(struct processor* p){
  printf("PC   A  X  Y  SR SP NV-BDIZC\n");
  printf("%.4X %.2X %.2x %.2X %.2X %.2X ", p->pCount, p->a, p->x, p->y, p->stReg, p->stPtr);

  for(int x = 7; x >= 0; x--) printf("%.1X", (p->status >> x) & 1);
  printf("\n");
  printf("Cycles: %ld\n", p->cycles);
}

int doCycle(struct processor* p){

  printProcessor(p);
  printf("\n");

  //Check flags

  if((p->status & 4) == 4){
    printf("Program interrupted\n");
    return 0;
  }

  reg_8 currentInstruction;
  reg_8 zPAddress; // Also can be the immediate too!
  reg_16 absAddress;
  reg_8 toAdd;
  reg_16 sumTest;
  long cyclesToAdd;

  currentInstruction = *(p->mem + p->pCount);
  zPAddress = *(p->mem + p->pCount + 1);
  absAddress = getFlipped(p->mem, 1, 2);
  toAdd = 2;
  cyclesToAdd = 2;

  printf("Current Instruction: ");

  switch(currentInstruction){
    case BRK_impl:
      printf("BRK_impl\n");

      setFlag(p, I, 1);

      break;
    case ORA_indX:
      printf("ORA_indX\n");

      zPAddress += p->x;

      p->a |=

      cyclesToAdd = 6;

      break;
    case LDA_im:
      printf("LDA_im\n");

      break;
    case STA_zpg:
      printf("STA_zpg\n");

      *getZPG(p->mem, zPAddress) = p->a;

      cyclesToAdd = 3;

      break;
    case ADC_zpg:
      printf("ADC_zpg\n");

      sumTest = p->a + *getZPG(p->mem, zPAddress);

      setFlag(p, C, sumTest > 0xFF);
      setFlag(p, Z, sumTest == 0x00);
      setFlag(p, N, (sumTest & 0x80) == 0x80);
      printf("%.2X %.2X %.2X\n", (p->a & 0x80) | (*getZPG(p->mem, zPAddress) & 0x80), (sumTest & 0x80), ( (p->a & 0x80) | (*getZPG(p->mem, zPAddress) & 0x80) ) != (sumTest & 0x80));
      setFlag(p, V, ( (p->a & 0x80) | (*getZPG(p->mem, zPAddress) & 0x80) ) != (sumTest & 0x80));

      p->a += *getZPG(p->mem, zPAddress);

      cyclesToAdd = 3;

      break;
    default:
      printf("ERROR! You shouldn\'t be seeing this.\n");
      setFlag(p, I, 1);
      break;
  }

  p->pCount += toAdd;
  p->cycles += cyclesToAdd;

  printf("\n");

  return 1;
}

reg_16 getFlipped(reg_8* mem, reg_8 pos1, reg_8 pos2){
  reg_16 final = 0;
  reg_8 p1, p2;

  p1 = *(mem + pos1);
  p2 = *(mem + pos2);
  final = p2;
  final <<= 8;
  final ^= p1;
  return final;
}

reg_8* getZPG(reg_8* mem, reg_8 a){
  return mem + a;
}

reg_8* getZPGOffset(reg_8* mem, reg_8 a, reg_8 o){
  return mem + a + o;
}

reg_8* getAbs(reg_8* mem, reg_16 a){
  return mem + a;
}

reg_8* getAbsOffset(reg_8* mem, reg_16 a, reg_8 o){
  return mem + a + o;
}

void setFlag(struct processor* p, reg_8 flag, reg_8 yOrN){
  if(yOrN) p->status |= flag;
  else if((p->status & flag) == flag) p->status -= flag;
  else 
}
