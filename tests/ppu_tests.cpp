#include <gtest/gtest.h>
#include "../include/ppu.hpp"

TEST(PPUTest, InitialState) {
    PPU ppu;
    EXPECT_EQ(ppu.read(0xFF44), 0); // LY should be 0
    // Initial stat is 0x85 (1000 0101) -> Mode 1 (VBlank) or Mode 1 based on my init?
    // In my impl: u8 stat = 0x85; -> Mode 1.
    EXPECT_EQ(ppu.read(0xFF41) & 0x03, 1); 
}

TEST(PPUTest, ModeTransition) {
    PPU ppu;
    // Set LCDC to enabled (bit 7)
    ppu.write(0xFF40, 0x80);
    
    // After reset/enable, it should be in Mode 2 (OAM Search) or follow the clock.
    // Let's force a step.
    ppu.step(4); 
    
    // In my code, if Mode was VBlank (1) and we step, if ly > 153 it goes to OAMSearch.
    // Initial LY is 0. 
    // Let's see: if get_mode() is VBlank and mode_clock >= 456...
    
    // To properly test, let's just observe LY advancement.
    // One scanline is 456 cycles.
    ppu.step(456);
    EXPECT_EQ(ppu.read(0xFF44), 1); // LY should increment
}

TEST(PPUTest, VBlankTrigger) {
    PPU ppu;
    ppu.write(0xFF40, 0x80); // Enable LCD
    
    // PPU starts in VBlank mode (stat=0x85) at LY=0.
    // Run through the initial VBlank period until LY wraps to 0 and OAM starts.
    // VBlank runs LY 0..153 (154 lines of 456 cycles each) before wrapping.
    for (int i = 0; i < 154; ++i) {
        for (int j = 0; j < 114; ++j) {
            ppu.step(4);
        }
    }
    EXPECT_EQ(ppu.read(0xFF44), 0);
    EXPECT_EQ(ppu.read(0xFF41) & 0x03, 2); // OAM Search mode

    // Now advance 144 scanlines (LY 0..143) through OAM/PT/HBlank
    for (int i = 0; i < 144; ++i) {
        for (int j = 0; j < 114; ++j) {
            ppu.step(4);
        }
    }
    
    EXPECT_EQ(ppu.read(0xFF44), 144);
    EXPECT_EQ(ppu.read(0xFF41) & 0x03, 1); // Should be in VBlank (Mode 1)
    EXPECT_TRUE(ppu.is_frame_ready());
}
