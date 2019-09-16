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

//Opcodes, hopefully all this work pays off

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

#define BMI_rel 0x30 //Branch on minus, relative
#define AND_indY 0x31 //And, indirectX
#define AND_zpgX 0x35 //And, zeroPageX
#define ROL_zpg 0x36 //Rotate left, zeroPage
#define SEC_impl 0x38 //Set carry, implicit
#define AND_absY 0x39 //And, absoluteY
#define AND_absX 0x3d //And, absoluteX
#define ROL_absX 0x3e //Rotate left, absoluteX

#define RTI_impl 0x40 //Return from interrupt, implicit
#define EOR_indX 0x41 //Exclusive or, indirectX
#define EOR_zpg 0x45 //Exclusive or, zeroPage
#define LSR_zpg 0x46 //Logical shift right, zeroPage
#define PHA_impl 0x48 //Push accumulator onto stack, implicit
#define EOR_im 0x49 //Exclusive or, immediate
#define LSR_a 0x4a //Logical shift right, accumulator
#define JMP_abs 0x4c//Jump, absolute
#define EOR_abs 0x4d //Exclusive or, absolute
#define LSR_abs 0x4e //Logical shift right, absolute

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
