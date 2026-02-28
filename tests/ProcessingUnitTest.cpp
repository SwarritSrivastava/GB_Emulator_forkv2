#include <gtest/gtest.h>
#include "../include/ProcessingUnit.hpp"


class CPUTest : public ::testing::Test {
protected:
    ProcessingUnit cpu; //Init cpu
};

TEST_F(CPUTest, ResetValuesAreCorrect) {
    // Check 16-bit combined registers
    EXPECT_EQ(cpu.get_af(), 0x01B0);
    EXPECT_EQ(cpu.get_bc(), 0x0013);
    EXPECT_EQ(cpu.get_de(), 0x00D8);
    EXPECT_EQ(cpu.get_hl(), 0x014D);

    // Check individual flags (B0 = 1011 0000)
    EXPECT_TRUE(cpu.get_flag_z());   // Bit 7
    EXPECT_FALSE(cpu.get_flag_n());  // Bit 6
    EXPECT_TRUE(cpu.get_flag_h());   // Bit 5
    EXPECT_TRUE(cpu.get_flag_c());   // Bit 4
}

TEST_F(CPUTest, HaltStateInitial) {
    EXPECT_FALSE(cpu.isHalt());
}