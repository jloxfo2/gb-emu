#include "mmu.h"

#define MIN(x,y) ((x < y) ? x : y)


MMU::MMU()
{
	initialize();
}


// Inputs: None
// Function: Clear the internal RAM and initialize the
//           special I/O registers in RAM
void MMU::initialize()
{
	// The GameBoy RAM actually contains random values when it's loaded,
	// but it's zero'd out for emulation.
	memset(memory, 0, RAM_size);

	// Special I/O registers
	memory[0xFF05] = 0x00;  // TIMA
	memory[0xFF06] = 0x00;  // TMA
	memory[0xFF07] = 0x00;  // TAC
	memory[0xFF10] = 0x80;  // NR10
	memory[0xFF11] = 0xBF;  // NR11
	memory[0xFF12] = 0xF3;  // NR12
	memory[0xFF14] = 0xBF;  // NR14
	memory[0xFF16] = 0x3F;  // NR21
	memory[0xFF17] = 0x00;  // NR22
	memory[0xFF19] = 0xBF;  // NR24
	memory[0xFF1A] = 0x7F;  // NR30
	memory[0xFF1B] = 0xFF;  // NR31
	memory[0xFF1C] = 0x9F;  // NR32
	memory[0xFF1E] = 0xBF;  // NR33
	memory[0xFF20] = 0xFF;  // NR41
	memory[0xFF21] = 0x00;  // NR42
	memory[0xFF22] = 0x00;  // NR43
	memory[0xFF23] = 0xBF;  // NR30
	memory[0xFF24] = 0x77;  // NR50
	memory[0xFF25] = 0xF3;  // NR51
	memory[0xFF26] = 0xF1;  // NR52
	memory[0xFF40] = 0x91;  // LCDC
	memory[0xFF42] = 0x00;  // SCY
	memory[0xFF43] = 0x00;  // SCX
	memory[0xFF45] = 0x00;  // LYC
	memory[0xFF47] = 0xFC;  // BGP
	memory[0xFF48] = 0xFF;  // OBP0
	memory[0xFF49] = 0xFF;  // OBP1
	memory[0xFF4A] = 0x00;  // WY
	memory[0xFF4B] = 0x00;  // WX
	memory[0xFFFF] = 0x00;  // IE
}


// Input: file_name - Name of the ROM to be loaded
// Return Value: Return 0 on success; 
//               return -1 on failure
// Function: Load the specified ROM into cartridge memory for emulation and store ROM info
int MMU::load_ROM(const char* file_name)
{
	std::ifstream File(file_name, std::ios::in | std::ios::binary | std::ios::ate);

	if (!File.is_open())
		return -1;

	memset(cartridge, 0, 0x200000);

	// get file size and set get position to beginning of file
	std::streampos size = File.tellg();
	File.seekg(0, std::ios::beg);

	File.read((char*)cartridge, size);

	cartridge_type = cartridge[0x0147];
	RAM_bank_size = cartridge[0x0148];
	ROM_bank_size = cartridge[0x0149];

	// load first available 32kB into memory
	memcpy(memory, cartridge, MIN((int)size, 0x8000));

	return 0;
}
