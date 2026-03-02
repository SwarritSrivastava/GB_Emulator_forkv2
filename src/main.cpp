#include "../include/cartridge.hpp"
#include "../include/success.hpp"
#include "../include/readROM.hpp"
#include "../include/ProcessingUnit.hpp"
#include "../include/mmu.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <iterator>

int main(const int argc, char **argv)
{

    if (argc < 2)
    {
        std::cerr << "Usage: gb_emu <rom_file>\n";
        return 1;
    }

    try
    {
        const std::vector<u8> rom_data = load_rom(argv[1]);

        // ----- MMU ------
        MMU mmu;
        if(mmu.map_rom(rom_data)) {
            std::cout << "ROM successfully mapped to MMU memory ..." << std::endl;
        }
        else {
            std::cerr << "failed to map ROM ... " << std::endl;
            return 1;
        }
        // ----- CPU ------
        ProcessingUnit cpu;
        std::cout << "Initial State (Post-Reset):" << std::endl;
        cpu.printStatus();
        success();
        cpu.reset();
        while (!cpu.isHalt()){
            cpu.step(mmu);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }

    return 0;
}
