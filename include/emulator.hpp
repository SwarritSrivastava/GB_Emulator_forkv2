#pragma once

#include "ProcessingUnit.hpp"
#include "mmu.hpp"
#include "ppu.hpp"
#include "common.hpp"
#include <string>
#include <vector>

class Emulator {
public:
    explicit Emulator(const std::string& romPath);

    void run();
    void stepOnce();

private:
    void loadRom(const std::string& romPath);

    std::vector<u8> romData;
    ProcessingUnit cpu;
    MMU mmu;
    PPU ppu;
    u64 cycles{0};
};
