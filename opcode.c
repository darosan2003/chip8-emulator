#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8.h"
#include "opcode.h"

void _0_opcodes(chip8_t *chip8, uint16_t opcode) {

  // CLS 
  if(opcode == 0x00E0)
    memset(chip8->screen, 0x00, SCRN_SIZE);
  // RET
  else if(opcode == 0x00EE) {
    if(chip8->sp > 0)
      chip8->pc = chip8->stack[--chip8->sp];
  }
      
}

void _1_opcodes(chip8_t *chip8, uint16_t opcode) {

  // JP nnn
  chip8->pc = (opcode & 0x0FFF);

}

void _2_opcodes(chip8_t *chip8, uint16_t opcode) {

  // CALL nnn
  if(chip8->sp < STACK_SIZE)
    chip8->stack[chip8->sp++] = chip8->pc;
  chip8->pc = (opcode & 0x0FFF);

}

void _3_opcodes(chip8_t *chip8, uint16_t opcode) {

  // SE Vx, kk
  if(chip8->v[(opcode >> 8) & 0x0F] == (opcode & 0x00FF))
    chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);

}

void _4_opcodes(chip8_t *chip8, uint16_t opcode) {

  // SNE Vx, kk
  if(chip8->v[(opcode >> 8) & 0x0F] != (opcode & 0x00FF))
    chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);

}

void _5_opcodes(chip8_t *chip8, uint16_t opcode) {

  // SE Vx, Vy
  if(chip8->v[(opcode >> 8) & 0x0F] == chip8->v[(opcode >> 4) & 0x00F])
    chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);

}

void _6_opcodes(chip8_t *chip8, uint16_t opcode) {

  // LD Vx, Vy
  chip8->v[(opcode >> 8) & 0x0F] = (opcode & 0x00FF);

}

void _7_opcodes(chip8_t *chip8, uint16_t opcode) {

  // ADD Vx, kk
  chip8->v[(opcode >> 8) & 0x0F] = (chip8->v[(opcode >> 8) & 0x0F] + (opcode & 0xFF)) & 0xFF;

}

void _8_opcodes(chip8_t *chip8, uint16_t opcode) {

  switch(opcode & 0x000F) {
    case 0x00:
      // LD Vx, Vy
      chip8->v[(opcode >> 8) & 0x0F] = chip8->v[(opcode >> 4) & 0x00F];
      break;

    case 0x01:
      // OR Vx, Vy
      chip8->v[(opcode >> 8) & 0x0F] |= chip8->v[(opcode >> 4) & 0x00F];
      break;

    case 0x02:
      // AND Vx, Vy
      chip8->v[(opcode >> 8) & 0x0F] &= chip8->v[(opcode >> 4) & 0x00F];
      break;

    case 0x03:
      // XOR Vx, Vy
      chip8->v[(opcode >> 8) & 0x0F] ^= chip8->v[(opcode >> 4) & 0x00F];
      break;

    case 0x04:
      // ADD Vx, Vy
      chip8->v[0x0F] = (chip8->v[(opcode >> 8) & 0x0F] > chip8->v[(opcode >> 8) & 0x0F] + chip8->v[(opcode >> 4) & 0x00F]);
      chip8->v[(opcode >> 8) & 0x0F] += chip8->v[(opcode >> 4) & 0x00F];
      break;

    case 0x05:
      // SUB Vx, Vy
      chip8->v[0x0F] = (chip8->v[(opcode >> 8) & 0x0F] > chip8->v[(opcode >> 4) & 0x0F]);
      chip8->v[(opcode >> 8) & 0x0F] -= chip8->v[(opcode >> 4) & 0x00F];
      break;

    case 0x06:
      // SHR Vx {, Vy}
      chip8->v[0x0F] = (chip8->v[(opcode >> 8) & 0x0F] & 1);
      chip8->v[(opcode >> 8) & 0x0F] >>= 1;
      break;

    case 0x07:
      // SUBN Vx, Vy
      chip8->v[0x0F] = (chip8->v[(opcode >> 4) & 0x00F] > chip8->v[(opcode >> 8) & 0x0F]);
      chip8->v[(opcode >> 8) & 0x0F] = chip8->v[(opcode >> 4) & 0x00F] - chip8->v[(opcode >> 8) & 0x0F];
      break;

    case 0x0E:
      // SHL Vx {, Vy}
      chip8->v[0x0F] = ((chip8->v[(opcode >> 8) & 0x0F] & 0x80) != 0);
      chip8->v[(opcode >> 8) & 0x0F] <<= 1;
      break;
  }

}

void _9_opcodes(chip8_t *chip8, uint16_t opcode) {

  // SNE Vx, Vy
  if(chip8->v[(opcode >> 8) & 0x0F] != chip8->v[(opcode >> 4) & 0x00F])
    chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);

}

void _A_opcodes(chip8_t *chip8, uint16_t opcode) {

  // LD I, nnn
  chip8->i = (opcode & 0x0FFF);

}

void _B_opcodes(chip8_t *chip8, uint16_t opcode) {

  // JP V0, nnn
  chip8->pc = (chip8->v[0x00] + (opcode & 0x0FFF)) & (MEM_SIZE - 1);

}

void _C_opcodes(chip8_t *chip8, uint16_t opcode) {

  // RND Vx, kk
  chip8->v[(opcode >> 8) & 0x0F] = rand() & (opcode & 0x00FF);

}

void _D_opcodes(chip8_t *chip8, uint16_t opcode) {

  // DRW Vx, Vy, lsd
  chip8->v[0x0F] = 0;
  for(int i=0; i<(opcode & 0x000F); i++) {
    int sprite = chip8->mem[chip8->i + i];
    for(int j=0; j<8; j++) {
      int px = (chip8->v[(opcode >> 8) & 0x0F] + j) & (TEXTURE_WIDTH - 1);
      int py = (chip8->v[(opcode >> 4) & 0x00F] + i) & (TEXTURE_HEIGHT - 1);
      int pos = TEXTURE_WIDTH * py + px;
      int pixel = (sprite & (1 << (7 - j))) != 0;

      chip8->v[0x0F] |= (chip8->screen[pos] & pixel);
      chip8->screen[pos] ^= pixel;
	  }
	}

}

void _E_opcodes(chip8_t *chip8, uint16_t opcode) {

  switch(opcode & 0x00FF) {
    case 0x9E:
      // SKP Vx
      if(key_pressed(chip8->v[(opcode >> 8) & 0x0F]))
        chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
      break;

    case 0xA1:
      // SKNP Vx
      if(!key_pressed(chip8->v[(opcode >> 8) & 0x0F]))
        chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
      break;
	}

}

void _F_opcodes(chip8_t *chip8, uint16_t opcode) {

  switch(opcode & 0x00FF) {
    case 0x07:
      // LD Vx, DT
      chip8->v[(opcode >> 8) & 0x0F] = chip8->dt;
    break;

    case 0x0A:
      // LD Vx, K
      chip8->wait_key = (opcode >> 8) & 0x0F;
    break;

    case 0x15:
      // LD DT, Vx
      chip8->dt = chip8->v[(opcode >> 8) & 0x0F];
    break;

    case 0x18:
      // LD ST, Vx
      chip8->st = chip8->v[(opcode >> 8) & 0x0F];
    break;

    case 0x1E:
      // ADD I, Vx
      chip8->i += chip8->v[(opcode >> 8) & 0x0F];
    break;

    case 0x29:
      // LD F, Vx
      chip8->i = 0x50 + (chip8->v[(opcode >> 8) & 0x0F] & 0x0F) * 5;
    break;

    case 0x33:
      // LD B, Vx
      chip8->mem[chip8->i + 2] = chip8->v[(opcode >> 8) & 0x0F] % 10;
      chip8->mem[chip8->i + 1] = (chip8->v[(opcode >> 8) & 0x0F] / 10) % 10;
      chip8->mem[chip8->i] = chip8->v[(opcode >> 8) & 0x0F] / 100;
    break;

    case 0x55:
      // LD [I], Vx
      for(uint16_t i=0; i<=((opcode >> 8) & 0x0F); i++)
        chip8->mem[chip8->i + i] = chip8->v[i];
      break;

    case 0x65:
      // LD Vx, [I]
      for(uint16_t i=0; i<=((opcode >> 8) & 0x0F); i++)
        chip8->v[i] = chip8->mem[chip8->i + i];
      break;
  }

}
