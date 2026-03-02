#include "../../include/mmu.hpp"
#include <algorithm>
#include <iterator>

MMU::MMU() {
    // zero out the memory
    std::fill(std::begin(memory), std::end(memory), 0);
}

u8 MMU::read(u16 address) {
    // return what is in the array
    return memory[address];
}

void MMU::write(u16 address, u8 value) {
    // cannot overwrite the physical game cartridge i.e from 0x0000 to 0x7FFF
    if (address < 0x8000) {
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

    std::copy(rom_data.begin(), rom_data.begin() + copy_size, std::begin(memory));

    return true;
}