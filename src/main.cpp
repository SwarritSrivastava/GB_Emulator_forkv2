#include "../include/cartridge.hpp"
#include "../include/success.hpp"
#include "../include/readROM.hpp"
#include "../include/ProcessingUnit.hpp"
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
        load_rom(argv[1]);
        ProcessingUnit cpu;
        std::cout << "Initial State (Post-Reset):" << std::endl;
        cpu.printStatus();
        success();
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
