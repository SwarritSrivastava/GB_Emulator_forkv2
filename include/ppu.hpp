#pragma once

#include "common.hpp"
#include <array>

class MMU;

class PPU {
public:
    PPU();

    void set_mmu(MMU* m) { mmu = m; }
    void step(int cycles);
    u8 read(u16 address) const;
    void write(u16 address, u8 value);

    const u32* get_framebuffer() const { return framebuffer.data(); }
    bool is_frame_ready() const { return frame_ready; }
    void clear_frame_ready() { frame_ready = false; }

private:
    std::array<u8, 0x2000> vram{};
    std::array<u8, 160> oam{};

    u8 lcdc = 0x00;
    u8 stat = 0x00;
    u8 scy = 0x00;
    u8 scx = 0x00;
    u8 ly = 0x00;
    u8 lyc = 0x00;
    u8 dma = 0x00;
    u8 bgp = 0x00;
    u8 obp0 = 0x00;
    u8 obp1 = 0x00;
    u8 wy = 0x00;
    u8 wx = 0x00;

    int mode_clock = 0;
    u8 window_line_counter = 0;
    bool frame_ready = false;
    std::array<u32, 160 * 144> framebuffer{};

    enum class Mode {
        HBlank = 0,
        VBlank = 1,
        OAMSearch = 2,
        PixelTransfer = 3
    };

    Mode get_mode() const { return static_cast<Mode>(stat & 0x03); }
    void set_mode(Mode mode);
    void update_stat();

    void render_scanline();
    void render_sprites();
    u32 get_color(u8 color_id, u16 palette_address) const;

    MMU* mmu = nullptr;
};
