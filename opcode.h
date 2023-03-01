#ifndef __OPCODE_
#define __OPCODE_

#include "chip8.h"

#define OPCODE_FUNCS 16

void _0_opcodes(chip8_t *, uint16_t);
void _1_opcodes(chip8_t *, uint16_t);
void _2_opcodes(chip8_t *, uint16_t);
void _3_opcodes(chip8_t *, uint16_t);
void _4_opcodes(chip8_t *, uint16_t);
void _5_opcodes(chip8_t *, uint16_t);
void _6_opcodes(chip8_t *, uint16_t);
void _7_opcodes(chip8_t *, uint16_t);
void _8_opcodes(chip8_t *, uint16_t);
void _9_opcodes(chip8_t *, uint16_t);
void _A_opcodes(chip8_t *, uint16_t);
void _B_opcodes(chip8_t *, uint16_t);
void _C_opcodes(chip8_t *, uint16_t);
void _D_opcodes(chip8_t *, uint16_t);
void _E_opcodes(chip8_t *, uint16_t);
void _F_opcodes(chip8_t *, uint16_t);

#endif
