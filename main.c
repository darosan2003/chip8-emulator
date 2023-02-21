#include <stdio.h>
#include "chip8.h"

int main(int argc, char **argv) {

  if(argc != 2) {
    fprintf(stderr, "[-] ERROR. Did not specify a rom\n");
    fprintf(stderr, "[?] Usage: %s pong.ch8\n", argv[0]);
    return 1;
  }

  chip8_t chip8;
  int exit = 0;

  initialize_chip(&chip8);
  load_rom(&chip8, argv[1]);

  while(!exit) {

    if(chip8.pc == MEM_SIZE || chip8.pc + 1 == MEM_SIZE)
      chip8.pc = 0;

    uint16_t opcode = (chip8.mem[chip8.pc] << 8) | chip8.mem[chip8.pc + 1];
    exit = process_opcode(opcode);

    chip8.pc += 2;

  }

  return 0;

}
