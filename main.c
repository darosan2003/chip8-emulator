#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"

int main(int argc, char **argv) {

  if(argc != 2) {
    fprintf(stderr, "[-] ERROR. Did not specify a rom\n");
    fprintf(stderr, "[?] Usage: %s pong.ch8\n", argv[0]);
    return 1;
  }

  chip8_t chip8;

  initialize_chip(&chip8);
  load_rom(&chip8, argv[1]);

  while(1) {

    step_forward(&chip8);
  
  }

  return 0;

}
