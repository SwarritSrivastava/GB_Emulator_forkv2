#include "../../include/ProcessingUnit.hpp"

#include "mmu.hpp"
#include <fstream>

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
    std::cout << "AF: " << std::setw(4) << get_af() << " [A:" << std::setw(2) << static_cast<int>(A) << " F:" << std::setw(2) << static_cast<int>(F) << "]" << std::endl;
    std::cout << "BC: " << std::setw(4) << get_bc() << " [B:" << std::setw(2) << static_cast<int>(B) << " C:" << std::setw(2) << static_cast<int>(C) << "]" << std::endl;
    std::cout << "DE: " << std::setw(4) << get_de() << " [D:" << std::setw(2) << static_cast<int>(D) << " E:" << std::setw(2) << static_cast<int>(E) << "]" << std::endl;
    std::cout << "HL: " << std::setw(4) << get_hl() << " [H:" << std::setw(2) << static_cast<int>(H) << " L:" << std::setw(2) << static_cast<int>(L) << "]" << std::endl;
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

int ProcessingUnit::step(const class MMU &mmu)
{
    if (isHalt()) {
        return 4; // Minimum instruction time = 4 cycles
    }

    const u8 opcode = mmu.read(PC); // fetch opcode
    PC++; // increment

    int cycles = 0;
    std::ofstream file("opcodes.txt", std::ios::app);
    // std::cout << "Opcode: 0x" << std::hex << static_cast<int>(opcode) << std::endl;
    if (!file.is_open())
    {
        std::cerr << "Failed to open file\n";
        return 1;
    }
    file << "Opcode: 0x" << std::hex << static_cast<int>(opcode) << std::endl;

    cycles = 4;
    return cycles;
}

bool ProcessingUnit::isHalt() const{
    return halt;
}
