#include <stdio.h>
#include <stdlib.h>

//Structure for registers

#define ZERO_PAGE_START 0x0000
#define ZERO_PAGE_END 0x00ff

#define INTERRUPER_HANDLER 0xfffa
#define POWER_RESET 0xfffc
#define INTERRUPT_REQUEST_HANDLER 0xfffe

typedef short address;
typedef char reg;

typedef struct {
  int cFlag : 1; //Carry flag
  int zFlag : 1; //Zero flag
  int intDisable : 1; //Interrupter disabler
  int dMode : 1; //Decimal mode
  int bCommand : 1; //Break command
  int oFlag : 1; //Overflow flag
  int nFlag : 1; //Negative flag
} pStatus; //Processor status

typedef struct {
  address pCount; //Program counter
  reg stPtr; //Stack pointer
  reg acc; //Accumulator
  reg rX; //Index register X
  reg rY; //Index register Y
  pStatus status; //Processor status
  //reg status; //Processor status
} cycle;
