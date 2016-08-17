#include "cpu.h"

#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)


CPU::CPU(MMU& mmu) : gb_mmu(mmu)
{
	initialize();
}


// Inputs: None
// Function: Initialize all of the CPU general registers, set the PC
//           and SP registers, clear the internal RAM, and initialize
//           the special I/O registers in RAM
void CPU::initialize()
{
	PC = 0x0100;
	SP = 0xFFFE;

	AF.highlow = 0x01B0;
	BC.highlow = 0x0013;
	DE.highlow = 0x00D8;
	HL.highlow = 0x014D;

	gb_mmu.initialize();
}


// Input: None
// Return Value: 
// Function: Execute the opcode and update the CPU state
int CPU::execute_next_opcode()
{
	uint8_t opcode = gb_mmu.read(PC++);

	switch(opcode) {
	case 0x00:  // NOP
		clock_cycles += 4;
		break;
	case 0xC3:  // JP nn
		JUMP((gb_mmu.read(PC + 1) << 8) | (gb_mmu.read(PC)));
		clock_cycles += 16;
		break;
	default:
		printf("Unknown opcode: 0x%02X\n", opcode);
		cpu_dump();
	}

	return 0;
}


// Input: None
// Return Value: None
// Function: Dump the state of all the cpu registers
void CPU::cpu_dump()
{
	printf("PC: 0x%04X\n", PC);
	printf("SP: 0x%04X\n", SP);
	printf("AF: 0x%02X\n", AF.highlow);
	printf("BC: 0x%02X\n", BC.highlow);
	printf("DE: 0x%02X\n", DE.highlow);
	printf("HL: 0x%02X\n", HL.highlow);
	getchar();
}