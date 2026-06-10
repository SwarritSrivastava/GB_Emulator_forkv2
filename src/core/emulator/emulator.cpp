#include "../../../include/emulator.hpp"
#include "../../../include/readROM.hpp"
#include "../../../include/cartridge.hpp"
#include <SFML/System.hpp>

Emulator::Emulator(const std::string& romPath)
    : cpu()
    , mmu()
    , ppu(cpu, mmu, PPU::RomInfo{})
{
    loadRom(romPath);
}

void Emulator::loadRom(const std::string& romPath) {
    romData = load_rom(romPath.c_str());
    if (!mmu.map_rom(romData)) {
        throw std::runtime_error("failed to map ROM");
    }

    rom_header header{};
    std::memcpy(&header, &romData[0x100], sizeof(rom_header));
    char title[17]{};
    std::memcpy(title, header.title, 16);

    PPU::RomInfo info{};
    info.title = title;
    info.type = header.type;
    info.rom_size = header.rom_size;
    info.ram_size = header.ram_size;
    info.rom_bytes = romData;
    ppu = PPU(cpu, mmu, info);
}

void Emulator::run() {
    sf::Clock clock;
    while (ppu.isOpen() && !cpu.isHalt()) {
        const float dt = clock.restart().asSeconds();
        ppu.handleEvents();

        if (!ppu.isPaused()) {
            const u16 pc_before = cpu.get_pc();
            const u8 opcode = mmu.read(pc_before);
            const int used = cpu.step(mmu);
            cycles += static_cast<u64>(used);
            ppu.recordOpcode(pc_before, opcode);
            ppu.checkBreakpoint(pc_before);
        }

        ppu.update(dt, cycles);
        ppu.render();
    }
}

void Emulator::stepOnce() {
    const u16 pc_before = cpu.get_pc();
    const u8 opcode = mmu.read(pc_before);
    const int used = cpu.step(mmu);
    cycles += static_cast<u64>(used);
    ppu.recordOpcode(pc_before, opcode);
}
