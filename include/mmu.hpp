#pragma once
#include "common.hpp"
#include <array>
#include <vector>
#include <memory>

class APU;
class Cartridge;
class PPU;
class Timer;
class InterruptController;

class MMU {
private:
    // Ownership pointers for default constructor fallbacks
    std::unique_ptr<Cartridge> dummy_cartridge;
    std::unique_ptr<InterruptController> dummy_ic;
    std::unique_ptr<Timer> dummy_timer;
    std::unique_ptr<PPU> dummy_ppu;

    // References used for routing
    Cartridge* cartridge = nullptr;
    PPU* ppu = nullptr;
    Timer* timer = nullptr;
    InterruptController* ic = nullptr;
    APU* apu = nullptr;

    std::array<u8, 0x2000> wram{}; // Work RAM (0xC000 - 0xDFFF)
    std::array<u8, 0x7F> hram{};   // High RAM (0xFF80 - 0xFFFE)
    std::array<u8, 0x80> io_regs{}; // Other IO registers (0xFF00 - 0xFF7F)
    
    u8 joypad_select = 0x30;
    u8 action_buttons = 0x0F;
    u8 direction_buttons = 0x0F;

    mutable u64 read_count = 0;
    mutable u64 write_count = 0;

    void perform_dma(u8 value);

public:
    MMU();
    MMU(Cartridge& cartridge, PPU& ppu, Timer& timer, InterruptController& ic);
    MMU(Cartridge& cartridge, PPU& ppu, Timer& timer, InterruptController& ic, APU& apu);
    ~MMU();

    [[nodiscard]] u8 read(u16 address) const;
    void write(u16 address, u8 value);

    bool map_rom(const std::vector<u8>& rom_data);
    void reset();

    void step_timer(int cycles);

    // Joypad state interface
    void set_joypad_state(u8 action, u8 direction);
    [[nodiscard]] u8 get_joypad_state() const;

    // Statistics
    [[nodiscard]] u64 get_read_count() const { return read_count; }
    [[nodiscard]] u64 get_write_count() const { return write_count; }
    [[nodiscard]] u8 get_current_rom_bank() const;

    // Serialization
    std::vector<u8> dump_memory() const;
    bool load_memory(const std::vector<u8>& dump);
};
