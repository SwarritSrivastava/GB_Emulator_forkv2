#include <gtest/gtest.h>
#include "apu.hpp"
#include <array>
#include <vector>

class APUTest : public ::testing::Test {
protected:
    APU apu;

    void SetUp() override {
        apu.reset();
    }
};

TEST_F(APUTest, InitialState) {
    // Master volume should be 1.0f, muted false
    EXPECT_FLOAT_EQ(apu.get_volume(), 1.0f);
    EXPECT_FALSE(apu.is_muted());

    // Channels volume should be 1.0f, muted false
    for (int i = 0; i < 4; i++) {
        EXPECT_FLOAT_EQ(apu.get_channel_volume(i), 1.0f);
        EXPECT_FALSE(apu.is_channel_muted(i));
    }

    // Default NR50, NR51, NR52 values on reset
    EXPECT_EQ(apu.read(0xFF24), 0x77);
    EXPECT_EQ(apu.read(0xFF25), 0xF3);
    // NR52 high bit (power on) should be set, other channels disabled
    EXPECT_EQ(apu.read(0xFF26), 0xF0); 
}

TEST_F(APUTest, ChannelVolumeMuteSetters) {
    // Test master volume
    apu.set_volume(0.5f);
    EXPECT_FLOAT_EQ(apu.get_volume(), 0.5f);

    apu.set_muted(true);
    EXPECT_TRUE(apu.is_muted());

    // Test per-channel volume
    apu.set_channel_volume(0, 0.2f);
    apu.set_channel_volume(1, 0.4f);
    apu.set_channel_volume(2, 0.6f);
    apu.set_channel_volume(3, 0.8f);

    EXPECT_FLOAT_EQ(apu.get_channel_volume(0), 0.2f);
    EXPECT_FLOAT_EQ(apu.get_channel_volume(1), 0.4f);
    EXPECT_FLOAT_EQ(apu.get_channel_volume(2), 0.6f);
    EXPECT_FLOAT_EQ(apu.get_channel_volume(3), 0.8f);

    // Out of bounds channel volume sets should be ignored or return defaults
    apu.set_channel_volume(-1, 0.0f);
    apu.set_channel_volume(4, 0.0f);
    EXPECT_FLOAT_EQ(apu.get_channel_volume(-1), 1.0f);
    EXPECT_FLOAT_EQ(apu.get_channel_volume(4), 1.0f);

    // Test per-channel mute
    apu.set_channel_muted(1, true);
    apu.set_channel_muted(3, true);
    EXPECT_FALSE(apu.is_channel_muted(0));
    EXPECT_TRUE(apu.is_channel_muted(1));
    EXPECT_FALSE(apu.is_channel_muted(2));
    EXPECT_TRUE(apu.is_channel_muted(3));
}

TEST_F(APUTest, RegisterReadWrite) {
    // Channel 1 Sweep register: NRx0
    apu.write(0xFF10, 0x5C); // Period=5, Negate=1, Shift=4
    // Read back should have unused bit 7 set (0x80)
    EXPECT_EQ(apu.read(0xFF10), 0x5C | 0x80);

    // Channel 1 Length/Duty: NR11
    apu.write(0xFF11, 0x80); // Duty=2 (10), Length=0
    EXPECT_EQ(apu.read(0xFF11), 0x80 | 0x3F); // Unused bits 0-5 read as 1

    // Wave RAM read/write (0xFF30 - 0xFF3F)
    for (u16 addr = 0xFF30; addr <= 0xFF3F; addr++) {
        apu.write(addr, static_cast<u8>(addr - 0xFF30 + 0xAA));
    }
    for (u16 addr = 0xFF30; addr <= 0xFF3F; addr++) {
        EXPECT_EQ(apu.read(addr), static_cast<u8>(addr - 0xFF30 + 0xAA));
    }
}

TEST_F(APUTest, PowerToggle) {
    // Enable some channel properties
    apu.write(0xFF10, 0x55);
    apu.write(0xFF12, 0xF3); // Envelope initial=15, period=3
    
    // Turn off power using NR52 (bit 7 = 0)
    apu.write(0xFF26, 0x00);
    EXPECT_EQ(apu.read(0xFF26) & 0x80, 0);

    // All registers 0xFF10 - 0xFF25 should read as 0 (with read-back masks applied)
    EXPECT_EQ(apu.read(0xFF10), 0x80); // 0 | 0x80 mask
    EXPECT_EQ(apu.read(0xFF12), 0x00); // 0 | 0x00 mask

    // While power is off, writes to other registers should be ignored
    apu.write(0xFF10, 0x55);
    EXPECT_EQ(apu.read(0xFF10), 0x80);

    // Turn power back on
    apu.write(0xFF26, 0x80);
    EXPECT_EQ(apu.read(0xFF26) & 0x80, 0x80);
}

TEST_F(APUTest, SampleGenerationRingBuffer) {
    // Set volume to 0 to easily test silence generation
    apu.set_volume(0.0f);
    
    // Step enough cycles to generate at least one sample
    // CPU_CLOCK (4194304) / SAMPLE_RATE (44100) = ~95 cycles per sample
    apu.step(200);

    std::array<std::int16_t, 10> buffer{};
    std::size_t read = apu.read_samples(buffer.data(), 10);
    
    // Should have read at least 2 samples (1 stereo frame: left + right)
    EXPECT_GE(read, 2);
    EXPECT_EQ(buffer[0], 0);
    EXPECT_EQ(buffer[1], 0);
}
