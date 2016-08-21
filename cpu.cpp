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
	uint8_t opcode = gb_mmu.read(PC);

	uint8_t tmp_byte;
	uint16_t tmp_word;

	switch(opcode) {
	case 0x00:  // NOP
		clock_cycles += 4;
		break;
	case 0xC3:  // JP nn
		JUMP((gb_mmu.read(PC + 2) << 8) | (gb_mmu.read(PC + 1)));
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
	case 0x06:  // LD B, n
		tmp_byte = gb_mmu.read(PC + 1);
		LD_reg_val('B', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x0E:  // LD C, n
		tmp_byte = gb_mmu.read(PC + 1);
		LD_reg_val('C', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x16:  // LD D, n
		tmp_byte = gb_mmu.read(PC + 1);
		LD_reg_val('D', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x1E:  // LD E, n
		tmp_byte = gb_mmu.read(PC + 1);
		LD_reg_val('E', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x26:  // LD H, n
		tmp_byte = gb_mmu.read(PC + 1);
		LD_reg_val('H', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x2E:  // LD L, n
		tmp_byte = gb_mmu.read(PC + 1);
		LD_reg_val('L', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x7F:  // LD A, A
		LD_reg_val('A', AF.high);
		break;
	case 0x78:  // LD A, B
		LD_reg_val('A', BC.high);
		break;
	case 0x79:  // LD A, C
		LD_reg_val('A', BC.low);
		break;
	case 0x7A:  // LD A, D
		LD_reg_val('A', DE.high);
		break;
	case 0x7B:  // LD A, E
		LD_reg_val('A', DE.low);
		break;
	case 0x7C:  // LD A, H
		LD_reg_val('A', HL.high);
		break;
	case 0x7D:  // LD A, L
		LD_reg_val('A', HL.low);
		break;
	case 0x7E:  // LD A, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		LD_reg_val('A', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x40:  // LD B, B
		LD_reg_val('B', BC.high);
		break;
	case 0x41:  // LD B, C
		LD_reg_val('B', BC.low);
		break;
	case 0x42:  // LD B, D
		LD_reg_val('B', DE.high);
		break;
	case 0x43:  // LD B, E
		LD_reg_val('B', DE.low);
		break;
	case 0x44:  // LD B, H
		LD_reg_val('B', HL.high);
		break;
	case 0x45:  // LD B, L
		LD_reg_val('B', HL.low);
		break;
	case 0x46:  // LD B, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		LD_reg_val('B', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x48:  // LD C, B
		LD_reg_val('C', BC.high);
		break;
	case 0x49:  // LD C, C
		LD_reg_val('C', BC.low);
		break;
	case 0x4A:  // LD C, D
		LD_reg_val('C', DE.high);
		break;
	case 0x4B:  // LD C, E
		LD_reg_val('C', DE.low);
		break;
	case 0x4C:  // LD C, H
		LD_reg_val('C', HL.high);
		break;
	case 0x4D:  // LD C, L
		LD_reg_val('C', HL.low);
		break;
	case 0x4E:  // LD C, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		LD_reg_val('C', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x50:  // LD D, B
		LD_reg_val('D', BC.high);
		break;
	case 0x51:  // LD D, C
		LD_reg_val('D', BC.low);
		break;
	case 0x52:  // LD D, D
		LD_reg_val('D', DE.high);
		break;
	case 0x53:  // LD D, E
		LD_reg_val('D', DE.low);
		break;
	case 0x54:  // LD D, H
		LD_reg_val('D', HL.high);
		break;
	case 0x55:  // LD D, L
		LD_reg_val('D', HL.low);
		break;
	case 0x56:  // LD D, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		LD_reg_val('D', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x58:  // LD E, B
		LD_reg_val('E', BC.high);
		break;
	case 0x59:  // LD E, C
		LD_reg_val('E', BC.low);
		break;
	case 0x5A:  // LD E, D
		LD_reg_val('E', DE.high);
		break;
	case 0x5B:  // LD E, E
		LD_reg_val('E', DE.low);
		break;
	case 0x5C:  // LD E, H
		LD_reg_val('E', HL.high);
		break;
	case 0x5D:  // LD E, L
		LD_reg_val('E', HL.low);
		break;
	case 0x5E:  // LD E, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		LD_reg_val('E', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x60:  // LD H, B
		LD_reg_val('H', BC.high);
		break;
	case 0x61:  // LD H, C
		LD_reg_val('H', BC.low);
		break;
	case 0x62:  // LD H, D
		LD_reg_val('H', DE.high);
		break;
	case 0x63:  // LD H, E
		LD_reg_val('H', DE.low);
		break;
	case 0x64:  // LD H, H
		LD_reg_val('H', HL.high);
		break;
	case 0x65:  // LD H, L
		LD_reg_val('H', HL.low);
		break;
	case 0x66:  // LD H, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		LD_reg_val('H', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x68:  // LD L, B
		LD_reg_val('L', BC.high);
		break;
	case 0x69:  // LD L, C
		LD_reg_val('L', BC.low);
		break;
	case 0x6A:  // LD L, D
		LD_reg_val('L', DE.high);
		break;
	case 0x6B:  // LD L, E
		LD_reg_val('L', DE.low);
		break;
	case 0x6C:  // LD L, H
		LD_reg_val('L', HL.high);
		break;
	case 0x6D:  // LD L, L
		LD_reg_val('L', HL.low);
		break;
	case 0x6E:  // LD L, (HL)
		tmp_byte = gb_mmu.read(HL.highlow);
		LD_reg_val('L', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x70:  // LD (HL), B
		LD_addr_val(HL.highlow, BC.high);
		break;
	case 0x71:  // LD (HL), C
		LD_addr_val(HL.highlow, BC.low);
		break;
	case 0x72:  // LD (HL), D
		LD_addr_val(HL.highlow, DE.high);
		break;
	case 0x73:  // LD (HL), E
		LD_addr_val(HL.highlow, DE.low);
		break;
	case 0x74:  // LD (HL), H
		LD_addr_val(HL.highlow, HL.high);
		break;
	case 0x75:  // LD (HL), L
		LD_addr_val(HL.highlow, HL.low);
		break;
	case 0x36:  // LD (HL), n
		tmp_byte = gb_mmu.read(PC + 1);
		LD_addr_val(HL.highlow, tmp_byte);
		clock_cycles += 4;
		break;
	case 0x0A:  // LD A, (BC)
		tmp_byte = gb_mmu.read(BC.highlow);
		LD_reg_val('A', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x1A:  // LD A, (DE)
		tmp_byte = gb_mmu.read(DE.highlow);
		LD_reg_val('A', tmp_byte);
		clock_cycles += 4;
		break;
	case 0xFA:  // LD A, nn
		tmp_word = (gb_mmu.read(PC + 2) << 8) | gb_mmu.read(PC + 1);
		tmp_byte = gb_mmu.read(tmp_word);
		LD_reg_val('A', tmp_byte);
		clock_cycles += 12;
		break;
	case 0x3E:  // LD A, #
		tmp_byte = gb_mmu.read(PC + 1);
		LD_reg_val('A', tmp_byte);
		clock_cycles += 4;
		break;
	case 0x47:  // LD B, A
		LD_reg_val('B', AF.high);
		break;
	case 0x4F:  // LD C, A
		LD_reg_val('C', AF.high);
		break;
	case 0x57:  // LD D, A
		LD_reg_val('D', AF.high);
		break;
	case 0x5F:  // LD E, A
		LD_reg_val('E', AF.high);
		break;
	case 0x67:  // LD H, A
		LD_reg_val('H', AF.high);
		break;
	case 0x6F:  // LD L, A
		LD_reg_val('L', AF.high);
		break;
	case 0x02:  // LD (BC), A
		LD_addr_val(BC.highlow, AF.high);
		break;
	case 0x12:  // LD (DE), A
		LD_addr_val(DE.highlow, AF.high);
		break;
	case 0x77:  // LD (HL), A
		LD_addr_val(HL.highlow, AF.high);
		break;
	case 0xEA:  // LD (nn), A
		tmp_word = (gb_mmu.read(PC + 2) << 8) | gb_mmu.read(PC + 1);
		LD_addr_val(tmp_word, AF.high);
		clock_cycles += 8;
		break;
	case 0xF2:  // LD A, (C)
		tmp_byte = gb_mmu.read(0xFF00 + BC.low);
		LD_reg_val('A', tmp_byte);
		clock_cycles += 4;
		break;
	case 0xE2:  // LD (C), A
		tmp_word = 0xFF00 + BC.low;
		LD_addr_val(tmp_word, AF.high);
		break;
	case 0x3A:  // LD A, (HLD)
		tmp_byte = gb_mmu.read(HL.highlow);
		LD_reg_val('A', tmp_byte);
		HL.highlow--;
		clock_cycles += 4;
		break;
	case 0x32:  // LD (HLD), A
		LD_addr_val(HL.highlow, AF.high);
		HL.highlow--;
		break;
	case 0x2A:  // LD A, (HLI)
		tmp_byte = gb_mmu.read(HL.highlow);
		LD_reg_val('A', tmp_byte);
		HL.highlow++;
		clock_cycles += 4;
		break;
	case 0x22:  // LD (HLI), A
		LD_addr_val(HL.highlow, AF.high);
		HL.highlow++;
		break;
	case 0xE0:  // LDH (n), A
		tmp_byte = gb_mmu.read(PC + 1);
		LD_addr_val(0xFF00 + tmp_byte, AF.high);
		clock_cycles += 4;
		break;
	case 0xF0:  // LDH A, (n)
		tmp_byte = gb_mmu.read(PC + 1);
		LD_reg_val('A', 0xFF00 + tmp_byte);
		clock_cycles += 8;
		break;
	default:
		printf("Unknown opcode: 0x%02X\n", opcode);
		cpu_dump();
	}

	PC++;
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


// ****** GameBoy Opcode Instructions ******

// Load val into the specified register
void CPU::LD_reg_val(const char reg, const uint8_t val)
{
	switch(reg) {
	case 'A':
		AF.high = val;
	case 'B':
		BC.high = val;
	case 'C':
		BC.low = val;
	case 'D':
		DE.high = val;
	case 'E':
		DE.low = val;
	case 'H':
		HL.high = val;
	case 'L':
		HL.low = val;
	}

	clock_cycles += 4;
}
