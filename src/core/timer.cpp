#include "../../include/timer.hpp"
#include "../../include/mmu.hpp"

void Timer::step(int cycles, MMU& mmu) {
    div_counter += cycles;
    
    if (tac & 0x04) { // Timer enabled
        tima_counter += cycles;
        int threshold = 0;
        switch (tac & 0x03) {
            case 0: threshold = 1024; break; // 4096 Hz
            case 1: threshold = 16;   break; // 262144 Hz
            case 2: threshold = 64;   break; // 65536 Hz
            case 3: threshold = 256;  break; // 16384 Hz
        }

        while (tima_counter >= threshold) {
            tima_counter -= threshold;
            if (tima == 0xFF) {
                tima = tma;
                // Request Timer Interrupt
                u8 if_reg = mmu.read(0xFF0F);
                mmu.write(0xFF0F, if_reg | 0x04);
            } else {
                tima++;
            }
        }
    }
}

u8 Timer::read(u16 address) const {
    switch (address) {
        case 0xFF04: return (div_counter >> 8) & 0xFF;
        case 0xFF05: return tima;
        case 0xFF06: return tma;
        case 0xFF07: return tac;
    }
    return 0xFF;
}

void Timer::write(u16 address, u8 value) {
    switch (address) {
        case 0xFF04: div_counter = 0; break;
        case 0xFF05: tima = value; break;
        case 0xFF06: tma = value; break;
        case 0xFF07: tac = value; break;
    }
}
