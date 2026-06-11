#include <gtest/gtest.h>
#include "interrupt_controller.hpp"

class InterruptTest : public ::testing::Test {
protected:
    InterruptController ic;

    void SetUp() override {
        ic.reset();
    }
};

TEST_F(InterruptTest, InitialState) {
    // Top 3 bits of IF are always set to 1 by hardware design
    EXPECT_EQ(ic.get_if(), 0xE1);
    EXPECT_EQ(ic.get_ie(), 0x00);
}

TEST_F(InterruptTest, RequestAndClearInterrupts) {
    ic.request_interrupt(InterruptType::VBlank);
    EXPECT_TRUE((ic.get_if() & static_cast<u8>(InterruptType::VBlank)) != 0);

    ic.clear_interrupt_flag(InterruptType::VBlank);
    EXPECT_TRUE((ic.get_if() & static_cast<u8>(InterruptType::VBlank)) == 0);
}

TEST_F(InterruptTest, PendingInterruptsRespectEnable) {
    ic.request_interrupt(InterruptType::Timer);
    
    // Timer interrupt requested, but not enabled yet in IE
    EXPECT_FALSE(ic.has_pending_interrupts());

    // Enable Timer interrupt
    ic.set_ie(static_cast<u8>(InterruptType::Timer));
    
    // The master CPU should now observe a pending interrupt mapped for execution
    EXPECT_TRUE(ic.has_pending_interrupts());
}

TEST_F(InterruptTest, IOMemoryReadWrites) {
    ic.write(0xFF0F, 0x0A);
    // Unused bits 5-7 are physically tied to HIGH (1)
    EXPECT_EQ(ic.read(0xFF0F), 0xEA); 

    ic.write(0xFFFF, 0x1F);
    EXPECT_EQ(ic.read(0xFFFF), 0x1F);
}
