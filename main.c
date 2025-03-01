#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct context
{
  // memory of 4096 bytes (4 kB)
  unsigned char memory[4096];

  // data Registers (for V0-VF)
  unsigned char V[16];

  // stack with 16 slots
  unsigned short stack[16];

  // frame buffer of 64 x 32
  int graphics[64 * 32];

  // Special registers

  // address Register (only gets loaded with 12-bits)
  unsigned short I;
  // program counter
  unsigned short PC;
  // stack pointer
  unsigned short SP;
  // delay timer register
  unsigned char DTR;
  // sound timer register
  unsigned char STR;
  // keys array
  unsigned char keys[16];

} Context;

// function to place pre-designed fonts into memory from 050 - 09F (80 - 159 in decimal)
void setup_fonts(Context *context)
{
  // font of 0
  context->memory[0x050] = 0xF0;
  context->memory[0x051] = 0x90;
  context->memory[0x052] = 0x90;
  context->memory[0x053] = 0x90;
  context->memory[0x054] = 0xF0;

  // font of 1
  context->memory[0x055] = 0x20;
  context->memory[0x056] = 0x60;
  context->memory[0x057] = 0x20;
  context->memory[0x058] = 0x20;
  context->memory[0x059] = 0x70;

  // font of 2
  context->memory[0x05A] = 0xF0;
  context->memory[0x05B] = 0x10;
  context->memory[0x05C] = 0xF0;
  context->memory[0x05D] = 0x80;
  context->memory[0x05E] = 0xF0;

  // font of 3
  context->memory[0x05F] = 0xF0;
  context->memory[0x060] = 0x10;
  context->memory[0x061] = 0xF0;
  context->memory[0x062] = 0x10;
  context->memory[0x063] = 0xF0;

  // font of 4
  context->memory[0x064] = 0x90;
  context->memory[0x065] = 0x90;
  context->memory[0x066] = 0xF0;
  context->memory[0x067] = 0x10;
  context->memory[0x068] = 0x10;

  // font of 5
  context->memory[0x069] = 0xF0;
  context->memory[0x06A] = 0x80;
  context->memory[0x06B] = 0xF0;
  context->memory[0x06C] = 0x10;
  context->memory[0x06D] = 0xF0;

  // font of 6
  context->memory[0x06E] = 0xF0;
  context->memory[0x06F] = 0x80;
  context->memory[0x070] = 0xF0;
  context->memory[0x071] = 0x90;
  context->memory[0x072] = 0xF0;

  // font of 7
  context->memory[0x073] = 0xF0;
  context->memory[0x074] = 0x10;
  context->memory[0x075] = 0x20;
  context->memory[0x076] = 0x40;
  context->memory[0x077] = 0x40;

  // font of 8
  context->memory[0x078] = 0xF0;
  context->memory[0x079] = 0x90;
  context->memory[0x07A] = 0xF0;
  context->memory[0x07B] = 0x90;
  context->memory[0x07C] = 0xF0;

  // font of 9
  context->memory[0x07D] = 0xF0;
  context->memory[0x07E] = 0x90;
  context->memory[0x07F] = 0xF0;
  context->memory[0x080] = 0x10;
  context->memory[0x081] = 0xF0;

  // font of A
  context->memory[0x082] = 0xF0;
  context->memory[0x083] = 0x90;
  context->memory[0x084] = 0xF0;
  context->memory[0x085] = 0x90;
  context->memory[0x086]= 0x90;

  // font of B
  context->memory[0x087] = 0xE0;
  context->memory[0x088] = 0x90;
  context->memory[0x089] = 0xE0;
  context->memory[0x08A] = 0x90;
  context->memory[0x08B] = 0xE0;

  // font of C
  context->memory[0x08C] = 0xF0;
  context->memory[0x08D] = 0x80;
  context->memory[0x08E] = 0x80;
  context->memory[0x08F] = 0x80;
  context->memory[0x090] = 0xF0;

  // font of D
  context->memory[0x091] = 0xE0;
  context->memory[0x092] = 0x90;
  context->memory[0x093] = 0x90;
  context->memory[0x094] = 0x90;
  context->memory[0x095] = 0xE0;

  // font of E
  context->memory[0x096] = 0xF0;
  context->memory[0x097] = 0x80;
  context->memory[0x098] = 0xF0;
  context->memory[0x099] = 0x80;
  context->memory[0x09A] = 0xF0;

  // font of F
  context->memory[0x09B] = 0xF0;
  context->memory[0x09C] = 0x80;
  context->memory[0x09D] = 0xF0;
  context->memory[0x09E] = 0x80;
  context->memory[0x09F] = 0x80;

}

// Add target program into interpreter memory
int loadProgram(char *fileName, Context *context) {
  FILE *fp = fopen(fileName, "r");

  // Scan file contents into memory byte by byte
  if (fp == NULL) {
    printf("Error: File loaded in by CLI not found.\n");
    return -1;
  } else {
    char c;
    // start loading at 0x200, but don't modify PC
    int i = context->PC;
    while (fscanf(fp, "%c", &c) != EOF) {
      context->memory[i] = c;
      i++;
    }
  }

  fclose(fp);

  return 0;
}

// takes 2 bytes from memory, combines them to make a decodable instruction.
unsigned short fetch(Context *context) {

  unsigned short instruction;

  // some bitshifting to combine 2 bytes into 1 instruction
  instruction = (context->memory[context->PC] << 8) + context->memory[context->PC + 1]; 

  context->PC += 2;
  
  return instruction;
}

void decodeThenExec(unsigned short instruction, Context *context) {
  // extract nibbles from instruction
  unsigned char nibble1 = (instruction & 0xF000) >> 12;
  unsigned char nibble2 = (instruction & 0x0F00) >> 8;
  unsigned char nibble3 = (instruction & 0x00F0) >> 4;
  unsigned char nibble4 = instruction & 0x000F;

  // 3 + 4 nibbles combined (NN). Immediate 8-bit number for use.
  unsigned char NN = (instruction & 0x00FF);

  // 2 + 3 + 4 nibbles combined (NNN). Immediate 12-bit memory address for use.
  unsigned short NNN = (instruction & 0x0FFF);

  // break up cases by the first nibble
  switch(nibble1) {
  case 0x0:
    // 0x00E0: clear the screen pixel by pixel
    if (instruction == 0x00E0) {
      for (int i = 0; i < (64 * 32) - 1; i++) {
	context->graphics[i] = 0;
      }
    }
    
    break;

  case 0x1:
    // 0x1NNN: jumping to addr NNN
    context->PC = NNN;
    break;

  case 0x2:
    break;

  case 0x3:
    break;

  case 0x4:
    break;

  case 0x5:
    break;

  case 0x6:
    // 0x6XNN: set V[X] to NN
    context->V[nibble2] = NN;
    break;

  case 0x7:
    // 0x7XNN: adds value NN to immediate register V[X]
    context->V[nibble2] += NN;
    break;

  case 0x8:
    break;

  case 0x9:
    break;

  case 0xA:
    // ANNN: store NNN into I reg
    context->I = NNN;
    break;

  case 0xB:
    break;

  case 0xC:
    break;

  case 0xD:
    // DXYN: Draw sprite at position VX, VY with N
    // bytes of sprite data starting at address stored in I

    // VF is set to 0x1 if set pixels are changed, 0x0 otherwise

    // get coords
    unsigned char x = context->V[nibble2] % 64;
    unsigned char y = context->V[nibble3] % 32;

    // reset VF
    context->V[0xF] = 0;

    for (int row = 0; row < nibble4; row++) {
      // get Nth byte of sprite data
      unsigned char spriteRowData = context->memory[context->I + row];

      // check each bit of sprite row
      for (int bit = 0; bit < 8; bit++) {
	// bounds check
        if ((x + bit < 64) && (y + row < 32)) {
	  // get index of current bit
	  int i = (x + bit) + ((y + row) * 64);
	  // get screen pixel
	  unsigned char screenPix = context->graphics[i];
	  // get sprite pixel
	  unsigned char spritePix = (spriteRowData & (1 << (7 - bit))) != 0;

	  // comparison
	  if (screenPix & spritePix) {
	    // set VF to 1, screen pixel to off
	    context->V[0xF] = 1;
	    context->graphics[i] = 0x0;
	  }
	  if (!screenPix && spritePix == 1) {
	    context->graphics[i] = 0x1;
	  }
	}

      }
      
    }
    
    break;

  case 0xE:
    break;

  case 0xF:
    break;
  }
  
}
		    

int main(int argc, char* argv[]) {
  // Check for CLI file input
  if (argc != 2) {
    printf("Error: Run program as ./chip8 <filename>\n");
    return -1;
  }

  // Setup program context struct
  Context context;
  // Program counter starts at 200
  context.PC = 0x200;

  // load fonts in
  setup_fonts(&context);

  // load target program into memory
  if (loadProgram(argv[1], &context) != 0) {
    // fail early due to file error
    return -1;
  }

  // Setup window
  SDL_Window* window = NULL;

  // Window surface
  SDL_Surface* screenSurface = NULL;

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL failed to init: %s\n", SDL_GetError());
  } else {
    // Create the window (64 x 32)
    window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED, 640,
			      320, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // set background as black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_RenderClear(renderer);

    if (window == NULL) {
      printf("Window failed to create: %s\n", SDL_GetError());
    } else {

      // trick to get window to stay up
      SDL_Event e;
      int quit = 0;

      while (quit == 0) {

	while (SDL_PollEvent(&e)) {
	  if (e.type  == SDL_QUIT)
	    quit = 1;
	}
	// fetch instruction
	unsigned short instruction = fetch(&context);

	// send instruction for decode + execution
	decodeThenExec(instruction, &context);

	// only draw when instruction is 0xDXYN
	if (instruction >> 12 == 0xD) {
	  // redraw
	  for (int i = 0; i < 32; i++) {
	    for (int j = 0; j < 64; j++) {
	      if (context.graphics[(i * 64) + j] == 0x1) {
		// set rendered pixel to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	      } else {
		// set rendered pixel to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	      }

	      // setup rectangle
	      SDL_Rect r;
	      r.x = j * 10;
	      r.y = i * 10;
	      r.w = 10;
	      r.h = 10;
	    

	      SDL_RenderFillRect(renderer, &r);
	      SDL_RenderPresent(renderer);
	    }
	  }
	}

      }
    }
  }

  // Destroy the window
  SDL_DestroyWindow(window);

  // Quit SDL subsystems
  SDL_Quit();

  return 0;
}
