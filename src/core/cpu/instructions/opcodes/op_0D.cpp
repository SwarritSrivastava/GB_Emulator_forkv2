#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_ret_nc(ProcessingUnit& cpu, MMU& mmu) // 0xD0
{
    if (cpu.get_flag_c()) {
        return totalMachineCycles(2);
    }

    const u16 sp = cpu.get_sp();
    const u8 lo = mmu.read(sp);
    const u8 hi = mmu.read(sp + 1);

    cpu.set_sp(sp + 2);
    cpu.set_pc(static_cast<u16>((hi << 8) | lo));

    return totalMachineCycles(5);
}

int op_pop_de(ProcessingUnit& cpu, MMU& mmu) // 0xD1
{
    const u16 sp = cpu.get_sp();
    const u8 lo = mmu.read(sp);
    const u8 hi = mmu.read(sp + 1);

    cpu.reg(ProcessingUnit::Register::E) = lo;
    cpu.reg(ProcessingUnit::Register::D) = hi;
    cpu.set_sp(sp + 2);

    return totalMachineCycles(3);
}

int op_jp_nc(ProcessingUnit& cpu, MMU& mmu) // 0xD2
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    const u16 addr = static_cast<u16>((hi << 8) | lo);

    if (!cpu.get_flag_c())
    {
        cpu.set_pc(addr);
        return totalMachineCycles(4);
    }

    return totalMachineCycles(3);
}

int op_illegal_d3(ProcessingUnit& cpu, MMU& mmu) // 0xD3
{
    throw std::runtime_error("Unimplemented opcode at PC: 0x" + std::to_string(cpu.last_pc));
}

int op_call_nc(ProcessingUnit& cpu, MMU& mmu) // 0xD4
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    const u16 addr = static_cast<u16>((hi << 8) | lo);

    if (!cpu.get_flag_c())
    {
        const u16 pc = cpu.get_pc();
        const u16 sp = cpu.get_sp();

        mmu.write(sp - 1, static_cast<u8>(pc >> 8));
        mmu.write(sp - 2, static_cast<u8>(pc & 0xFF));
        cpu.set_sp(sp - 2);
        cpu.set_pc(addr);

        return totalMachineCycles(6);
    }

    return totalMachineCycles(3);
}

int op_push_de(ProcessingUnit& cpu, MMU& mmu) // 0xD5
{
    const u8 d = cpu.reg(ProcessingUnit::Register::D);
    const u8 e = cpu.reg(ProcessingUnit::Register::E);
    const u16 sp = cpu.get_sp();

    mmu.write(sp - 1, d);
    mmu.write(sp - 2, e);
    cpu.set_sp(sp - 2);

    return totalMachineCycles(4);
}

int op_sub_d8(ProcessingUnit& cpu, MMU& mmu) // 0xD6
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 d8 = mmu.read(cpu.inc_pc());
    const int result = a - d8;
    const int half_carry = (a & 0x0F) - (d8 & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);

    return totalMachineCycles(2);
}

int op_rst_10(ProcessingUnit& cpu, MMU& mmu) // 0xD7
{
    const u16 pc = cpu.get_pc();

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), (pc >> 8) & 0xFF);

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), pc & 0xFF);

    cpu.set_pc(0x10);

    return totalMachineCycles(4);
}

int op_ret_c(ProcessingUnit& cpu, MMU& mmu) // 0xD8
{
    if (!cpu.get_flag_c()) {
        return totalMachineCycles(2);
    }

    const u16 sp = cpu.get_sp();
    const u8 lo = mmu.read(sp);
    const u8 hi = mmu.read(sp + 1);

    cpu.set_sp(sp + 2);
    cpu.set_pc(static_cast<u16>((hi << 8) | lo));

    return totalMachineCycles(5);
}

int op_reti(ProcessingUnit& cpu, MMU& mmu) // 0xD9
{
    const u16 sp = cpu.get_sp();
    const u8 lo = mmu.read(sp);
    const u8 hi = mmu.read(sp + 1);

    cpu.set_sp(sp + 2);
    cpu.set_pc(static_cast<u16>((hi << 8) | lo));
    cpu.setIME(true);

    return totalMachineCycles(4);
}

int op_jp_c(ProcessingUnit& cpu, MMU& mmu) // 0xDA
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    const u16 addr = static_cast<u16>((hi << 8) | lo);

    if (cpu.get_flag_c())
    {
        cpu.set_pc(addr);
        return totalMachineCycles(4);
    }

    return totalMachineCycles(3);
}

int op_illegal_db(ProcessingUnit& cpu, MMU& mmu) // 0xDB
{
    throw std::runtime_error("Unimplemented opcode at PC: 0x" + std::to_string(cpu.last_pc));
}

int op_call_c(ProcessingUnit& cpu, MMU& mmu) // 0xDC
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    const u16 addr = static_cast<u16>((hi << 8) | lo);

    if (cpu.get_flag_c())
    {
        const u16 pc = cpu.get_pc();
        const u16 sp = cpu.get_sp();

        mmu.write(sp - 1, static_cast<u8>(pc >> 8));
        mmu.write(sp - 2, static_cast<u8>(pc & 0xFF));
        cpu.set_sp(sp - 2);
        cpu.set_pc(addr);

        return totalMachineCycles(6);
    }

    return totalMachineCycles(3);
}

int op_illegal_dd(ProcessingUnit& cpu, MMU& mmu) // 0xDD
{
    throw std::runtime_error("Unimplemented opcode at PC: 0x" + std::to_string(cpu.last_pc));
}

int op_sbc_a_d8(ProcessingUnit& cpu, MMU& mmu) // 0xDE
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 d8 = mmu.read(cpu.inc_pc());
    const u8 carry = cpu.get_flag_c() ? 1 : 0;
    const int result = a - d8 - carry;
    const int half_carry = (a & 0x0F) - (d8 & 0x0F) - carry;

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);

    return totalMachineCycles(2);
}

int op_rst_18(ProcessingUnit& cpu, MMU& mmu) // 0xDF
{
    const u16 pc = cpu.get_pc();

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), (pc >> 8) & 0xFF);

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), pc & 0xFF);

    cpu.set_pc(0x18);

    return totalMachineCycles(4);
}
