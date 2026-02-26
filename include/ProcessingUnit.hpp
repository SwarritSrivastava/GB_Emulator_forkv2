//
// Created by wizard on 25/02/26.
//
#pragma once
#include "common.hpp"

class ProcessingUnit{
private:
/* -------------------------------- */
/*         16-bit Registers         */
/* -------------------------------- */
    // Hi Lo       Name/Function
    u8 A, F;    // Accumulator & Flags
    u8 B, C;    // BC
    u8 D, E;    // DE
    u8 H, L;    // HL
/* -------------------------------- */
    u16 SP;     // Stack Pointer
    u16 PC;     // Program Counter
/* -------------------------------- */
    bool IME;   // Interrupt Master Enable
    bool halt;  // HALT State { When CPU paused, waiting for interrupt}

public:
    ProcessingUnit();
    void reset();

bool isHalt();

bool isHalt() const;
};