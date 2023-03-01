#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "opcode.h"

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

  if(file_size > MEM_SIZE - 0x200) {
    fprintf(stderr, "[-] Error. ROM size cannot exceed %d bytes\n", MEM_SIZE - 0x200);
    exit(3);
  }

  fread(chip8->mem + 0x200, file_size, 1, fp);
  fclose(fp);

}

void step_forward(chip8_t *chip8) {

  uint16_t opcode = (chip8->mem[chip8->pc] << 8) | chip8->mem[chip8->pc + 1];
  chip8->pc = (chip8->pc + 2) & (MEM_SIZE - 1);
 
  process_opcode(chip8, opcode);

}

void process_opcode(chip8_t *chip8, uint16_t opcode) {

  void (*opcode_func[OPCODE_FUNCS])(chip8_t *, uint16_t) = {_0_opcodes, _1_opcodes, _2_opcodes, _3_opcodes,
                                                            _4_opcodes, _5_opcodes, _6_opcodes, _7_opcodes,
                                                            _8_opcodes, _9_opcodes, _A_opcodes, _B_opcodes,
                                                            _C_opcodes, _D_opcodes, _E_opcodes, _F_opcodes};

  opcode_func[opcode >> 12](chip8, opcode);

}
