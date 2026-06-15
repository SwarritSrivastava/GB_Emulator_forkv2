#include "../../../include/mmu.hpp"
#include "../../../include/apu.hpp"
#include "../../../include/cartridge.hpp"
#include "../../../include/ppu.hpp"
#include "../../../include/timer.hpp"
#include "../../../include/interrupt_controller.hpp"
#include <algorithm>
#include <iterator>

MMU::MMU() {
    dummy_ic = std::make_unique<InterruptController>();
    dummy_cartridge = std::make_unique<Cartridge>();
    dummy_timer = std::make_unique<Timer>(*dummy_ic);
    dummy_ppu = std::make_unique<PPU>(*dummy_ic);

    cartridge = dummy_cartridge.get();
    ic = dummy_ic.get();
    timer = dummy_timer.get();
    ppu = dummy_ppu.get();

    io_regs.fill(0);
    hram.fill(0);
    wram.fill(0);

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

MMU::MMU(Cartridge& cartRef, PPU& ppuRef, Timer& timerRef, InterruptController& icRef)
    : cartridge(&cartRef)
    , ppu(&ppuRef)
    , timer(&timerRef)
    , ic(&icRef)
{
    io_regs.fill(0);
    hram.fill(0);
    wram.fill(0);

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

MMU::MMU(Cartridge& cartRef, PPU& ppuRef, Timer& timerRef, InterruptController& icRef, APU& apuRef)
    : cartridge(&cartRef)
    , ppu(&ppuRef)
    , timer(&timerRef)
    , ic(&icRef)
    , apu(&apuRef)
{
    io_regs.fill(0);
    hram.fill(0);
    wram.fill(0);

    // Post-boot defaults
    write(0xFF00, 0xCF); // Joypad
    write(0xFF04, 0xAB); // DIV
    write(0xFF05, 0x00); // TIMA
    write(0xFF06, 0x00); // TMA
    write(0xFF07, 0xF8); // TAC
    write(0xFF0F, 0xE1); // IF
    // Audio post-boot defaults
    write(0xFF10, 0x80); // NR10
    write(0xFF11, 0xBF); // NR11
    write(0xFF12, 0xF3); // NR12
    write(0xFF14, 0xBF); // NR14
    write(0xFF16, 0x3F); // NR21
    write(0xFF17, 0x00); // NR22
    write(0xFF19, 0xBF); // NR24
    write(0xFF1A, 0x7F); // NR30
    write(0xFF1B, 0xFF); // NR31
    write(0xFF1C, 0x9F); // NR32
    write(0xFF1E, 0xBF); // NR34
    write(0xFF20, 0xFF); // NR41
    write(0xFF21, 0x00); // NR42
    write(0xFF22, 0x00); // NR43
    write(0xFF23, 0xBF); // NR44
    write(0xFF24, 0x77); // NR50
    write(0xFF25, 0xF3); // NR51
    write(0xFF26, 0xF1); // NR52
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
    
    // ROM (0x0000 - 0x7FFF)
    if (address <= 0x7FFF) {
        return cartridge->read(address);
    }
    // VRAM (0x8000 - 0x9FFF)
    if (address <= 0x9FFF) {
        return ppu->read(address);
    }
    // External RAM (0xA000 - 0xBFFF)
    if (address <= 0xBFFF) {
        return cartridge->read(address);
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
        return ppu->read(address);
    }
    // Unusable (0xFEA0 - 0xFEFF)
    if (address <= 0xFEFF) {
        return 0x00;
    }
    // I/O Registers (0xFF00 - 0xFF7F)
    if (address == 0xFF00) return get_joypad_state();
    if (address >= 0xFF04 && address <= 0xFF07) return timer->read(address);
    if (address == 0xFF0F) return ic->read(address);
    if (apu && address >= 0xFF10 && address <= 0xFF3F) return apu->read(address);
    if (address >= 0xFF40 && address <= 0xFF4B) return ppu->read(address);
    if (address >= 0xFF00 && address <= 0xFF7F) return io_regs[address - 0xFF00];
    
    // HRAM (0xFF80 - 0xFFFE)
    if (address <= 0xFFFE) return hram[address - 0xFF80];
    
    // IE (0xFFFF)
    if (address == 0xFFFF) return ic->read(address);

    return 0xFF;
}

void MMU::write(const u16 address, u8 value) {
    write_count++;
    
    // ROM / MBC (0x0000 - 0x7FFF)
    if (address <= 0x7FFF) {
        cartridge->write(address, value);
        return;
    }
    // VRAM (0x8000 - 0x9FFF)
    if (address <= 0x9FFF) {
        ppu->write(address, value);
        return;
    }
    // External RAM (0xA000 - 0xBFFF)
    if (address <= 0xBFFF) {
        cartridge->write(address, value);
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
        ppu->write(address, value);
        return;
    }
    // Unusable (0xFEA0 - 0xFEFF)
    if (address <= 0xFEFF) {
        return;
    }
    // I/O Registers
    if (address == 0xFF00) { joypad_select = value & 0x30; return; }
    if (address >= 0xFF04 && address <= 0xFF07) { timer->write(address, value); return; }
    if (address == 0xFF0F) { ic->write(address, value); return; }
    if (apu && address >= 0xFF10 && address <= 0xFF3F) { apu->write(address, value); return; }
    if (address == 0xFF46) { perform_dma(value); ppu->write(address, value); return; }
    if (address >= 0xFF40 && address <= 0xFF4B) { ppu->write(address, value); return; }
    if (address >= 0xFF00 && address <= 0xFF7F) { io_regs[address - 0xFF00] = value; return; }
    
    // HRAM (0xFF80 - 0xFFFE)
    if (address <= 0xFFFE) { hram[address - 0xFF80] = value; return; }
    
    // IE (0xFFFF)
    if (address == 0xFFFF) { ic->write(address, value); return; }
}

u8 MMU::get_current_rom_bank() const {
    return cartridge->get_current_rom_bank();
}

void MMU::step_timer(int cycles) {
    timer->step(cycles);
}

bool MMU::map_rom(const std::vector<u8>& rom_data) {
    return cartridge->load_rom(rom_data);
}

void MMU::perform_dma(u8 value) {
    u16 source_address = value << 8;
    for (u16 i = 0; i < 0xA0; i++) {
        write(0xFE00 + i, read(source_address + i));
    }
}

u8 MMU::get_joypad_state() const {
    u8 state = 0x0F;
    if (!(joypad_select & 0x10)) state &= direction_buttons;
    if (!(joypad_select & 0x20)) state &= action_buttons;
    return 0xC0 | joypad_select | state;
}

void MMU::set_joypad_state(u8 action, u8 direction) {
    u8 old_state = get_joypad_state();
    action_buttons = action;
    direction_buttons = direction;
    u8 new_state = get_joypad_state();
    // Interrupt requested if any button line goes from 1 (unpressed) to 0 (pressed)
    if ((old_state & ~new_state) & 0x0F) {
        ic->request_interrupt(InterruptType::Joypad);
    }
}

std::vector<u8> MMU::dump_memory() const {
    std::vector<u8> dump(0x10000, 0);
    
    // Copy main arrays
    std::copy(wram.begin(), wram.end(), dump.begin() + 0xC000);
    std::copy(hram.begin(), hram.end(), dump.begin() + 0xFF80);
    std::copy(io_regs.begin(), io_regs.end(), dump.begin() + 0xFF00);
    
    // Copy cartridge & ppu data via read
    for (u16 addr = 0x8000; addr <= 0x9FFF; ++addr) {
        dump[addr] = read(addr);
    }
    for (u16 addr = 0xA000; addr <= 0xBFFF; ++addr) {
        dump[addr] = read(addr);
    }
    for (u16 addr = 0xFE00; addr <= 0xFE9F; ++addr) {
        dump[addr] = read(addr);
    }
    for (u16 addr = 0xFF40; addr <= 0xFF4B; ++addr) {
        dump[addr] = read(addr);
    }

    // Save metadata
    dump[0x00] = cartridge->get_current_rom_bank();
    dump[0x01] = joypad_select;
    dump[0x02] = action_buttons;
    dump[0x03] = direction_buttons;
    dump[0x04] = ic->get_if();
    dump[0x05] = ic->get_ie();
    
    return dump;
}

bool MMU::load_memory(const std::vector<u8>& dump) {
    if (dump.size() != 0x10000) {
        return false;
    }
    
    std::copy(dump.begin() + 0xC000, dump.begin() + 0xE000, wram.begin());
    std::copy(dump.begin() + 0xFF80, dump.begin() + 0xFFFF, hram.begin());
    std::copy(dump.begin() + 0xFF00, dump.begin() + 0xFF80, io_regs.begin());
    
    cartridge->set_current_rom_bank(dump[0x00]);
    joypad_select = dump[0x01];
    action_buttons = dump[0x02];
    direction_buttons = dump[0x03];
    ic->set_if(dump[0x04]);
    ic->set_ie(dump[0x05]);
    
    // Restore state of VRAM, OAM, ERAM and registers
    for (u16 addr = 0x8000; addr <= 0x9FFF; ++addr) {
        write(addr, dump[addr]);
    }
    for (u16 addr = 0xA000; addr <= 0xBFFF; ++addr) {
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

MMU::~MMU() = default;

void MMU::reset() {
    wram.fill(0);
    hram.fill(0);
    io_regs.fill(0);
    
    joypad_select = 0x30;
    action_buttons = 0x0F;
    direction_buttons = 0x0F;
    read_count = 0;
    write_count = 0;

    if (cartridge) cartridge->reset();
    if (timer) timer->reset();
    if (ic) ic->reset();
    if (ppu) ppu->reset();
    if (apu) apu->reset();

    // Post-boot register defaults
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
