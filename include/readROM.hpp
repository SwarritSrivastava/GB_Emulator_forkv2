//
// Created by wizard on 25/02/26.
//
#include "cartridge.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <iterator>

int load_rom(const char* _filepath){
    std::ifstream file(_filepath, std::ios::binary);

    if (!file){
        throw std::runtime_error("Error opening file");
    }

    std::vector<u8> rom(
        (std::istreambuf_iterator(file)),
         std::istreambuf_iterator<char>()
    );
    std::cout << "ROM Size: " << (rom.size()/1024) << "K" << std::endl;

    if(rom.empty()){
        throw std::runtime_error("ROM file is empty");
    }

    if(rom.size() < 0x150){
        throw std::runtime_error("ROM too small");
    }

    rom_header _header{};
    std::memcpy(&_header, &rom[0x100], sizeof(rom_header));

    /* avoid Null-terminator error */
    char title[17]{};
    std::memcpy(title, _header.title, 16);

    std::cout << "Title: " << " " << title << std::endl;
    std::cout << "Type:  0x" << std::hex << static_cast<int>(_header.type) << "\n";
    std::cout << "ROM:   0x" << std::hex << static_cast<int>(_header.rom_size) << "\n";
    std::cout << "RAM:   0x" << std::hex << static_cast<int>(_header.ram_size) << "\n";

    u8 sum = 0;

    for (int i = 0x134; i <= 0x14C; i++) {
        sum = sum - rom[i] - 1;
    }

    if (sum == _header.checksum)
        std::cout << "Header OK\n" << std::endl;
    else
        std::cout << "Bad header\n" << std::endl;

    return 0; // success
}