**Chip8 Emulator Written in C**
By: Paul Warwick

INTRO:

This project is meant to get my feet wet in the realm of emulator programming. Before tackling
my already began project of the GameBoy fully, I'm following the recommendation of many 
emulator devs who say this is a good starting point for this field. 


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
640 x 320 pixels for more powerful modern monitors.


**Sources:**

CHIP‐8 Technical Reference
https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference

CHIP‐8 Instruction Set
https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set

High-Level Guide to Making a Chip-8 Emulator by Tobias V. Langhoff
https://tobiasvl.github.io/blog/write-a-chip-8-emulator/