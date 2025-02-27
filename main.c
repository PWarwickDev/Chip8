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
	int matrix[64][32];

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
void setup_fonts(Context context)
{
	// font of 0
	context.memory[0x050] = 0xF0;
	context.memory[0x051] = 0x90;
	context.memory[0x052] = 0x90;
	context.memory[0x053] = 0x90;
	context.memory[0x054] = 0xF0;

	// font of 1
	context.memory[0x055] = 0x20;
	context.memory[0x056] = 0x60;
	context.memory[0x057] = 0x20;
	context.memory[0x058] = 0x20;
	context.memory[0x059] = 0x70;

	// font of 2
	context.memory[0x05A] = 0xF0;
	context.memory[0x05B] = 0x10;
	context.memory[0x05C] = 0xF0;
	context.memory[0x05D] = 0x80;
	context.memory[0x05E] = 0xF0;

	// font of 3
	context.memory[0x05F] = 0xF0;
	context.memory[0x060] = 0x10;
	context.memory[0x061] = 0xF0;
	context.memory[0x062] = 0x10;
	context.memory[0x063] = 0xF0;

	// font of 4
	context.memory[0x064] = 0x90;
	context.memory[0x065] = 0x90;
	context.memory[0x066] = 0xF0;
	context.memory[0x067] = 0x10;
	context.memory[0x068] = 0x10;

	// font of 5
	context.memory[0x069] = 0xF0;
	context.memory[0x06A] = 0x80;
	context.memory[0x06B] = 0xF0;
	context.memory[0x06C] = 0x10;
	context.memory[0x06D] = 0xF0;

	// font of 6
	context.memory[0x06E] = 0xF0;
	context.memory[0x06F] = 0x80;
	context.memory[0x070] = 0xF0;
	context.memory[0x071] = 0x90;
	context.memory[0x072] = 0xF0;

	// font of 7
	context.memory[0x073] = 0xF0;
	context.memory[0x074] = 0x10;
	context.memory[0x075] = 0x20;
	context.memory[0x076] = 0x40;
	context.memory[0x077] = 0x40;

	// font of 8
	context.memory[0x078] = 0xF0;
	context.memory[0x079] = 0x90;
	context.memory[0x07A] = 0xF0;
	context.memory[0x07B] = 0x90;
	context.memory[0x07C] = 0xF0;

	// font of 9
	context.memory[0x07D] = 0xF0;
	context.memory[0x07E] = 0x90;
	context.memory[0x07F] = 0xF0;
	context.memory[0x080] = 0x10;
	context.memory[0x081] = 0xF0;

	// font of A
	context.memory[0x082] = 0xF0;
	context.memory[0x083] = 0x90;
	context.memory[0x084] = 0xF0;
	context.memory[0x085] = 0x90;
	context.memory[0x086]= 0x90;

	// font of B
	context.memory[0x087] = 0xE0;
	context.memory[0x088] = 0x90;
	context.memory[0x089] = 0xE0;
	context.memory[0x08A] = 0x90;
	context.memory[0x08B] = 0xE0;

	// font of C
	context.memory[0x08C] = 0xF0;
	context.memory[0x08D] = 0x80;
	context.memory[0x08E] = 0x80;
	context.memory[0x08F] = 0x80;
	context.memory[0x090] = 0xF0;

	// font of D
	context.memory[0x091] = 0xE0;
	context.memory[0x092] = 0x90;
	context.memory[0x093] = 0x90;
	context.memory[0x094] = 0x90;
	context.memory[0x095] = 0xE0;

	// font of E
	context.memory[0x096] = 0xF0;
	context.memory[0x097] = 0x80;
	context.memory[0x098] = 0xF0;
	context.memory[0x099] = 0x80;
	context.memory[0x09A] = 0xF0;

	// font of F
	context.memory[0x09B] = 0xF0;
	context.memory[0x09C] = 0x80;
	context.memory[0x09D] = 0xF0;
	context.memory[0x09E] = 0x80;
	context.memory[0x09F] = 0x80;

}

int main(int argc, char* argv[]) {
	Context context;

	setup_fonts(context);

	// Setup window
	SDL_Window* window = NULL;

	// Window surface
	SDL_Surface* screenSurface = NULL;

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	  printf("SDL failed to init: %s\n", SDL_GetError());
	} else {
	  // Create the window (64 x 32)
	  window = SDL_CreateWindow("Basic Window", SDL_WINDOWPOS_UNDEFINED,
				    SDL_WINDOWPOS_UNDEFINED, 640,
				    320, SDL_WINDOW_SHOWN);

	  if (window == NULL) {
	    printf("Window failed to create: %s\n", SDL_GetError());
	  } else {
	    // Get window surface
	    screenSurface = SDL_GetWindowSurface(window);

	    // Make it just white
	    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format,
							 0xFF, 0xFF, 0xFF));

	    // Update surface
	    SDL_UpdateWindowSurface(window);

	    // trick to get window to stay up
	    SDL_Event e;
	    int quit = 0;
	    while (quit == 0) {
	      while (SDL_PollEvent(&e)) {
		if (e.type  == SDL_QUIT)
		  quit = 1;
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
