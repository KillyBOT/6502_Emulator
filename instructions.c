#include "6502.h"
#include <stdio.h>
#include <stdlib.h>

struct processor* initProcessor(){
  struct processor* p = malloc(sizeof(struct processor));
  p->mem = calloc(sizeof(reg_8), 0x10000);
  p->pCount = 0x800;
  p->stPtr = 0x00;
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

  free(buffer);

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
  printf("PC   A  X  Y  SP NV-BDIZC\n");
  printf("%.4X %.2X %.2X %.2X %.2X ", p->pCount, p->a, p->x, p->y, p->stPtr);

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
  reg_8 read_8; // Zero page and immediate
  reg_16 read_16;
  reg_8 toAdd;
  reg_16 sumTest;
  long cyclesToAdd;

  currentInstruction = *(p->mem + p->pCount);
  read_8 = *(p->mem + p->pCount + 1);
  read_16 = getFlipped(p->mem, 1, 2);
  toAdd = 2;
  cyclesToAdd = 2;

  printf("Current Instruction: ");

  switch(currentInstruction){
    case BRK_impl:
      printf("BRK_impl\n");

      setFlag(p, I, 1);

      break;
    case PHP_impl:
      printf("PHP_impl\n");

      *(p->mem + SYSTEM_STACK_BEGIN + p->stPtr) = p->status;
      p->stPtr += 1;

      cyclesToAdd = 3;

      break;
    default:
      printf("ERROR: Illegal opcode.\n");
      //setFlag(p, I, 1);
      return 0;
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

reg_8* getIndX(reg_8* mem, reg_8 a, reg_8 x){
  reg_16 address = getFlipped(mem + a + x, 0, 1);
  return mem + address;
}

reg_8* getIndY(reg_8* mem, reg_8 a, reg_8 y){
  reg_16 address = getFlipped(mem + a, 0, 1);
  return mem + address + y;
}

reg_8* getVal(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a){
  switch(a){
    case im:
      return read_8;
    case zpg:
      return mem + read_8;
    case zpgX:
      toAdd = *getZPGOffset(p->mem, read_8, p->x);
      break;
    case absN:
      toAdd = *getAbs(p->mem, read_16);
      break;
    case absX:
      toAdd = *getAbsOffset(p->mem, read_16, p->x);
      if(read_16 + p->x > 0xffff) p->cycles++;
      break;
    case absY:
      toAdd = *getAbsOffset(p->mem, read_16, p->y);
      if(read_16 + p->y > 0xffff) p->cycles++;
      break;
    case indX:
      toAdd = *getIndX(p->mem, read_16, p->x);
      break;
    case indY:
      toAdd = *getIndY(p->mem, read_16, p->y);
      if(read_16 + p->y > 0xffff) p->cycles++;
      break;
    default:
      printf("Unaccounted addressing mode\n");
      break;
  }
}

void setFlag(struct processor* p, reg_8 flag, reg_8 yOrN){
  if(yOrN) p->status |= flag;
  else if( (p->status & flag) == flag) p->status -= flag;
}



void adc(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a){
  reg_8 sign1 = p->a;
  reg_8 toAdd;

  switch(a){
    case im:
      toAdd = read_8;
      break;
    case zpg:
      toAdd = *getZPG(p->mem, read_8);
      break;
    case zpgX:
      toAdd = *getZPGOffset(p->mem, read_8, p->x);
      break;
    case absN:
      toAdd = *getAbs(p->mem, read_16);
      break;
    case absX:
      toAdd = *getAbsOffset(p->mem, read_16, p->x);
      if(read_16 + p->x > 0xffff) p->cycles++;
      break;
    case absY:
      toAdd = *getAbsOffset(p->mem, read_16, p->y);
      if(read_16 + p->y > 0xffff) p->cycles++;
      break;
    case indX:
      toAdd = *getIndX(p->mem, read_16, p->x);
      break;
    case indY:
      toAdd = *getIndY(p->mem, read_16, p->y);
      if(read_16 + p->y > 0xffff) p->cycles++;
      break;
    default:
      printf("Unaccounted addressing mode\n");
      break;
  }

  added = p->a + toAdd;
  p->a += toAdd;

  setFlag(p, C, added > 0xff);
  setFlag(p, Z, p->a == 0);
  setflag(p, V, (sign1 >> 7) || (toAdd >> 7) == (p->a >> 7));
  setFlag(p, N, (p->a & 0x80) == 0x80);

}
void and_(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a){
  reg_8 toAnd;

  switch(a){
    case im:
      toAnd = read_8;
    case zpg:
      toAnd =
    case zpgX:
      p->a &= *getZPGOffset(p->mem, read_8, p->x);
      break;
    case absN:
      p->a &= *getAbs(p->mem, read_16);
      break;
    case absX:
  }
}
void bit(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void cmp(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void cpx(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void cpy(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void dec(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void dex(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void dey(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void eor(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void inc(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void inx(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void iny(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void lda(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void ldx(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void ldy(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void lsr(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void ora(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void rol(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void ror(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void sbc(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void sta(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void stx(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
void sty(struct processor* p, reg_8 read_8, reg_16 read_16, enum addr_mode a);
