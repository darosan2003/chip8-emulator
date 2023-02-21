#ifndef __CHIP8_H
#define __CHIP8_H

#include <stdint.h>
#include "stack.h"

#define REG_NUM    16
#define MEM_SIZE   4096

#ifndef __CHIP8__T
#define __CHIP8__T
typedef struct {
  uint8_t mem[MEM_SIZE];
  uint8_t v[REG_NUM];
  uint8_t dt, st;
  uint16_t i, pc, sp;
  stack_t stack;
} chip8_t;
#endif

#endif
