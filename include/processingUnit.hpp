//
// Created by wizard on 25/02/26.
//
#include "common.hpp"
#pragma once

class processingUnit{
public:
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
    processingUnit();
    void reset();
};