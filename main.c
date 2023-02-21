#include <stdio.h>
#include "chip8.h"

int main(int argc, char **argv) {

  chip8_t chip8;

  initialize_chip(&chip8);
  load_rom(&chip8);

  return 0;

}
