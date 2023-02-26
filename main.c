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
  int last_ticks = 0;

  initialize_chip(&chip8);
  load_rom(&chip8, argv[1]);

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *win = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  SDL_Renderer *rnd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture *tex = SDL_CreateTexture(rnd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
  SDL_Surface *sur = SDL_CreateRGBSurface(0, TEXTURE_WIDTH, TEXTURE_HEIGHT, SURFACE_DEPTH, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  SDL_Event ev;

  int cycles = 0;

  SDL_LockTexture(tex, NULL, &sur->pixels, &sur->pitch);
  conversion(chip8.screen, (uint32_t *)sur->pixels);
  SDL_UnlockTexture(tex);

  while(!quit) {

    while(SDL_PollEvent(&ev)) {
      switch(ev.type) {
        case SDL_QUIT:
	  quit = 1;
	  break;
      }
    }

    if(SDL_GetTicks() - cycles > 1) {
      if(chip8.wait_key == -1)
        step_forward(&chip8);
      else {
        for(int i=0; i<=0x0F; i++) {
	  if(key_pressed(i)) {
	    chip8.v[(int)chip8.wait_key] = i;
	    chip8.wait_key = -1;
	    break;
	  }
	}
      }
      cycles = SDL_GetTicks();
    }

    if(SDL_GetTicks() - last_ticks > (1000 / 60)) {
      if(chip8.dt) chip8.dt--;
      if(chip8.st) chip8.st--;

      SDL_LockTexture(tex, NULL, &sur->pixels, &sur->pitch);
      conversion(chip8.screen, (uint32_t *)sur->pixels);
      SDL_UnlockTexture(tex);

      //SDL_RenderClear(rnd);
      SDL_RenderCopy(rnd, tex, NULL, NULL);
      SDL_RenderPresent(rnd);
      last_ticks = SDL_GetTicks();
    }

  }

  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(rnd);
  SDL_DestroyWindow(win);

  SDL_Quit();

  return 0;

}
