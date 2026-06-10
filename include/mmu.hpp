#pragma once
#include "common.hpp"
#include "timer.hpp"
#include <array>
#include <vector>

class PPU;

class MMU {
    private:
        std::vector<u8> full_rom_data;
        u8 current_rom_bank = 1;

        std::array<u8, 0x2000> wram{}; // (0xC000 - 0xDFFF)
        std::array<u8, 0x2000> eram{}; // (0xA000 - 0xBFFF) External RAM
        std::array<u8, 0x2000> vram_fallback{}; // VRAM fallback when no PPU
        std::array<u8, 0xA0> oam_fallback{}; // OAM fallback when no PPU
        std::array<u8, 0x0C> ppu_reg_fallback{}; // PPU register fallback (FF40-FF4B)
        std::array<u8, 0x7F> hram{}; // (0xFF80 - 0xFFFE)
        std::array<u8, 0x80> io_regs{}; // (0xFF00 - 0xFF7F)
        
        u8 joypad_select = 0x30;
        u8 action_buttons = 0x0F;
        u8 direction_buttons = 0x0F;
        u8 interrupt_flag = 0xE1;
        u8 interrupt_enable = 0x00;

        PPU* ppu = nullptr;
        Timer timer;

        void perform_dma(u8 value);

    public:
        MMU();

        void set_ppu(PPU* p) { ppu = p; }
        void step_timer(int cycles) { timer.step(cycles, *this); }

        bool map_rom(const std::vector<u8>& rom_data);
        u8 read(u16 address) const;
        void write(u16 address, u8 value);
        
        u8 get_joypad_state() const {
            u8 state = 0x0F;
            if (!(joypad_select & 0x10)) state &= direction_buttons;
            if (!(joypad_select & 0x20)) state &= action_buttons;
            return 0xC0 | joypad_select | state;
        }

        void set_joypad_state(u8 action, u8 direction) {
            u8 old_state = get_joypad_state();
            action_buttons = action;
            direction_buttons = direction;
            u8 new_state = get_joypad_state();
            // Interrupt requested if any button line goes from 1 (unpressed) to 0 (pressed)
            if ((old_state & ~new_state) & 0x0F) {
                interrupt_flag |= 0x10;
            }
        }
};
