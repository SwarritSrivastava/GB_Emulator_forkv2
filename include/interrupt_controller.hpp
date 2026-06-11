#pragma once
#include "common.hpp"

enum class InterruptType : u8 {
    VBlank   = 0x01,
    LCDStat  = 0x02,
    Timer    = 0x04,
    Serial   = 0x08,
    Joypad   = 0x10
};

class InterruptController {
public:
    InterruptController() = default;
    ~InterruptController() = default;

    [[nodiscard]] u8 read(u16 address) const {
        if (address == 0xFF0F) return interrupt_flag | 0xE0;
        if (address == 0xFFFF) return interrupt_enable;
        return 0xFF;
    }

    void write(u16 address, u8 value) {
        if (address == 0xFF0F) {
            interrupt_flag = value | 0xE0;
        } else if (address == 0xFFFF) {
            interrupt_enable = value;
        }
    }

    void request_interrupt(InterruptType type) {
        interrupt_flag |= static_cast<u8>(type);
    }

    void clear_interrupt_flag(InterruptType type) {
        interrupt_flag &= ~static_cast<u8>(type);
    }

    [[nodiscard]] bool has_pending_interrupts() const {
        return (interrupt_flag & interrupt_enable & 0x1F) != 0;
    }
    
    [[nodiscard]] u8 get_if() const { return interrupt_flag; }
    [[nodiscard]] u8 get_ie() const { return interrupt_enable; }

    void set_if(u8 value) { interrupt_flag = value | 0xE0; }
    void set_ie(u8 value) { interrupt_enable = value; }

    void reset() {
        interrupt_flag = 0xE1;
        interrupt_enable = 0x00;
    }

private:
    u8 interrupt_flag = 0xE1;
    u8 interrupt_enable = 0x00;
};
