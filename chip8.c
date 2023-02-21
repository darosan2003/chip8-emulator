#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

void initialize_chip(chip8_t *chip8) {
  
  chip8->i  = 0x00;
  chip8->dt = 0x00;
  chip8->st = 0x00;
  chip8->sp = 0x00;
  chip8->pc = 0x200;

  for(int i=0; i<REG_NUM; i++)
    chip8->v[i] = 0x00;

  for(int i=0; i<MEM_SIZE; i++)
    chip8->mem[i] = 0x00;
 
  if(chip8->stack != NULL) {
    while(!empty_stack(chip8->stack))
      delete_stack(chip8->stack);
  }else {
    if((chip8->stack = malloc(sizeof(stack_t))) == NULL) {
      printf("[-] ERROR. Unable to create stack\n");
      exit(1);
    }
  }

}
