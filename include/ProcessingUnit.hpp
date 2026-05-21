#pragma once

#include "common.hpp"

class MMU;

class ProcessingUnit{
private:
/* -------------------------------- */
/*         16-bit Registers         */
/* -------------------------------- */
    // Hi Lo       Name/Function
    u8 A{}, F{};   // Accumulator & Flags
    u8 B{}, C{};   // BC
    u8 D{}, E{};   // DE
    u8 H{}, L{};   // HL
/* -------------------------------- */
    u16 SP{};    // Stack Pointer
    u16 PC{};    // Program Counter
/* -------------------------------- */
    bool IME{};  // Interrupt Master Enable
    bool halt{}; // HALT State { When CPU paused, waiting for interrupt}

public:
    ProcessingUnit();
    void reset();
    void printStatus() const; // print task
    [[nodiscard]] u16 get_pc() const;
    void set_pc(u16 value) { PC = value; }
    [[nodiscard]] u16 get_sp() const;
    void set_sp(u16 value) { SP = value; }
    // Getting Full 16bit Register Values
    [[nodiscard]] u16 get_bc() const { return (B << 8) | C; }
    [[nodiscard]] u16 get_de() const { return (D << 8) | E; }
    [[nodiscard]] u16 get_hl() const { return (H << 8) | L; }
    [[nodiscard]] u16 get_af() const { return (A << 8) | F; }

    // Flag Helpers (Bit positions: Z=7, N=6, H=5, C=4)
    [[nodiscard]] bool get_flag_z() const { return (F >> 7) & 1; }
    [[nodiscard]] bool get_flag_n() const { return (F >> 6) & 1; }
    [[nodiscard]] bool get_flag_h() const { return (F >> 5) & 1; }
    [[nodiscard]] bool get_flag_c() const { return (F >> 4) & 1; }

    int step(MMU &mmu);

    void setHalt(bool newValue);

    [[nodiscard]] bool isHalt() const;

    void setIME(bool newValue);
    [[nodiscard]] bool getIME() const;
/* -------------------------------- */
/*        Register Identifiers      */
/* -------------------------------- */
    enum class Register {
        B, C, D, E, H, L, A, F
    };
/* -------------------------------- */
/*              Flags               */
/* -------------------------------- */
    enum class Flag {
        Z = 7,
        N = 6,
        H = 5,
        C = 4
    };
/* -------------------------------- */
/*         Register Access          */
/* -------------------------------- */
    u8& reg(Register r);
    [[nodiscard]] const u8& reg(Register r) const;

    u16 inc_pc();
/* -------------------------------- */
/*           FLAGS Access           */
/* -------------------------------- */
    void clearFlags() {
        F = 0;
    }
    void normalizeFlags()
    {
        F = 0xF0;
    }
    void setFlag(Flag flag, bool value)
    {
        const u8 mask = 1 << static_cast<int>(flag);
        if (value) F |= mask;   // set bit
        else F &= ~mask;  // clear bit
    }

    bool stop{};

    void setStop(const bool value) { stop = value; }
    [[nodiscard]] bool isStop() const { return stop; }
};
