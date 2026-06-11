#pragma once

#include "common.hpp"

class InterruptController;

class Timer {
public:
    explicit Timer(InterruptController& ic);
    ~Timer() = default;

    void step(int cycles);
    [[nodiscard]] u8 read(u16 address) const;
    void write(u16 address, u8 value);
    void reset();

    // Serialization support
    [[nodiscard]] u16 get_div_counter() const { return div_counter; }
    [[nodiscard]] u8 get_tima() const { return tima; }
    [[nodiscard]] u8 get_tma() const { return tma; }
    [[nodiscard]] u8 get_tac() const { return tac; }
    [[nodiscard]] int get_tima_counter() const { return tima_counter; }

    void set_div_counter(u16 value) { div_counter = value; }
    void set_tima(u8 value) { tima = value; }
    void set_tma(u8 value) { tma = value; }
    void set_tac(u8 value) { tac = value; }
    void set_tima_counter(int value) { tima_counter = value; }

private:
    InterruptController& ic;
    u16 div_counter = 0;
    u8 tima = 0;
    u8 tma = 0;
    u8 tac = 0;
    int tima_counter = 0;
};
