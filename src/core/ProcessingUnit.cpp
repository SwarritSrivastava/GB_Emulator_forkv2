//
// Created by wizard on 25/02/26.
//

#include "../../include/ProcessingUnit.hpp"

ProcessingUnit::ProcessingUnit(){
    reset();
}

void ProcessingUnit::reset(){
    // DMG Default
    A = 0x01;
    F = 0xB0;
    B = 0x00;
    C = 0x13;
    H = 0x01;
    L = 0x4D;
    SP = 0xFFFE;
    PC = 0x0100;

    IME = false;
    halt = false;
}

bool ProcessingUnit::isHalt(){
    return halt;
}
