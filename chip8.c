#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "stack.h"

void initialize_chip(chip8_t *chip8) {
  
  chip8->i  = 0x00;
  chip8->dt = 0x00;
  chip8->st = 0x00;
  chip8->sp = 0x00;
  chip8->pc = 0x200;

  for(int i=0; i<REG_NUM; i++)
    chip8->v[i] = 0x00;

  for(int i=0; i<MEM_SIZE; i++)
    chip8->mem[i] = 0x00;
 
  if(chip8->stack != NULL) {
    while(!empty_stack(chip8->stack))
      delete_stack(chip8->stack);
  }else {
    if((chip8->stack = malloc(sizeof(stack_t))) == NULL) {
      printf("[-] ERROR. Unable to create stack\n");
      exit(1);
    }
    create_stack(chip8->stack);
  }

}

void load_rom(chip8_t *chip8) {

  FILE *fp;
  int file_size;

  if((fp = fopen("snake.ch8", "r")) == NULL) {
  	fprintf(stderr, "[-] ERROR. Could not find ROM\n");
  	exit(2);
  }

  fseek(fp, 0, SEEK_END);
  file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  fread(chip8->mem + 0x200, file_size, 1, fp);
  fclose(fp);

}

int process_opcode(uint16_t opcode) {

  uint8_t msd  = opcode >> 12;
  uint8_t x    = (opcode >> 8) & 0x0F;
  uint8_t y    = (opcode >> 4) & 0x00F;
  uint8_t kk   = opcode & 0x00FF;
  uint8_t lsd  = opcode & 0x000F;
  uint16_t nnn = opcode & 0x0FFF;

  switch(msd) {
    case 0x00:
      if(opcode == 0x00E0)
        printf("CLS\n");
      else if(opcode == 0x00EE)
        printf("RET\n");
      break;

    case 0x01:
      printf("JP %x\n", nnn);
      break;

    case 0x02:
      printf("CALL %x\n", nnn);
      break;

    case 0x03:
      printf("SE (if v[%x] == %x)\n", x, kk);
      break;

    case 0x04:
      printf("SNE (if v[%x] != %x)\n", x, kk);
      break;

    case 0x05:
      printf("SE v[%x] v[%x]\n", x, y);
      break;

    case 0x06:
      printf("LD v[%x] %x\n", x, kk);
      break;

    case 0x07:
      printf("ADD v[%x] %x\n", x, kk);
      break;

    case 0x08:
      switch(lsd) {
        case 0x00:
          printf("LD v[%x] v[%x]\n", x, y);
          break;

        case 0x01:
          printf("OR v[%x] v[%x]\n", x, y);
          break;

        case 0x02:
          printf("AND v[%x] v[%x]\n", x, y);
          break;

        case 0x03:
          printf("XOR v[%x] v[%x]\n", x, y);
          break;

        case 0x04:
          printf("ADD v[%x] v[%x]\n", x, y);
          break;

        case 0x05:
          printf("SUB v[%x] v[%x]\n", x, y);
          break;

        case 0x06:
          printf("SHR v[%x]\n", x);
          break;

        case 0x07:
          printf("SUBN v[%x] v[%x]\n", x, y);
          break;

        case 0x0E:
          printf("SHL v[%x]\n", x);
          break;
      }
      break;

      case 0x09:
        printf("SNE v[%x] v[%x]\n", x, y);
        break;

      case 0x0A:
        printf("LD i %x\n", nnn);
        break;

      case 0x0B:
        printf("JP %x + v[0]\n", nnn);
        break;

      case 0x0C:
        printf("RND v[%x] %x (v[%x] = RND | %x)\n", x, kk, x, kk);
        break;

      case 0x0D:
        printf("DRW v[%x] v[%x] %x\n", x, y, lsd);
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
            printf("LD v[%x] dt\n", x);
            break;

          case 0x0A:
            printf("LD v[%x] k (wait until key pressed)\n", x);
            break;

          case 0x15:
            printf("LD dt v[%x]\n", x);
            break;

          case 0x18:
            printf("LD st v[%x]\n", x);
            break;

          case 0x1E:
            printf("ADD i v[%x]\n", x);
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

  return 0;

}
