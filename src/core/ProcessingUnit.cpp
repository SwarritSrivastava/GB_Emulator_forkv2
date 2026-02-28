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
    D = 0x00;
    E = 0xD8;
    H = 0x01;
    L = 0x4D;
    SP = 0xFFFE;
    PC = 0x0100;

    IME = false;
    halt = false;
}

void ProcessingUnit::printStatus() const
{
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout); // copy old format
    std::cout << std::endl;
    std::cout << "--- CPU State ---" << std::endl;
    std::cout << std::hex << std::uppercase << std::setfill('0');
    // Print 16 bit Registers and their high and low counterparts
    std::cout << "AF: " << std::setw(4) << get_af() << " [A:" << std::setw(2) << (int)A << " F:" << std::setw(2) << (int)F << "]" << std::endl;
    std::cout << "BC: " << std::setw(4) << get_bc() << " [B:" << std::setw(2) << (int)B << " C:" << std::setw(2) << (int)C << "]" << std::endl;
    std::cout << "DE: " << std::setw(4) << get_de() << " [D:" << std::setw(2) << (int)D << " E:" << std::setw(2) << (int)E << "]" << std::endl;
    std::cout << "HL: " << std::setw(4) << get_hl() << " [H:" << std::setw(2) << (int)H << " L:" << std::setw(2) << (int)L << "]" << std::endl;
    // Print SP and PC
    std::cout << "SP: " << std::setw(4) << SP << " PC: " << std::setw(4) << PC << std::endl;
    // Print Flag Register Values
    std::cout << "Flags: "
              << (get_flag_z() ? 'Z' : '-')
              << (get_flag_n() ? 'N' : '-')
              << (get_flag_h() ? 'H' : '-')
              << (get_flag_c() ? 'C' : '-')
              << " [IME:" << IME << " HALT:" << halt << "]" << std::endl;
    std::cout << std::endl;
    std::cout.copyfmt(oldState); // set to old format
}

bool ProcessingUnit::isHalt() const{
    return halt;
}
