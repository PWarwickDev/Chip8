**Chip8 Emulator Written in C**
By: Paul Warwick

INTRO:

This project is meant to get my feet wet in the realm of emulator programming. Before tackling
my already began project of the GameBoy fully, I'm following the recommendation of many 
emulator devs who say this is a good starting point for this field.

3/23/2025 Update: Mostly complete with some minor flag errors to be worked out in the future if I want to. Overall this project was quite interesting, and made me get back into low-level binary manipulation in a challenging way.


**MEMORY**

- 4096 byte storage (4 kB) can be stored with an array of 4096 bytes.

- Stack of 16 unsigned shorts for holding fetched instructions.

- Memory which is loaded from the target program is placed at 0x200 and
beyond, before that is reserved for the Chip-8 interpreter program.

- The fonts are stored from 0x050 upwards in the memory by a standard
convention. Includes a mapped way of graphically displaying 0-9 and A-F.


**PROCESSING**

- Fetch, decode, execute is the cycle in which the program is emulated.
  - Fetch in an instruction from memory (2 bytes long).
  - Decode using specified instructions set by original Chip-8 implementations.
  - Execute those instructions.


**GRAPHICS**

- Using SDL to display the 64 x 32 pixel display, but enlarged the screen to be
640 x 320 pixels for more powerful modern monitors. Off is black, on is white.

- The main instruction for changing pixel colors is 0xDXYN. In my implementation
of this instruction it goes as follows:
   - Get x-pos from V[0xX] register, and y-pos from V[0xY] register.
   - Reset the register V[0xF] to zero.
   - From row 0 to row N, get the Nth byte of that sprite row data using the
   current value of the I register. This data is then compared to current screen
   data.
   - In another loop from 0-7 bits of a sprite row, first do a bounds-check to
   see if data should even be written. Then determine the index of the pixel to
   be compared in the graphics array in the program context struct. The screen pixel
   is then compared to the current sprite pixel which has been bitwise AND'd out of
   the current sprite row.
   - First check if screen pixel is the same as the sprite pixel using bitwise AND,
   if they are, then V[0xF] is set to 1, and the current screen pixel is set to 0.
   - Then check if screen pixel is 0x0, while the sprite pixel is 0x1. If so, current
   screen pixel is set to 0x1.

**INPUT**

- SDL deals with keyboard events of keyup and keydown. 16 keys [1,2,3,4,q,w,e,r,t,
a,s,d,f,z,x,c,v] are mapped into keys for the chip-8 emulator.

**SOURCES**

CHIP‐8 Technical Reference
https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference

CHIP‐8 Instruction Set
https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set

High-Level Guide to Making a Chip-8 Emulator by Tobias V. Langhoff
https://tobiasvl.github.io/blog/write-a-chip-8-emulator/