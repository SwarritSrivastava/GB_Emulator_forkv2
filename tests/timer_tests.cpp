#include <gtest/gtest.h>
#include "timer.hpp"
#include "interrupt_controller.hpp"

class TimerTest : public ::testing::Test {
protected:
    InterruptController ic;
    Timer timer{ic};

    void SetUp() override {
        timer.reset();
        ic.reset();
    }
};

TEST_F(TimerTest, InitialState) {
    EXPECT_EQ(timer.get_div_counter(), 0);
    EXPECT_EQ(timer.get_tima(), 0);
    EXPECT_EQ(timer.get_tma(), 0);
    EXPECT_EQ(timer.get_tac(), 0);
}

TEST_F(TimerTest, DivIncrementsIndependently) {
    // DIV is the upper 8 bits of a 16-bit internal counter, incrementing every 256 cycles.
    timer.step(255);
    EXPECT_EQ(timer.read(0xFF04), 0);
    timer.step(1);
    EXPECT_EQ(timer.read(0xFF04), 1);
    timer.step(256);
    EXPECT_EQ(timer.read(0xFF04), 2);
}

TEST_F(TimerTest, DivResetOnWrite) {
    timer.step(256);
    EXPECT_EQ(timer.read(0xFF04), 1);
    timer.write(0xFF04, 0x55); // Writing any value to DIV immediately resets it to 0
    EXPECT_EQ(timer.read(0xFF04), 0);
}

TEST_F(TimerTest, TimaIncrementsWhenEnabled) {
    timer.write(0xFF07, 0x04); // Enable timer (bit 2) and set frequency to 4096Hz (clock/1024)
    timer.step(1023);
    EXPECT_EQ(timer.read(0xFF05), 0);
    timer.step(1);
    EXPECT_EQ(timer.read(0xFF05), 1);
}

TEST_F(TimerTest, TimaOverflowsAndGeneratesInterrupt) {
    timer.write(0xFF07, 0x04); // Enable, freq = clock/1024
    timer.write(0xFF06, 0x42); // TMA = 0x42
    timer.write(0xFF05, 0xFF); // TIMA = 0xFF

    timer.step(1024);
    
    // TIMA should overflow, reset to TMA, and seamlessly trigger the Timer hardware interrupt
    EXPECT_EQ(timer.read(0xFF05), 0x42);
    EXPECT_TRUE((ic.get_if() & 0x04) != 0);
}

TEST_F(TimerTest, TimaDoesNotIncrementWhenDisabled) {
    timer.write(0xFF07, 0x00); // Disable timer completely
    timer.write(0xFF05, 0x00);

    timer.step(4096);
    
    EXPECT_EQ(timer.read(0xFF05), 0); // TIMA absolutely should not change
}
