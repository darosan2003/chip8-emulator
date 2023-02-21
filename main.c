#include <stdio.h>
#include "chip8.h"

int main(int argc, char **argv) {

  chip8_t chip8;
  int exit = 0;

  initialize_chip(&chip8);
  load_rom(&chip8);

  while(!exit) {

    if(chip8.pc == MEM_SIZE || chip8.pc + 1 == MEM_SIZE)
      chip8.pc = 0;

    uint16_t opcode = (chip8.mem[chip8.pc] << 8) | chip8.mem[chip8.pc + 1];
    exit = process_opcode(opcode);

    chip8.pc += 2;

  }

  return 0;

}
