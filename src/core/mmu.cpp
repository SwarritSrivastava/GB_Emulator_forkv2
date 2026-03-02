#include "../../include/mmu.hpp"
#include <algorithm>
#include <iterator>

MMU::MMU() {
    // zero out the memory
    std::ranges::fill(memory, 0);
}

u8 MMU::read(u16 address) const {
    // warm
    if (address >= 0xC000 && address <= 0xDFFF) {
        return warm[address - 0xC000];
    }
    // return what is in the array
    return memory[address];
}

void MMU::write(u16 address, u8 value) {
    // cannot overwrite the physical game cartridge i.e from 0x0000 to 0x7FFF
    if (address < 0x8000) {
        return; 
    }

    // instead of the flat array save it into our dedicated warm chip
    if (address >= 0xC000 && address <= 0xDFFF) {
        warm[address - 0xC000] = value;
        return;
    }

    memory[address] = value;
}

bool MMU::map_rom(const std::vector<u8>& rom_data) {
    if(rom_data.empty()) return false;

    size_t copy_size = rom_data.size();
    // fixed ROM is strictly 32 KB, chop off if size is greater
    if(copy_size > 0x8000) {
        copy_size = 0x8000;
    }

    std::copy_n(rom_data.begin(), copy_size, std::begin(memory));

    return true;
}