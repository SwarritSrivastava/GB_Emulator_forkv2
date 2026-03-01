#include "../../include/mmu.hpp"

MMU::MMU() {
    // zero out the memory
    for (int i = 0; i < 0x10000; i++) {
        memory[i] = 0;   
    }
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