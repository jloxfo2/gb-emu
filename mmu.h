#ifndef MMU_H_
#define MMU_H_

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string.h>


class MMU {
public:
	MMU();
	void initialize();
	int load_ROM(const char* file_name);
	
	uint8_t read(const uint16_t addr) const;
	void write_byte(const uint16_t addr, const uint8_t value);
	void write_word(const uint16_t addr, const uint16_t value);
private:
	static const int RAM_size = 0x10000;
	static const int cartridge_size = 0x200000;
	uint8_t memory[RAM_size];
	uint8_t cartridge[cartridge_size];  // GameBoy ROMs are stored on cartridges

	uint8_t cartridge_type;
	uint8_t RAM_bank_size;
	uint8_t ROM_bank_size;
};


// Input: addr - 16-bit memory address
// Return: 8-bit value stored at the provided address
inline uint8_t MMU::read(const uint16_t addr) const
{
	return memory[addr];
}


// Input: addr - 16-bit memory address
//        value - 8-bit value to be written at provided address
// Return: None
inline void MMU::write_byte(const uint16_t addr, const uint8_t value)
{
	// write to I/O ports
	// if ((addr >= 0xFF00) && (addr < 0xFF4C)) 
	//	memory[addr] = value;

	memory[addr] = value;
}



// Input: addr - 16-bit memory address
//        value - 16-bit value to be written at provided address
// Return: None
inline void MMU::write_word(const uint16_t addr, const uint16_t value)
{
	memory[addr] = value & 0x0F;
	memory[addr + 1] = (value & 0xF0) >> 8;
}

#endif  // MMU_H_
