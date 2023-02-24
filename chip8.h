#ifndef __CHIP8_H
#define __CHIP8_H

#include <stdint.h>

#define REG_NUM    16
#define MEM_SIZE   4096
#define STACK_SIZE 16

#ifndef __CHIP8__T
#define __CHIP8__T
typedef struct {
  uint8_t mem[MEM_SIZE];
  uint8_t v[REG_NUM];
  uint8_t dt, st;
  uint16_t i, pc, sp;
  uint16_t stack[STACK_SIZE];
} chip8_t;
#endif

void initialize_chip(chip8_t *);
void load_rom(chip8_t *, char *);
int process_opcode(chip8_t *, uint16_t);

#endif
