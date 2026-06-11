#include "../../../include/ProcessingUnit.hpp"
#include "../../../include/mmu.hpp"
#include "../../../include/opcode_table.hpp"

ProcessingUnit::ProcessingUnit(){
    reset();
}

void ProcessingUnit::reset(){
    A = 0x01; F = 0xB0; B = 0x00; C = 0x13; D = 0x00; E = 0xD8; H = 0x01; L = 0x4D;
    SP = 0xFFFE; PC = 0x0100;
    IME = false; halt = false;
}

int ProcessingUnit::step(MMU &mmu) {
    last_instr_cycles = 0;
    check_interrupts(mmu);
    if (last_instr_cycles > 0) return last_instr_cycles;

    if (halt) {
        return 4;
    }

    last_pc = PC;
    const u8 opcode = mmu.read(PC++);
    try {
        return instructionTable[opcode](*this, mmu);
    } catch (...) {
        // Illegal opcode — treat as NOP and keep going
        return 4;
    }
}

void ProcessingUnit::check_interrupts(MMU &mmu) {
    u8 if_reg = mmu.read(0xFF0F);
    u8 ie_reg = mmu.read(0xFFFF);
    u8 pending = if_reg & ie_reg & 0x1F;

    if (pending > 0) {
        halt = false;
    }

    if (!IME || pending == 0) return;

    if (pending & 0x01) execute_interrupt(mmu, 0x0040, 0);
    else if (pending & 0x02) execute_interrupt(mmu, 0x0048, 1);
    else if (pending & 0x04) execute_interrupt(mmu, 0x0050, 2);
    else if (pending & 0x08) execute_interrupt(mmu, 0x0058, 3);
    else if (pending & 0x10) execute_interrupt(mmu, 0x0060, 4);
}

void ProcessingUnit::execute_interrupt(MMU &mmu, u16 address, int bit) {
    IME = false;
    u8 if_reg = mmu.read(0xFF0F);
    mmu.write(0xFF0F, if_reg & ~(1 << bit));

    mmu.write(--SP, (PC >> 8) & 0xFF);
    mmu.write(--SP, PC & 0xFF);
    PC = address;
    last_instr_cycles = 20;
}

void ProcessingUnit::printStatus() const {}
u16 ProcessingUnit::get_pc() const { return PC; }
u16 ProcessingUnit::get_sp() const { return SP; }
void ProcessingUnit::setHalt(bool newValue) { halt = newValue; }
bool ProcessingUnit::isHalt() const { return halt; }
void ProcessingUnit::setIME(bool newValue) { IME = newValue; }
bool ProcessingUnit::getIME() const { return IME; }
u8& ProcessingUnit::reg(Register r) {
    switch (r) {
        case Register::B: return B; case Register::C: return C;
        case Register::D: return D; case Register::E: return E;
        case Register::H: return H; case Register::L: return L;
        case Register::A: return A; case Register::F: return F;
    }
    throw std::runtime_error("Invalid register");
}
const u8& ProcessingUnit::reg(Register r) const { return const_cast<ProcessingUnit*>(this)->reg(r); }
u16 ProcessingUnit::inc_pc() { return PC++; }
