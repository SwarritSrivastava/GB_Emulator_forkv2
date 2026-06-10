#include "../include/cartridge.hpp"
#include "../include/success.hpp"
#include "../include/readROM.hpp"
#include "../include/ProcessingUnit.hpp"
#include "../include/mmu.hpp"
#include "../include/ppu.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <iterator>
#include <SFML/Graphics.hpp>

static void update_joypad(MMU& mmu, const JoypadState& joy) {
    u8 action = 0x0F;
    u8 direction = 0x0F;

    if (joy.right) direction &= ~0x01;
    if (joy.left)  direction &= ~0x02;
    if (joy.up)    direction &= ~0x04;
    if (joy.down)  direction &= ~0x08;

    if (joy.a)      action &= ~0x01;
    if (joy.b)      action &= ~0x02;
    if (joy.select) action &= ~0x04;
    if (joy.start)  action &= ~0x08;

    mmu.set_joypad_state(action, direction);
}

int main(const int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: gb_emu <rom_file> [--debug]\n";
        return 1;
    }

    bool debug_mode = false;
    for (int i = 2; i < argc; ++i) {
        if (std::strcmp(argv[i], "--debug") == 0 || std::strcmp(argv[i], "-d") == 0) {
            debug_mode = true;
        }
    }

    try {
        const std::vector<u8> rom_data = load_rom(argv[1]);
        MMU mmu;
        PPU ppu;
        mmu.set_ppu(&ppu);
        ppu.set_mmu(&mmu);
        
        if (!mmu.map_rom(rom_data)) {
            std::cerr << "Failed to map ROM\n";
            return 1;
        }

        ProcessingUnit cpu;
        ppu.set_cpu(&cpu);

        // Populate ROM Info for the debugger panel
        rom_header header{};
        std::memcpy(&header, &rom_data[0x100], sizeof(rom_header));
        char title[17]{};
        std::memcpy(title, header.title, 16);

        PPU::RomInfo info{};
        info.title = title;
        info.type = header.type;
        info.rom_size = header.rom_size;
        info.ram_size = header.ram_size;
        info.rom_bytes = rom_data;
        ppu.set_rom_info(info);

        ppu.init_window(debug_mode, title);
        std::cout << "Running in " << (debug_mode ? "Debug/Feature" : "Simple") << " Mode...\n";

        JoypadState joypad;
        sf::Clock clock;
        u64 cycles = 0;

        while (ppu.isOpen()) {
            ppu.handleEvents(joypad);
            update_joypad(mmu, joypad);

            const float dt = clock.restart().asSeconds();

            if (!ppu.isPaused()) {
                while (!ppu.is_frame_ready()) {
                    const u16 pc_before = cpu.get_pc();
                    const u8 opcode = mmu.read(pc_before);

                    int used = cpu.step(mmu);
                    cycles += static_cast<u64>(used);
                    ppu.step(used);
                    mmu.step_timer(used);

                    ppu.recordOpcode(pc_before, opcode);
                    ppu.checkBreakpoint(pc_before);

                    if (ppu.isPaused()) {
                        break;
                    }
                }
                if (ppu.is_frame_ready()) {
                    ppu.clear_frame_ready();
                }
            } else {
                if (ppu.isStepRequested()) {
                    const u16 pc_before = cpu.get_pc();
                    const u8 opcode = mmu.read(pc_before);

                    int used = cpu.step(mmu);
                    cycles += static_cast<u64>(used);
                    ppu.step(used);
                    mmu.step_timer(used);

                    ppu.recordOpcode(pc_before, opcode);
                    ppu.checkBreakpoint(pc_before);

                    ppu.clearStepRequest();
                }
            }

            ppu.update(dt, cycles);
            ppu.render();
        }
        std::cout << std::dec << "\n" << cycles << " cycles executed" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
