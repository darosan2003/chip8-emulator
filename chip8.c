#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "chip8.h"

char keys[] = {
  SDL_SCANCODE_X,
  SDL_SCANCODE_1,
  SDL_SCANCODE_2,
  SDL_SCANCODE_3,
  SDL_SCANCODE_Q,
  SDL_SCANCODE_W,
  SDL_SCANCODE_E,
  SDL_SCANCODE_A,
  SDL_SCANCODE_S,
  SDL_SCANCODE_D,
  SDL_SCANCODE_Z,
  SDL_SCANCODE_C,
  SDL_SCANCODE_4,
  SDL_SCANCODE_R,
  SDL_SCANCODE_F,
  SDL_SCANCODE_V
};

char number_hexcodes[] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0,
  0x20, 0x60, 0x20, 0x20, 0x70,
  0xF0, 0x10, 0xF0, 0x80, 0xF0,
  0xF0, 0x10, 0xF0, 0x10, 0xF0,
  0x90, 0x90, 0xF0, 0x10, 0x10,
  0xF0, 0x80, 0xF0, 0x10, 0xF0,
  0xF0, 0x80, 0xF0, 0x90, 0xF0,
  0xF0, 0x10, 0x20, 0x40, 0x40,
  0xF0, 0x90, 0xF0, 0x90, 0xF0,
  0xF0, 0x90, 0xF0, 0x10, 0xF0,
  0xF0, 0x90, 0xF0, 0x90, 0x90,
  0xE0, 0x90, 0xE0, 0x90, 0xE0,
  0xF0, 0x80, 0x80, 0x80, 0xF0,
  0xE0, 0x90, 0x90, 0x90, 0xE0,
  0xF0, 0x80, 0xF0, 0x80, 0xF0,
  0xF0, 0x80, 0xF0, 0x80, 0x80
};

int key_pressed(char key) {

  const uint8_t *keyboard = SDL_GetKeyboardState(NULL);
  uint8_t real_key = keys[(int)key];
  
  return keyboard[real_key];

}

void conversion(char *scr, uint32_t *pixels) {

  for(int i=0; i<SCRN_SIZE; i++)
    pixels[i] = (scr[i]) ? -1 : 0;

}

void initialize_chip(chip8_t *chip8) {
 
  memset(chip8, 0x00, sizeof(chip8_t));
  memcpy(chip8->mem + 0x50, number_hexcodes, sizeof(number_hexcodes));
  chip8->pc = 0x200;
  chip8->wait_key = -1;

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

  uint16_t opcode = (chip8->mem[chip8->pc] << 8) | chip8->mem[chip8->pc + 1];
  chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
 
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
      if(chip8->sp < STACK_SIZE)
      	chip8->stack[chip8->sp++] = chip8->pc;
      chip8->pc = nnn;
      break;

    case 0x03:
      if(chip8->v[x] == kk)
        chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
      break;

    case 0x04:
      if(chip8->v[x] != kk)
        chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
      break;

    case 0x05:
      if(chip8->v[x] == chip8->v[y])
        chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
      break;

    case 0x06:
      chip8->v[x] = kk;
      break;

    case 0x07:
      chip8->v[x] = (chip8->v[x] + kk) & 0xff;
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
          chip8->v[0x0F] = (chip8->v[x] & 1);
          chip8->v[x] >>= 1;
          break;

        case 0x07:
          chip8->v[0x0F] = (chip8->v[y] > chip8->v[x]);
          chip8->v[x] = chip8->v[y] - chip8->v[x];
          break;

        case 0x0E: 
          chip8->v[0x0F] = ((chip8->v[x] & 0x80) != 0);
	  chip8->v[x] <<= 1;
          break;
      }
      break;

      case 0x09:
        if(chip8->v[x] != chip8->v[y])
          chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
        break;

      case 0x0A:
        chip8->i = nnn;
        break;

      case 0x0B:
        chip8->pc = (chip8->v[0x00] + nnn) & (MEM_SIZE - 1);
        break;

      case 0x0C:
	chip8->v[x] = rand() & kk;
        break;

      case 0x0D:
        chip8->v[0x0F] = 0;
	for(int i=0; i<lsd; i++) {
	  uint8_t sprite = chip8->mem[chip8->i + i];
	  for(int j=0; j<8; j++) {
	    int px = (chip8->v[x] + j) & (TEXTURE_WIDTH - 1);
	    int py = (chip8->v[y] + i) & (TEXTURE_HEIGHT - 1);
            int pos = TEXTURE_WIDTH * py + px;
	    int pixel = (sprite & (1 << (7 - j))) != 0;

	    chip8->v[0x0F] |= (chip8->screen[pos] & pixel);
	    chip8->screen[pos] ^= pixel;
	  }
	}
        break;

      case 0x0E:
        switch(kk) {
          case 0x9E:
            if(key_pressed(chip8->v[x]))
              chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
            break;

          case 0xA1:
            if(!key_pressed(chip8->v[x]))
	      chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
            break;
	}
        break;

      case 0x0F:
        switch(kk) {
          case 0x07:
            chip8->v[x] = chip8->dt;
            break;

          case 0x0A:
	    chip8->wait_key = x;
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
            chip8->i = 0x50 + (chip8->v[x] & 0x0F) * 5;
            break;

          case 0x33:
	    chip8->mem[chip8->i + 2] = chip8->v[x] % 10;
            chip8->mem[chip8->i + 1] = (chip8->v[x] / 10) % 10;
	    chip8->mem[chip8->i] = chip8->v[x] / 100;
	    break;

          case 0x55:
	    for(int i=0; i<=x; i++)
	      chip8->mem[chip8->i + i] = chip8->v[i];
            break;

          case 0x65:
	    for(int i=0; i<=x; i++)
	      chip8->v[i] = chip8->mem[chip8->i + i];
            break;
        }
        break;
  }

}
