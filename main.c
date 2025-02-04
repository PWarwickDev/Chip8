#include <stdio.h>
#include <stdint.h>

typedef struct Register
{
	// Data Registers

	int8_t V0;
	int8_t V1;
	int8_t V2;
	int8_t V3;
	int8_t V4;
	int8_t V5;
	int8_t V6;
	int8_t V7;
	int8_t V8;
	int8_t V9;
	int8_t VA;
	int8_t VB;
	int8_t VC;
	int8_t VD;
	// Often VF is used as a flag
	int8_t VF;

	// Address Register (only gets loaded with 12-bits)

	int16_t I;
} register_t; 

int main(int argc, char* argv[]) {
	register_t registers;	
    	printf("int8_t size = %d byte, int16_t size = %d bytes\n", sizeof(registers.V0), sizeof(registers.I));

    	return 0;
}
