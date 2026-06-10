#include "../../../include/mmu.hpp"
#include "../../../include/ppu.hpp"
#include <algorithm>
#include <iterator>

MMU::MMU() {
    io_regs.fill(0);
    hram.fill(0);
    wram.fill(0);
    eram.fill(0);
    vram_fallback.fill(0);
    oam_fallback.fill(0);
    ppu_reg_fallback.fill(0);

    // Post-boot defaults
    write(0xFF00, 0xCF); // Joypad
    write(0xFF04, 0xAB); // DIV
    write(0xFF05, 0x00); // TIMA
    write(0xFF06, 0x00); // TMA
    write(0xFF07, 0xF8); // TAC
    write(0xFF0F, 0xE1); // IF
    write(0xFF40, 0x91); // LCDC
    write(0xFF41, 0x85); // STAT
    write(0xFF42, 0x00); // SCY
    write(0xFF43, 0x00); // SCX
    write(0xFF44, 0x00); // LY
    write(0xFF45, 0x00); // LYC
    write(0xFF47, 0xFC); // BGP
    write(0xFF48, 0xFF); // OBP0
    write(0xFF49, 0xFF); // OBP1
    write(0xFF4A, 0x00); // WY
    write(0xFF4B, 0x00); // WX
    write(0xFFFF, 0x00); // IE
}

u8 MMU::read(const u16 address) const {
    read_count++;
    // ROM Bank 0 (0x0000 - 0x3FFF)
    if (address <= 0x3FFF) {
        if (address < full_rom_data.size()) return full_rom_data[address];
        return 0x00;
    }
    // ROM Bank N (0x4000 - 0x7FFF)
    if (address <= 0x7FFF) {
        u32 banked_address = (address - 0x4000) + (current_rom_bank * 0x4000);
        if (banked_address < full_rom_data.size()) return full_rom_data[banked_address];
        return 0x00;
    }
    // VRAM (0x8000 - 0x9FFF)
    if (address <= 0x9FFF) {
        return ppu ? ppu->read(address) : vram_fallback[address - 0x8000];
    }
    // External RAM (0xA000 - 0xBFFF)
    if (address <= 0xBFFF) {
        return eram[address - 0xA000];
    }
    // WRAM (0xC000 - 0xDFFF)
    if (address <= 0xDFFF) {
        return wram[address - 0xC000];
    }
    // Echo RAM (0xE000 - 0xFDFF) — mirrors WRAM
    if (address <= 0xFDFF) {
        return wram[address - 0xE000];
    }
    // OAM (0xFE00 - 0xFE9F)
    if (address <= 0xFE9F) {
        return ppu ? ppu->read(address) : oam_fallback[address - 0xFE00];
    }
    // Unusable (0xFEA0 - 0xFEFF)
    if (address <= 0xFEFF) {
        return 0x00;
    }
    // I/O Registers (0xFF00 - 0xFF7F)
    if (address == 0xFF00) return get_joypad_state();
    if (address >= 0xFF04 && address <= 0xFF07) return timer.read(address);
    if (address == 0xFF0F) return interrupt_flag;
    if (address >= 0xFF40 && address <= 0xFF4B) {
        return ppu ? ppu->read(address) : ppu_reg_fallback[address - 0xFF40];
    }
    if (address >= 0xFF00 && address <= 0xFF7F) return io_regs[address - 0xFF00];
    // HRAM (0xFF80 - 0xFFFE)
    if (address <= 0xFFFE) return hram[address - 0xFF80];
    // IE (0xFFFF)
    if (address == 0xFFFF) return interrupt_enable;

    return 0xFF;
}

void MMU::write(const u16 address, u8 value) {
    write_count++;
    // MBC1: RAM Enable (0x0000 - 0x1FFF)
    if (address <= 0x1FFF) {
        // RAM enable/disable — ignored for now (no battery save)
        return;
    }
    // MBC1/MBC3: ROM Bank Number (0x2000 - 0x3FFF)
    if (address <= 0x3FFF) {
        u8 bank = value & 0x7F; // 0x7F supports up to 2MB ROMs (MBC3 or large MBC1)
        if (bank == 0) bank = 1;
        current_rom_bank = bank;
        return;
    }
    // MBC1: RAM Bank / Upper ROM Bank (0x4000 - 0x5FFF)
    if (address <= 0x5FFF) {
        // For simple MBC1 with <=512KB ROM, ignored
        return;
    }
    // MBC1: Banking Mode (0x6000 - 0x7FFF)
    if (address <= 0x7FFF) {
        // Banking mode select — ignored for simple MBC1
        return;
    }
    // VRAM (0x8000 - 0x9FFF)
    if (address <= 0x9FFF) {
        if (ppu) ppu->write(address, value);
        else vram_fallback[address - 0x8000] = value;
        return;
    }
    // External RAM (0xA000 - 0xBFFF)
    if (address <= 0xBFFF) {
        eram[address - 0xA000] = value;
        return;
    }
    // WRAM (0xC000 - 0xDFFF)
    if (address <= 0xDFFF) {
        wram[address - 0xC000] = value;
        return;
    }
    // Echo RAM (0xE000 - 0xFDFF) — mirrors WRAM
    if (address <= 0xFDFF) {
        wram[address - 0xE000] = value;
        return;
    }
    // OAM (0xFE00 - 0xFE9F)
    if (address <= 0xFE9F) {
        if (ppu) ppu->write(address, value);
        else oam_fallback[address - 0xFE00] = value;
        return;
    }
    // Unusable (0xFEA0 - 0xFEFF)
    if (address <= 0xFEFF) {
        return;
    }
    // I/O Registers
    if (address == 0xFF00) { joypad_select = value & 0x30; return; }
    if (address >= 0xFF04 && address <= 0xFF07) { timer.write(address, value); return; }
    if (address == 0xFF0F) { interrupt_flag = value; return; }
    if (address == 0xFF46) { perform_dma(value); if (ppu) ppu->write(address, value); return; }
    if (address >= 0xFF40 && address <= 0xFF4B) {
        if (ppu) ppu->write(address, value);
        else ppu_reg_fallback[address - 0xFF40] = value;
        return;
    }
    if (address >= 0xFF00 && address <= 0xFF7F) { io_regs[address - 0xFF00] = value; return; }
    // HRAM (0xFF80 - 0xFFFE)
    if (address <= 0xFFFE) { hram[address - 0xFF80] = value; return; }
    // IE (0xFFFF)
    if (address == 0xFFFF) { interrupt_enable = value; return; }
}

bool MMU::map_rom(const std::vector<u8>& rom_data) {
    if(rom_data.empty()) return false;
    full_rom_data = rom_data;
    return true;
}

void MMU::perform_dma(u8 value) {
    u16 source_address = value << 8;
    for (u16 i = 0; i < 0xA0; i++) {
        write(0xFE00 + i, read(source_address + i));
    }
}

std::vector<u8> MMU::dump_memory() const {
    std::vector<u8> dump(0x10000, 0);
    
    // Copy main arrays
    std::copy(wram.begin(), wram.end(), dump.begin() + 0xC000);
    std::copy(eram.begin(), eram.end(), dump.begin() + 0xA000);
    std::copy(hram.begin(), hram.end(), dump.begin() + 0xFF80);
    std::copy(io_regs.begin(), io_regs.end(), dump.begin() + 0xFF00);
    
    // Save metadata/joypad/interrupt variables in unused/ROM regions of the dump
    dump[0x00] = current_rom_bank;
    dump[0x01] = joypad_select;
    dump[0x02] = action_buttons;
    dump[0x03] = direction_buttons;
    dump[0x04] = interrupt_flag;
    dump[0x05] = interrupt_enable;
    
    // Save fallbacks
    std::copy(vram_fallback.begin(), vram_fallback.end(), dump.begin() + 0x8000);
    std::copy(oam_fallback.begin(), oam_fallback.end(), dump.begin() + 0xFE00);

    // Save PPU VRAM, OAM and registers if present
    for (u16 addr = 0x8000; addr <= 0x9FFF; ++addr) {
        dump[addr] = read(addr);
    }
    for (u16 addr = 0xFE00; addr <= 0xFE9F; ++addr) {
        dump[addr] = read(addr);
    }
    for (u16 addr = 0xFF40; addr <= 0xFF4B; ++addr) {
        dump[addr] = read(addr);
    }
    
    return dump;
}

bool MMU::load_memory(const std::vector<u8>& dump) {
    if (dump.size() != 0x10000) {
        return false;
    }
    
    std::copy(dump.begin() + 0xC000, dump.begin() + 0xE000, wram.begin());
    std::copy(dump.begin() + 0xA000, dump.begin() + 0xC000, eram.begin());
    std::copy(dump.begin() + 0xFF80, dump.begin() + 0xFFFF, hram.begin());
    std::copy(dump.begin() + 0xFF00, dump.begin() + 0xFF80, io_regs.begin());
    
    current_rom_bank = dump[0x00];
    joypad_select = dump[0x01];
    action_buttons = dump[0x02];
    direction_buttons = dump[0x03];
    interrupt_flag = dump[0x04];
    interrupt_enable = dump[0x05];
    
    std::copy(dump.begin() + 0x8000, dump.begin() + 0xA000, vram_fallback.begin());
    std::copy(dump.begin() + 0xFE00, dump.begin() + 0xFEA0, oam_fallback.begin());
    
    // Restore state of VRAM, OAM and registers back to PPU
    for (u16 addr = 0x8000; addr <= 0x9FFF; ++addr) {
        write(addr, dump[addr]);
    }
    for (u16 addr = 0xFE00; addr <= 0xFE9F; ++addr) {
        write(addr, dump[addr]);
    }
    for (u16 addr = 0xFF40; addr <= 0xFF4B; ++addr) {
        write(addr, dump[addr]);
    }
    
    return true;
}
