#include <stdio.h>
#include <stdlib.h>

//Structure for registers

#define ZERO_PAGE_START 0x0000
#define ZERO_PAGE_END 0x00ff

#define SYSTEM_STACK_BEGIN 0x0100
#define SYSTEM_STACK_END 0x01ff

#define INTERRUPER_HANDLER 0xfffa
#define POWER_RESET 0xfffc
#define INTERRUPT_REQUEST_HANDLER 0xfffe

//Opcodes

#define BRK_impl 0x00 //Break, implicit
#define ORA_ind 0x01 //Or accumulator, indirect
#define ORA_zpg 0x05 //Or accumulator, zeroPage
#define ASL_zpg 0x06 //Arithmetic shift left, zeroPage
#define PHP_impl 0x08 //Push processor status onto stack, implicit
#define ORA_im 0x09 //Or accumulator, immediate
#define ASL_a 0x0a //Arithmetic shift left, accumulator
#define ORA_abs 0x0d //Or accumulator, absolute
#define ASL_abs 0x0e //Arithmetic shift left, absolute

#define BPL_rel 0x10 //Branch on plus, relative
#define ORA_ind 0x11 //Or accumulator, zpgY
#define ORA_zpgX 0x15 //Or accumulator, zpgX
#define ASL_zpgX 0x16 //Arithmetic shift left, zeroPageX
#define CLC_impl 0x18 //Clear carry, implicit
#define ORA_absY 0x19 //Or accumulator, absoluteY
#define ORA_absX 0x1d //Or accumulator, absoluteX
#define ASL_absX 0x1e //Arithmetic shift left, absoluteX

#define JSR_abs 0x20 //Jump subroutine, absolute
#define AND_indX 0x21 //And, indirectX
#define BIT_zpg 0x24 //Bit test, zeroPage
#define AND_zpg 0x25 //And, zeroPage
#define ROL_zpg 0x26 //Rotate left, zeroPage
#define PLP_impl 0x28 //Pull processor from stack, implicit
#define AND_im 0x29 //And, immediate
#define ROL_a 0x2a //Rotate left, accumulator
#define BIT_abs 0x2c //Bit test, absolute
#define AND_abs 0x2d //And, absolute
#define ROL_abs 0x2e //Rotate left, absolute

typedef short address;
typedef char reg;

//enum add_modes {implicit, accumulator, immediate, zeroPage, zeroPageX, zeroPageY, relative, absolute, absoluteX, absoluteY, indirect, indexedIndirect, indirectIndexed};



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
  address* mem; //Memory
  address pCount; //Program counter
  reg stPtr; //Stack pointer
  reg acc; //Accumulator
  reg rX; //Index register X
  reg rY; //Index register Y
  pStatus status; //Processor status
  //reg status; //Processor status
} cycle;
