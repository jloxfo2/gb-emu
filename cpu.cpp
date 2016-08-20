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

	uint8_t tmp_byte;
	// uint16_t tmp_word;

	switch(opcode) {
	case 0x00:  // NOP
		clock_cycles += 4;
		break;
	case 0xC3:  // JP nn
		JUMP((gb_mmu.read(PC + 1) << 8) | (gb_mmu.read(PC)));
		clock_cycles += 16;
		break;
	case 0x87:  // ADD A, A
		ADD_8BIT(AF.high);
		break;
	case 0x80:  // ADD A, B
		ADD_8BIT(BC.high);
		break;
	case 0x81:  // ADD A, C
		ADD_8BIT(BC.low);
		break;
	case 0x82:
		ADD_8BIT(DE.high);
		break;
	case 0x83:  // ADD A, E
		ADD_8BIT(DE.low);
		break;
	case 0x84:  // ADD A, H
		ADD_8BIT(HL.high);
		break;
	case 0x85:  // ADD A, L
		ADD_8BIT(HL.low);
		break;
	case 0x86:  // ADD A, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		ADD_8BIT(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xC6:  // ADD A, #
		tmp_byte = gb_mmu.read(PC + 1);
		ADD_8BIT(tmp_byte);
		clock_cycles += 4;
		break;
	case 0x8F:  // ADC A, A
		ADC_8BIT(AF.high);
		break;
	case 0x88:  // ADC A, B
		ADC_8BIT(BC.high);
		break;
	case 0x89:  // ADC A, C
		ADC_8BIT(BC.low);
		break;
	case 0x8A:  // ADC A, D
		ADC_8BIT(DE.high);
		break;
	case 0x8B:  // ADC A, E
		ADC_8BIT(DE.low);
		break;
	case 0x8C:  // ADC A, H
		ADC_8BIT(HL.high);
		break;
	case 0x8D:  // ADC A, L
		ADC_8BIT(HL.low);
		break;
	case 0x8E:  // ADC A, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		ADC_8BIT(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xCE:  // ADC A, #
		tmp_byte = gb_mmu.read(PC + 1);
		ADC_8BIT(tmp_byte);
		clock_cycles += 4;
		break;
	case 0x97:  // SUB A
		SUB(AF.high);
		break;
	case 0x90:  // SUB B
		SUB(BC.high);
		break;
	case 0x91:  // SUB C
		SUB(BC.low);
		break;
	case 0x92:  // SUB D
		SUB(DE.high);
		break;
	case 0x93:  // SUB E
		SUB(DE.low);
		break;
	case 0x94:  // SUB H
		SUB(HL.high);
		break;
	case 0x95:  // SUB L
		SUB(HL.low);
		break;
	case 0x96:  // SUB (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		SUB(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xD6:  // SUB #
		tmp_byte = gb_mmu.read(PC + 1);
		SUB(tmp_byte);
		clock_cycles += 4;
		break;
	case 0x9F:  // SBC A, A
		SBC(AF.high);
		break;
	case 0x98:  // SBC A, B
		SBC(BC.high);
		break;
	case 0x99:  // SBC A, C
		SBC(BC.low);
		break;
	case 0x9A:  // SBC A, D
		SBC(DE.high);
		break;
	case 0x9B:  // SBC A, E
		SBC(DE.low);
		break;
	case 0x9C:  // SBC A, H
		SBC(HL.high);
		break;
	case 0x9D:  // SBC A, L
		SBC(HL.low);
		break;
	case 0x9E:  // SBC A, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		SBC(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xA7:  // AND A
		AND(AF.high);
		break;
	case 0xA0:  // AND B
		AND(BC.high);
		break;
	case 0xA1:  // AND C
		AND(BC.low);
		break;
	case 0xA2:  // AND D
		AND(DE.high);
		break;
	case 0xA3:  // AND E
		AND(DE.low);
		break;
	case 0xA4:  // AND H
		AND(HL.high);
		break;
	case 0xA5:  // AND L
		AND(HL.low);
		break;
	case 0xA6:  // AND (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		AND(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xE6:  // AND #
		tmp_byte = gb_mmu.read(PC + 1);
		AND(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xB7:  // OR A
		OR(AF.high);
		break;
	case 0xB0:  // OR B
		OR(BC.high);
		break;
	case 0xB1:  // OR C
		OR(BC.low);
		break;
	case 0xB2:  // OR D
		OR(DE.high);
		break;
	case 0xB3:  // OR E
		OR(DE.low);
		break;
	case 0xB4:  // OR H
		OR(HL.high);
		break;
	case 0xB5:  // OR L
		OR(HL.low);
		break;
	case 0xB6:  // OR (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		OR(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xF6:  // OR #
		tmp_byte = gb_mmu.read(PC + 1);
		OR(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xAF:  // XOR A
		XOR(AF.high);
		break;
	case 0xA8:  // XOR B
		XOR(BC.high);
		break;
	case 0xA9:  // XOR C
		XOR(BC.low);
		break;
	case 0xAA:  // XOR D
		XOR(DE.high);
		break;
	case 0xAB:  // XOR E
		XOR(DE.low);
		break;
	case 0xAC:  // XOR H
		XOR(HL.high);
		break;
	case 0xAD:  // XOR L
		XOR(HL.low);
		break;
	case 0xAE:  // XOR (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		XOR(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xEE:  // XOR #
		tmp_byte = gb_mmu.read(PC + 1);
		XOR(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xBF:  // CP A
		CP(AF.high);
		break;
	case 0xB8:  // CP B
		CP(BC.high);
		break;
	case 0xB9:  // CP C
		CP(BC.low);
		break;
	case 0xBA:  // CP D
		CP(DE.high);
		break;
	case 0xBB:  // CP E
		CP(DE.low);
		break;
	case 0xBC:  // CP H
		CP(HL.high);
		break;
	case 0xBD:  // CP L
		CP(HL.low);
		break;
	case 0xBE:  // CP (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		CP(tmp_byte);
		clock_cycles += 4;
		break;
	case 0xFE:  // CP #
		tmp_byte = gb_mmu.read(PC + 1);
		CP(tmp_byte);
		clock_cycles += 4;
		break;
	case 0x3C:  // INC A
		AF.high = INC_8BIT(AF.high);
		break;
	case 0x04:  // INC B
		BC.high = INC_8BIT(BC.high);
		break;
	case 0x0C:  //INC C
		BC.low = INC_8BIT(BC.low);
		break;
	case 0x14:  // INC D
		DE.high = INC_8BIT(DE.high);
		break;
	case 0x1C:  // INC E
		DE.low = INC_8BIT(DE.low);
		break;
	case 0x24:  // INC H
		HL.high = INC_8BIT(HL.high);
		break;
	case 0x2C:  // INC L
		HL.low = INC_8BIT(HL.low);
		break;
	case 0x34:  // INC (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		tmp_byte = INC_8BIT(tmp_byte);
		gb_mmu.write(HL.highlow, tmp_byte);
		clock_cycles += 8;
		break;
	case 0x3D:  // DEC A
		AF.high = DEC_8BIT(AF.high);
		break;
	case 0x05:  // DEC B
		BC.high = DEC_8BIT(BC.high);
		break;
	case 0x0D:  // DEC C
		BC.low = DEC_8BIT(BC.low);
		break;
	case 0x15:  // DEC D
		DE.high = DEC_8BIT(DE.high);
		break;
	case 0x1D:  // DEC E
		DE.low = DEC_8BIT(DE.low);
		break;
	case 0x25:  // DEC H
		HL.high = DEC_8BIT(HL.high);
		break;
	case 0x2D:  // DEC L
		HL.low = DEC_8BIT(HL.low);
		break;
	case 0x35:  // DEC (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		tmp_byte = DEC_8BIT(tmp_byte);
		gb_mmu.write(HL.highlow, tmp_byte);
		clock_cycles += 8;
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
	printf("AF: 0x%04X\n", AF.highlow);
	printf("BC: 0x%04X\n", BC.highlow);
	printf("DE: 0x%04X\n", DE.highlow);
	printf("HL: 0x%04X\n", HL.highlow);
	getchar();
}
