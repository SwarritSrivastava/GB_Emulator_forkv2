#pragma once
#include "common.hpp"

class MMU {
    private:
        u8 memory[0x10000];

    public:
        MMU();

        u8 read(u16 address);
        void write(u16 address, u8 value);
};