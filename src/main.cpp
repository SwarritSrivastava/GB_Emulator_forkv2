#include "../include/cartridge.hpp"
#include "../include/success.hpp"
#include "functions/readROM.cpp"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <iterator>

int main(const int argc, char** argv) {

    if(argc < 2){
        std::cerr << "Usage: gb_emu <rom_file>\n";
        return 1;
    }

    try{
        load_rom(argv[1]);
        success();
    }
    catch(const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch(...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
    return 0;
}
