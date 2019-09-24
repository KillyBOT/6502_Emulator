/*
Help from:

https://www.masswerk.at/6502/6502_instruction_set.html#ORA
http://www.obelisk.me.uk/6502/index.html
*/

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
#define ORA_indX 0x01 //Or accumulator, indirect
#define ORA_zpg 0x05 //Or accumulator, zeroPage
#define ASL_zpg 0x06 //Arithmetic shift left, zeroPage
#define PHP_impl 0x08 //Push processor status onto stack, implicit
#define ORA_im 0x09 //Or accumulator, immediate
#define ASL_a 0x0a //Arithmetic shift left, accumulator
#define ORA_abs 0x0d //Or accumulator, absolute
#define ASL_abs 0x0e //Arithmetic shift left, absolute

#define BPL_rel 0x10 //Branch on plus, relative
#define ORA_indY 0x11 //Or accumulator, zpgY
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
#define ROL_zpgX 0x36 //Rotate left, zeroPage
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

#define BVC_rel 0x50 //Branch or overflow clear, relative
#define EOR_intY 0x51 //Exclusive or, indirectY
#define EOR_zpgX 0x55 //Exclusive or, zeroPageX
#define LSR_zpgX 0x56 //Logical shift right, zeroPageX
#define CLI_impl 0x58 //Clear interrupt disable, implicit
#define EOR_absY 0x59 //Exclusive or, absoluteY
#define EOR_absX 0x5d //Exclusive or, absoluteX
#define LSR_absX 0x5e //Logical shift right, absoluteX

#define RTS_impl 0x60 //return from subroutine, implicit
#define ADC_indX 0x61 //Add with carry, indirectX
#define ADC_zpg 0x65 //Add with carry, zeroPage
#define ROR_zpg 0x66 //Rotate right, zeroPage
#define PLA_impl 0x68 //Pull accumulator from stack, implicit
#define ADC_im 0x69 //Add with carry, immediate
#define ROR_a 0x6a //Right rotate, accumulator
#define JMP_ind 0x6c //Jump, indirect
#define ADC_abs 0x6d //Add with carry, absolute
#define ROR_abs 0x6e //Rotate right, absolute

#define BVS_rel 0x70 //Branch or overflow set, relative
#define ADC_indY 0x71 //Add with carry, indirectY
#define ADC_zpgX 0x75 //Add with carry, zeroPageX
#define ROR_zpgX 0x76 //Rotate right, zeroPageX
#define SEI_impl 0x78 //Set interrupt disable, implicit
#define ADC_absY 0x79 //Add with carry, absoluteY
#define ADC_absX 0x7d //Add with carry, absouluteX
#define ROR_absX 0x7e //Rotate right, absouluteX

#define STA_indX 0x81 //Store accumulator, indirectX
#define STY_zpg 0x84 //Store Y, zeroPage
#define STA_zpg 0x85 //Store accumulator, zeroPage
#define STX_zpg 0x86 //Store X, zeroPage
#define DEY_impl 0x88 //Decrement Y, implicit
#define TXA_impl 0x8a //Transfer X to accumulator, implicit
#define STY_abs 0x8c //Store Y, absolute
#define STA_abs 0x8d //Store accumulator, absolute
#define STX_abs 0x8e //Store X, absolute

#define BCC_rel 0x90 //Branch on carry clear, relative
#define STA_indY 0x91 //Store accumulator, indirectY
#define STY_zpgY 0x94 //Store Y, zeroPageY
#define STA_zpgX 0x95 //Store accumulator, zeroPageX
#define STA_zpgY 0x96 //Store accumulator, zeroPageY
#define TYA_impl 0x98 //Transfer Y to accumulator, implicit
#define STA_absY 0x99 //Store accumulator, absoluteY
#define TXS_impl 0x9a //Transfer X to stack pointer, implicit
#define STA_absX 0x9d //Store accumulator, absoluteX

#define LDY_im 0xa0 //Load Y, immediate
#define LDA_indX 0xa1 //Load accumulator, indirectX
#define LDX_im 0xa2 //Load X, immediate
#define LDY_zpg 0xa4 //Load Y, zeroPage
#define LDA_zpg 0xa5 //Load accumulator, zeroPage
#define LDX_zpg 0xa6 //Load X, zeroPage
#define TAY_impl 0xa8 //Transfer accumulator to Y, implicit
#define LDA_im 0xa9 //Load accumulator, immediate
#define TAX_impl 0xaa //Transfer accumulator to X, implicit
#define LDY_abs 0xac //Load y, absolute
#define LDA_abs 0xad //Load accumulator, absolute
#define LDX_abs 0xae //Load X, absolute

#define BCS_rel 0xb0 //Branch on clear, relative
#define LDA_indY 0xb1 //Load accumulator, indirectY
#define LDY_zpgX 0xb4 //Load Y, zeroPageX
#define LDA_zpgX 0xb5 //Load accumulator, zeroPageX
#define LDX_zpgY 0xb6 //Load X, zeroPgeY
#define CLV_impl 0xb8 //Clear overflow, implicit
#define LDA_absY 0xb9 //Load accumulator, absoluteY
#define TSX_impl 0xba //Transfer stack pointer to X, implicit
#define LDY_absX 0xbc //Load Y, absoulteX
#define LDA_absX 0xbd //Load accumulator, absouluteX
#define LDX_absY 0xbe //Load X, absoluteY

#define CPY_im 0xc0 //Compare to Y, immediate
#define CMP_indX 0xc1 //Compare to accumulator, indirectX
#define CPY_zpg 0xc4 //Compare to Y, zeroPage
#define CMP_zpg 0xc5 //Compare to accumulator, zeroPage
#define DEC_zpg 0xc6 //Decrement, zeroPage
#define INY_impl 0xc8 //Increment Y, implicit
#define CMP_im 0xc9 //Compare to accumulator, immediate
#define DEX_impl 0xca //Decrement X, implicit
#define CPY_abs 0xcc //Compare to Y, absolute
#define CMP_abs 0xcd //Compare to accumulator, absolute
#define DEC_abs 0xce //Decrement, absolute

#define BNE_rel 0xd0 //Branch on not equal, relative
#define CMP_indY 0xd1 //Compare to accumulator, indirectY
#define CMP_zpgX 0xd5 //Compare to accumulator, zeroPageX
#define DEC_zpgX 0xd6 //Decrement, zeroPageX
#define CLD_impl 0xd8 //Clear decimal, implicit
#define CMP_absY 0xd9 //Compare to accumulator, absoluteY
#define CMP_absX 0xdd //Compare to accumulator, absoluteX
#define DEC_absX 0xde //Decrement, absoluteX

#define CPX_im 0xe0 //Compare to X, immediate
#define SBC_indX 0xe1 //Subtract with carry, indirectX
#define CPX_zpg 0xe4 //Compare to X, zeroPage
#define SBC_zpg 0xe5 //Subtract with carry, zeroPage
#define INC_zpg 0xe6 //Increment, zeroPage
#define INX_impl 0xe8 //Increment X, implicit
#define SBC_im 0xe9 //Subtract with carry, immediate
#define NOP_impl 0xea //No operation, implicit
#define CPX_abs 0xec //Compare with X, absolute
#define SBC_abs 0xed //Subtract with carry, absolute
#define INC_abs 0xee //Increment, absolute

#define BEQ_rel 0xf0 //Branch if equal, relative
#define SBC_indY 0xf1 //Subtract with carry, indirectY
#define SBC_zpgX 0xf5 //Subtract with carry, zeroPageX
#define INC_zpgX 0xf6 //Increment, zeroPageX
#define SED_impl 0xf8 //Set decimal, implicit
#define SBC_absY 0xf9 //Subtract with carry, absoluteY
#define SBC_absX 0xfd //Subtract with carry, absoluteY
#define INC_absX 0xfe //Increment, absoluteX

typedef short reg_16;
typedef char reg_8;

//enum add_modes {implicit, accumulator, immediate, zeroPage, zeroPageX, zeroPageY, relative, absolute, absoluteX, absoluteY, indirect, indexedIndirect, indirectIndexed};

typedef struct {
  reg_8* mem; //Memory
  reg_16 pCount; //Program counter
  reg_8 stPtr; //Stack pointer
  reg_8 stReg; //Status register
  reg_8 a; //Accumulator
  reg_8 X; //Register X
  reg_8 Y; //Register Y
  reg_8 status; //Processor status
  /*
  Bit 7: Negative (N)
  Bit 6: Overflow (V)
  Bit 4: Break (B)
  Bit 3: Decimal (D)
  Bit 2: Interrupt (I)
  Bit 1: Zero (Z)
  Bit 0: Carry (C)
  */
  reg_8 stackPointer;
} cycle;

void readInstructions(FILE* fp, reg_16 programPointerStart, reg_16* mem);
void memDump(reg_16* memory, reg_16 start, reg_16 size);
int doCycle(cycle *currentCycle);
