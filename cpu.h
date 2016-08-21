#ifndef CPU_H_
#define CPU_H_

#include "mmu.h"
#include <stdint.h>
#include <iostream>

#define MAX_INT_4BIT  0x000F
#define MAX_INT_8BIT  0x00FF
#define MAX_INT_12BIT 0x0FFF
#define MAX_INT_16BIT 0xFFFF

#define C_FLAG 0x10  // carry flag
#define H_FLAG 0x20  // half carry flag
#define N_FLAG 0x40  // subtract flag
#define Z_FLAG 0x80  // zero flag


class CPU {
public:
	CPU(MMU& mmu);
	void initialize();
	int execute_next_opcode();

	void cpu_dump();
	
private:
	union cpu_register {
		uint16_t highlow;
		struct {
			uint16_t low : 8;  // Order is subject to system endianess
			uint16_t high : 8;
		};
	};

	// The 8-bit general registers have been paired to form four 16-bit registers
	// since the cpu frequently combines them for certain opcodes.
	cpu_register AF;
	cpu_register BC;
	cpu_register DE;
	cpu_register HL;

	uint16_t SP;
	uint16_t PC;

	int clock_cycles;  // 4 clock cycles = 1 machine cycle

	MMU& gb_mmu;

	// ****** GameBoy Opcode Instructions ******
	void JUMP(const uint16_t addr);

	// 8-Bit ALU
	void ADD_8BIT(const uint8_t val);
	void ADC_8BIT(const uint8_t val);
	void SUB(const uint8_t val);
	void SBC(const uint8_t val);
	void XOR(const uint8_t val);
	void AND(const uint8_t val);
	void OR(const uint8_t val);
	void CP(const uint8_t val);
	uint8_t INC_8BIT(uint8_t val);
	uint8_t DEC_8BIT(uint8_t val);

	// 8-Bit Loads
	void LD_reg_val(const char reg, const uint8_t val);
	void LD_addr_val(const uint16_t addr, const uint8_t val);
};



// Jump to address designated by val.
inline void CPU::JUMP(const uint16_t addr)
{
	PC = addr;
}


// Add val to register A.
inline void CPU::ADD_8BIT(const uint8_t val)
{
	// set flags
	if (AF.high + val == 0)
		AF.low |= Z_FLAG;
	if ((AF.high & 0xF) > (MAX_INT_4BIT - (val & 0xF)))
		AF.low |= H_FLAG;
	if (AF.high > (MAX_INT_8BIT - val))
		AF.low |= C_FLAG;
	AF.low &= ~(N_FLAG);

	AF.high += val;
	clock_cycles += 4;
}


// Add val + Carry Flag to A.
inline void CPU::ADC_8BIT(const uint8_t val)
{
	// set flags
	if (AF.high + val == 0)
		AF.low |= Z_FLAG;
	if ((AF.high & 0xF) > (MAX_INT_4BIT - (val & 0xF)))
		AF.low |= H_FLAG;
	if (AF.high > (MAX_INT_8BIT - val))
		AF.low |= C_FLAG;
	AF.low &= ~(N_FLAG);

	AF.high = AF.high + val + ((AF.low & C_FLAG) >> 4);
	clock_cycles += 4;
}


// Subtract val from A.
inline void CPU::SUB(const uint8_t val)
{
	// set flags
	if (AF.high - val == 0)
		AF.low |= Z_FLAG;
	if ((AF.high & 0xF) > (val & 0xF))  // Set if no borrow
		AF.low |= H_FLAG;
	if (AF.high > val)  // Set if no borrow
		AF.low |= C_FLAG;
	AF.low |= N_FLAG;

	AF.high -= val;
	clock_cycles += 4;
}


// Subtract A + Carry Flag from A.
inline void CPU::SBC(const uint8_t val)
{
	// set flags
	if (AF.high - val == 0)
		AF.low |= Z_FLAG;
	if ((AF.high & 0xF) > (val & 0xF))  // Set if no borrow
		AF.low |= H_FLAG;
	if (AF.high > val)  // Set if no borrow
		AF.low |= C_FLAG;
	AF.low |= N_FLAG;

	AF.high = AF.high - val - ((AF.low & C_FLAG) >> 4);
	clock_cycles += 4;
}


// Logical exclusive OR register A with val.
inline void CPU::XOR(const uint8_t val)
{
	// set flags
	AF.low = 0;
	if ((AF.high ^ val) == 0)
		AF.low |= Z_FLAG;

	AF.high ^= val;
	clock_cycles += 4;
}


// Logical AND register A with val.
inline void CPU::AND(const uint8_t val)
{
	// set flags
	AF.low = 0;
	AF.low |= H_FLAG;
	if ((AF.high & val) == 0)
		AF.low |= Z_FLAG;

	AF.high &= val;
	clock_cycles += 4;
}


// Logical OR register A with val.
inline void CPU::OR(const uint8_t val)
{
	// set flags
	AF.low = 0;
	if ((AF.high | val) == 0)
		AF.low |= Z_FLAG;

	AF.high |= val;
	clock_cycles += 4;
}


// Compare register A with val.
inline void CPU::CP(const uint8_t val)
{
	// set flags
	AF.low |= N_FLAG;
	if (AF.high - val == 0)
		AF.low |= Z_FLAG;
	if ((AF.high & 0xF) > (val & 0xF))  // Set if no borrow
		AF.low |= H_FLAG;
	if (AF.high > val)  // Set if no borrow
		AF.low |= C_FLAG;

	clock_cycles += 4;
}


// Increment val by 1.
inline uint8_t CPU::INC_8BIT(uint8_t val)
{
	// set flags
	AF.low &= ~(N_FLAG);
	if (val + 1 == 0)
		AF.low |= Z_FLAG;
	if (((val & 0xF) + 1) > MAX_INT_4BIT)
		AF.low |= H_FLAG;

	val++;
	clock_cycles += 4;

	return val;
}


// Decrement val by 1.
inline uint8_t CPU::DEC_8BIT(uint8_t val)
{
	// set flags
	AF.low |= N_FLAG;
	if (val - 1 == 0)
		AF.low |= Z_FLAG;
	if ((val & 0xF) > 1)  // Set if no borrow
		AF.low |= H_FLAG;

	val--;
	clock_cycles += 4;

	return val;
}


// Load val into the specified 16-bit address
inline void CPU::LD_addr_val(const uint16_t addr, const uint8_t val)
{
	gb_mmu.write(addr, val);
	clock_cycles += 8;
}


#endif  // CPU_H_
