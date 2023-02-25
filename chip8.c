#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8.h"

void conversion(char *scr, uint32_t *pixels) {

  for(int i=0; i<SCRN_SIZE; i++)
    pixels[i] = scr[i] ? -1 : 0;

}

void initialize_chip(chip8_t *chip8) {
  
  chip8->i  = 0x00;
  chip8->dt = 0x00;
  chip8->st = 0x00;
  chip8->sp = 0x00;
  chip8->pc = 0x200;

  memset(chip8->v, 0x00, sizeof(chip8->v));
  memset(chip8->stack, 0x00, sizeof(chip8->stack));
  memset(chip8->mem, 0x00, sizeof(chip8->mem));
  memset(chip8->screen, 0x00, sizeof(chip8->screen));

}

void load_rom(chip8_t *chip8, char *rom_name) {

  FILE *fp;
  int file_size;

  if((fp = fopen(rom_name, "r")) == NULL) {
    fprintf(stderr, "[-] ERROR. Could not find ROM\n");
    exit(2);
  }

  fseek(fp, 0, SEEK_END);
  file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  fread(chip8->mem + 0x200, file_size, 1, fp);
  fclose(fp);

}

void step_forward(chip8_t *chip8) {

  chip8->pc = (chip8->pc + 2) % MEM_SIZE;
	
  uint16_t opcode = (chip8->mem[chip8->pc] << 8 | chip8->mem[chip8->pc + 1]);
  process_opcode(chip8, opcode);

}

void process_opcode(chip8_t *chip8, uint16_t opcode) {

  uint8_t msd  = opcode >> 12;
  uint8_t x    = (opcode >> 8) & 0x0F;
  uint8_t y    = (opcode >> 4) & 0x00F;
  uint8_t kk   = opcode & 0x00FF;
  uint8_t lsd  = opcode & 0x000F;
  uint16_t nnn = opcode & 0x0FFF;

  switch(msd) {
    case 0x00:
      if(opcode == 0x00E0)
	memset(chip8->screen, 0x00, SCRN_SIZE);
      else if(opcode == 0x00EE) {
	if(chip8->sp > 0)
          chip8->pc = chip8->stack[--chip8->sp];
      }
      break;

    case 0x01:
      chip8->pc = nnn;
      break;

    case 0x02:
      if(chip8->sp < STACK_SIZE - 1)
      	chip8->stack[chip8->sp++] = chip8->pc;
      chip8->pc = nnn;
      break;

    case 0x03:
      if(chip8->v[x] == kk)
        chip8->pc = (chip8->pc + 2) % MEM_SIZE;
      break;

    case 0x04:
      if(chip8->v[x] != kk)
        chip8->pc = (chip8->pc + 2) % MEM_SIZE;
      break;

    case 0x05:
      if(chip8->v[x] == chip8->v[y])
        chip8->pc = (chip8->pc + 2) % MEM_SIZE;
      break;

    case 0x06:
      chip8->v[x] = kk;
      break;

    case 0x07:
      chip8->v[x] = (chip8->v[x] + kk) % UINT8_MAX;
      break;

    case 0x08:
      switch(lsd) {
        case 0x00:
          chip8->v[x] = chip8->v[y];
          break;

        case 0x01:
          chip8->v[x] |= chip8->v[y];
          break;

        case 0x02:
          chip8->v[x] &= chip8->v[y];
          break;

        case 0x03:
          chip8->v[x] ^= chip8->v[y];
          break;

        case 0x04:
          chip8->v[0x0F] = (chip8->v[x] > chip8->v[x] + chip8->v[y]);
          chip8->v[x] += chip8->v[y];
          break;

        case 0x05:
          chip8->v[0x0F] = (chip8->v[x] > chip8->v[y]);
          chip8->v[x] -= chip8->v[y];
          break;

        case 0x06:
          chip8->v[0x0F] = (chip8->v[x] & 0x01);
          chip8->v[x] >>= 1;
          break;

        case 0x07:
          chip8->v[0x0F] = (chip8->v[y] > chip8->v[x]);
          chip8->v[y] -= chip8->v[x];
          break;

        case 0x0E: 
          chip8->v[0x0F] = (chip8->v[x] & (1 << 7)) ? 1 : 0;
          chip8->v[x] <<= 1;
          break;
      }
      break;

      case 0x09:
        if(chip8->v[x] != chip8->v[y])
          chip8->pc = (chip8->pc + 2) % MEM_SIZE;
        break;

      case 0x0A:
        chip8->i = nnn;
        break;

      case 0x0B:
        chip8->pc = (chip8->pc + chip8->v[0x00] + nnn) % MEM_SIZE;
        break;

      case 0x0C:
	chip8->v[x] = rand() & kk;
        break;

      case 0x0D:
        for(int i=0; i<lsd; i++) {
	  uint8_t sprite = chip8->mem[chip8->i];
	  for(int j=0; j<7; j++) {
	    int px = (chip8->v[x] + j) & (TEXTURE_WIDTH - 1);
	    int py = (chip8->v[y] + i) & (TEXTURE_HEIGHT - 1);
	    chip8->screen[TEXTURE_WIDTH * py + px] = (sprite & (1 << (7 - j))) != 0;
	  }
	}
        break;

      case 0x0E:
        switch(kk) {
          case 0x9E:
            printf("SKP v[%x] (if key = v[%x])\n", x, x);
            break;

          case 0xA1:
            printf("SKNP v[%x] (if key != v[%x])\n", x, x);
        }
        break;

      case 0x0F:
        switch(kk) {
          case 0x07:
            chip8->v[x] = chip8->dt;
            break;

          case 0x0A:
            printf("LD v[%x] k (wait until key pressed)\n", x);
            break;

          case 0x15:
            chip8->dt = chip8->v[x];
            break;

          case 0x18:
            chip8->st = chip8->v[x];
            break;

          case 0x1E:
            chip8->i += chip8->v[x];
            break;

          case 0x29:
            printf("LD f v[%x]\n", x);
            break;

          case 0x33:
            printf("LD b v[%x]\n", x);
            break;

          case 0x55:
            printf("LD [i] v[%x]\n", x);
            break;

          case 0x65:
            printf("LD v[%x] [i]\n", x);
            break;
        }
        break;
  }

}
