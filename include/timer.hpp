#pragma once

#include "common.hpp"

class MMU;

class Timer {
public:
    void step(int cycles, MMU& mmu);
    u8 read(u16 address) const;
    void write(u16 address, u8 value);

private:
    u16 div_counter = 0;
    u8 tima = 0;
    u8 tma = 0;
    u8 tac = 0;
    int tima_counter = 0;
};
