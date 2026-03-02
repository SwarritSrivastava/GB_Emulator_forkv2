#pragma once

#include "common.hpp"

class ProcessingUnit{
private:
/* -------------------------------- */
/*         16-bit Registers         */
/* -------------------------------- */
    // Hi Lo       Name/Function
    u8 A, F;   // Accumulator & Flags
    u8 B, C;   // BC
    u8 D, E;   // DE
    u8 H, L;   // HL
/* -------------------------------- */
    u16 SP;    // Stack Pointer
    u16 PC;    // Program Counter
/* -------------------------------- */
    bool IME;  // Interrupt Master Enable
    bool halt; // HALT State { When CPU paused, waiting for interrupt}

public:
    ProcessingUnit();
    void reset();
    void printStatus() const; // print task
    u16 get_pc() const;
    // Getting Full 16bit Register Values
    u16 get_bc() const { return (B << 8) | C; }
    u16 get_de() const { return (D << 8) | E; }
    u16 get_hl() const { return (H << 8) | L; }
    u16 get_af() const { return (A << 8) | F; }

    // Flag Helpers (Bit positions: Z=7, N=6, H=5, C=4)
    bool get_flag_z() const { return (F >> 7) & 1; }
    bool get_flag_n() const { return (F >> 6) & 1; }
    bool get_flag_h() const { return (F >> 5) & 1; }
    bool get_flag_c() const { return (F >> 4) & 1; }

    int step(const class MMU& mmu);

    bool isHalt() const;
};
