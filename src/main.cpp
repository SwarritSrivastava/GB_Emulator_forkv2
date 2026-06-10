#include "../include/cartridge.hpp"
#include "../include/success.hpp"
#include "../include/readROM.hpp"
#include "../include/ProcessingUnit.hpp"
#include "../include/mmu.hpp"
#include "../include/ppu.hpp"
#include <SFML/System.hpp>
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
        if(!mmu.map_rom(rom_data)){
            std::cerr << "failed to map ROM ... " << std::endl;
            return 1;
        }
        std::cout << "ROM successfully mapped to MMU memory ..." << std::endl;
        // ----- CPU ------
        ProcessingUnit cpu;
        std::cout << "Initial State (Post-Reset):" << std::endl;
        cpu.printStatus();
        success();

        rom_header header{};
        std::memcpy(&header, &rom_data[0x100], sizeof(rom_header));
        char title[17]{};
        std::memcpy(title, header.title, 16);

        PPU::RomInfo info{};
        info.title = title;
        info.type = header.type;
        info.rom_size = header.rom_size;
        info.ram_size = header.ram_size;

        PPU ppu(cpu, mmu, info);

        sf::Clock clock;
        u64 cycles = 0;
        while (ppu.isOpen() && !cpu.isHalt())
        {
            const float dt = clock.restart().asSeconds();
            ppu.handleEvents();

            const u16 pc_before = cpu.get_pc();
            const u8 opcode = mmu.read(pc_before);
            const int used = cpu.step(mmu);
            cycles += static_cast<u64>(used);
            ppu.recordOpcode(pc_before, opcode);

            ppu.update(dt, cycles);
            ppu.render();
        }
        std::cout << std::dec << std::endl << cycles << " cycles executed" << std::endl;
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
