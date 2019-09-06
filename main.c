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
  
} registers;

int main(){
  printf("Finished\n");
  return 0;
}
