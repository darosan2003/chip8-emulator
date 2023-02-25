#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "chip8.h"

int main(int argc, char **argv) {

  if(argc != 2) {
    fprintf(stderr, "[-] ERROR. Did not specify a rom\n");
    fprintf(stderr, "[?] Usage: %s pong.ch8\n", argv[0]);
    return 1;
  }

  srand(time(NULL));

  chip8_t chip8;
  int quit = 0;

  initialize_chip(&chip8);
  load_rom(&chip8, argv[1]);

  for(int i=0; i<SCRN_SIZE; i++)
    chip8.screen[i] = rand() % 2;

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *win = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_OPENGL);
  SDL_Renderer *rnd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture *tex = SDL_CreateTexture(rnd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
  SDL_Event ev;

  uint32_t *pixels;
  int pitch;

  SDL_LockTexture(tex, NULL, (void **)&pixels, &pitch);
  conversion(chip8.screen, pixels);
  SDL_UnlockTexture(tex);

  while(!quit) {

    //step_forward(&chip8);

    SDL_RenderClear(rnd);
    SDL_RenderCopy(rnd, tex, NULL, NULL);
    SDL_RenderPresent(rnd);

    SDL_WaitEvent(&ev);
    if(ev.type == SDL_QUIT)
	    quit = 1;
  
  }

  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(rnd);
  SDL_DestroyWindow(win);

  SDL_Quit();

  return 0;

}
