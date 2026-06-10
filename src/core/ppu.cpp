#include "../../include/ppu.hpp"
#include "../../include/mmu.hpp"
#include <iostream>

PPU::PPU() {
    framebuffer.fill(0xFFFFFFFF);
    lcdc = 0x91;
    stat = 0x85;
    bgp = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;
}

void PPU::step(int cycles) {
    if (!(lcdc & 0x80)) {
        ly = 0;
        window_line_counter = 0;
        mode_clock = 0;
        set_mode(Mode::HBlank);
        return;
    }

    mode_clock += cycles;

    switch (get_mode()) {
        case Mode::OAMSearch:
            if (mode_clock >= 80) {
                mode_clock -= 80;
                set_mode(Mode::PixelTransfer);
            }
            break;
        case Mode::PixelTransfer:
            if (mode_clock >= 172) {
                mode_clock -= 172;
                set_mode(Mode::HBlank);
                render_scanline();
                render_sprites();
            }
            break;
        case Mode::HBlank:
            if (mode_clock >= 204) {
                mode_clock -= 204;
                ly++;
                if (ly == 144) {
                    set_mode(Mode::VBlank);
                    frame_ready = true;
                } else {
                    set_mode(Mode::OAMSearch);
                }
                update_stat();
            }
            break;
        case Mode::VBlank:
            if (mode_clock >= 456) {
                mode_clock -= 456;
                ly++;
                if (ly > 153) {
                    ly = 0;
                    window_line_counter = 0;
                    set_mode(Mode::OAMSearch);
                }
                update_stat();
            }
            break;
    }
}

u8 PPU::read(u16 address) const {
    if (address >= 0x8000 && address <= 0x9FFF) return vram[address - 0x8000];
    if (address >= 0xFE00 && address <= 0xFE9F) return oam[address - 0xFE00];
    switch (address) {
        case 0xFF40: return lcdc;
        case 0xFF41: return stat;
        case 0xFF42: return scy;
        case 0xFF43: return scx;
        case 0xFF44: return ly;
        case 0xFF45: return lyc;
        case 0xFF46: return dma;
        case 0xFF47: return bgp;
        case 0xFF48: return obp0;
        case 0xFF49: return obp1;
        case 0xFF4A: return wy;
        case 0xFF4B: return wx;
    }
    return 0xFF;
}

void PPU::write(u16 address, u8 value) {
    if (address >= 0x8000 && address <= 0x9FFF) { vram[address - 0x8000] = value; return; }
    if (address >= 0xFE00 && address <= 0xFE9F) { oam[address - 0xFE00] = value; return; }
    switch (address) {
        case 0xFF40: lcdc = value; break;
        case 0xFF41: stat = (stat & 0x07) | (value & 0xF8); break;
        case 0xFF42: scy = value; break;
        case 0xFF43: scx = value; break;
        case 0xFF44: ly = 0; break;
        case 0xFF45: lyc = value; break;
        case 0xFF46: dma = value; break;
        case 0xFF47: bgp = value; break;
        case 0xFF48: obp0 = value; break;
        case 0xFF49: obp1 = value; break;
        case 0xFF4A: wy = value; break;
        case 0xFF4B: wx = value; break;
    }
}

void PPU::set_mode(Mode mode) {
    stat = (stat & ~0x03) | static_cast<u8>(mode);
    if (mmu) {
        if (mode == Mode::VBlank) {
            u8 if_reg = mmu->read(0xFF0F);
            mmu->write(0xFF0F, if_reg | 0x01);
        }
        bool interrupt = false;
        if (mode == Mode::HBlank && (stat & 0x08)) interrupt = true;
        if (mode == Mode::VBlank && (stat & 0x10)) interrupt = true;
        if (mode == Mode::OAMSearch && (stat & 0x20)) interrupt = true;
        if (interrupt) {
            u8 if_reg = mmu->read(0xFF0F);
            mmu->write(0xFF0F, if_reg | 0x02);
        }
    }
}

void PPU::update_stat() {
    if (ly == lyc) {
        stat |= 0x04;
        if (mmu && (stat & 0x40)) {
            u8 if_reg = mmu->read(0xFF0F);
            mmu->write(0xFF0F, if_reg | 0x02);
        }
    } else {
        stat &= ~0x04;
    }
}

void PPU::render_scanline() {
    bool bg_enabled = (lcdc & 0x01);
    bool window_enabled = (lcdc & 0x20) && (ly >= wy);
    if (!bg_enabled && !window_enabled) return;
    u16 tile_data = (lcdc & 0x10) ? 0x8000 : 0x8800;
    bool unsigned_addressing = (lcdc & 0x10);
    u16 bg_map = (lcdc & 0x08) ? 0x9C00 : 0x9800;
    u16 win_map = (lcdc & 0x40) ? 0x9C00 : 0x9800;
    bool window_drawn_this_line = false;
    for (int pixel = 0; pixel < 160; pixel++) {
        u16 current_map; u8 x_pos, y_pos;
        bool is_window = window_enabled && (pixel + 7 >= wx);
        if (is_window) {
            current_map = win_map; x_pos = pixel + 7 - wx; y_pos = window_line_counter;
            window_drawn_this_line = true;
        } else if (bg_enabled) {
            current_map = bg_map; x_pos = pixel + scx; y_pos = ly + scy;
        } else continue;
        u16 tile_row = (u16)(y_pos / 8) * 32;
        u16 tile_col = (x_pos / 8);
        u16 tile_address = current_map + tile_row + tile_col;
        int16_t tile_num = unsigned_addressing ? vram[tile_address - 0x8000] : (int8_t)vram[tile_address - 0x8000];
        u16 tile_location = tile_data + (unsigned_addressing ? (tile_num * 16) : ((tile_num + 128) * 16));
        u8 line = (y_pos % 8) * 2;
        u8 d1 = vram[tile_location + line - 0x8000], d2 = vram[tile_location + line + 1 - 0x8000];
        int bit = 7 - (x_pos % 8);
        u8 color_id = (BIT(d2, bit) << 1) | BIT(d1, bit);
        framebuffer[ly * 160 + pixel] = get_color(color_id, 0xFF47);
    }
    if (window_drawn_this_line) window_line_counter++;
}

void PPU::render_sprites() {
    if (!(lcdc & 0x02)) return;
    bool use8x16 = (lcdc & 0x04);
    for (int i = 0; i < 40; i++) {
        u8 index = i * 4;
        u8 y_pos = oam[index] - 16, x_pos = oam[index + 1] - 8, tile_num = oam[index + 2], flags = oam[index + 3];
        if (ly >= y_pos && ly < (y_pos + (use8x16 ? 16 : 8))) {
            u8 line = ly - y_pos;
            if (flags & 0x40) line = (use8x16 ? 15 : 7) - line;
            u16 tile_location = 0x8000 + (tile_num * 16) + (line * 2);
            u8 d1 = vram[tile_location - 0x8000], d2 = vram[tile_location + 1 - 0x8000];
            for (int tile_pixel = 7; tile_pixel >= 0; tile_pixel--) {
                int bit = (flags & 0x20) ? 7 - tile_pixel : tile_pixel;
                u8 color_id = (BIT(d2, bit) << 1) | BIT(d1, bit);
                u16 palette = (flags & 0x10) ? 0xFF49 : 0xFF48;
                if (color_id != 0 && (x_pos + (7 - tile_pixel)) < 160) {
                    framebuffer[ly * 160 + x_pos + (7 - tile_pixel)] = get_color(color_id, palette);
                }
            }
        }
    }
}

u32 PPU::get_color(u8 color_id, u16 palette_address) const {
    u8 p = read(palette_address);
    u8 color = (BIT(p, color_id * 2 + 1) << 1) | BIT(p, color_id * 2);
    switch (color) {
        case 0: return 0xFFFFFFFF;
        case 1: return 0xFF969696;
        case 2: return 0xFF555555;
        case 3: return 0xFF000000;
    }
    return 0xFFFFFFFF;
}
