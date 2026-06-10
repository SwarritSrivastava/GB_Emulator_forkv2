#include "../../include/ppu.hpp"
#include "../../include/mmu.hpp"
#include "../../include/cartridge.hpp"
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace {
constexpr u32 screen_width = 160;
constexpr u32 screen_height = 144;
constexpr u32 ui_width = 560;
constexpr u32 ui_height = 420;
constexpr u32 window_width = screen_width + ui_width + 64;
constexpr u32 window_height = 720;
constexpr size_t opcode_log_size = 28;
constexpr float status_display_seconds = 2.5f;
constexpr size_t rom_bytes_per_row = 16;
constexpr size_t rom_rows_visible = 10;

std::string mbcNameFromType(u8 type) {
    switch (type) {
        case 0x00: return "ROM ONLY";
        case 0x01: return "MBC1";
        case 0x02: return "MBC1+RAM";
        case 0x03: return "MBC1+RAM+BATTERY";
        case 0x05: return "MBC2";
        case 0x06: return "MBC2+BATTERY";
        case 0x08: return "ROM+RAM";
        case 0x09: return "ROM+RAM+BATTERY";
        case 0x0F: return "MBC3+TIMER+BATTERY";
        case 0x10: return "MBC3+TIMER+RAM+BATTERY";
        case 0x11: return "MBC3";
        case 0x12: return "MBC3+RAM";
        case 0x13: return "MBC3+RAM+BATTERY";
        case 0x19: return "MBC5";
        case 0x1A: return "MBC5+RAM";
        case 0x1B: return "MBC5+RAM+BATTERY";
        case 0x1C: return "MBC5+RUMBLE";
        case 0x1D: return "MBC5+RUMBLE+RAM";
        case 0x1E: return "MBC5+RUMBLE+RAM+BATTERY";
        default: return "Unknown";
    }
}
}

PPU::PPU()
    : screenSprite(screenTexture)
{
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
        const int index = i * 4;
        const int y_pos = static_cast<int>(oam[index]) - 16;
        const int x_pos = static_cast<int>(oam[index + 1]) - 8;
        u8 tile_num = oam[index + 2];
        const u8 flags = oam[index + 3];
        const int sprite_height = use8x16 ? 16 : 8;
        if (static_cast<int>(ly) >= y_pos && static_cast<int>(ly) < (y_pos + sprite_height)) {
            int line = static_cast<int>(ly) - y_pos;
            if (flags & 0x40) line = (sprite_height - 1) - line;
            u8 effective_tile = tile_num;
            int effective_line = line;
            if (use8x16) {
                effective_tile &= 0xFE;
                if (effective_line >= 8) { effective_tile += 1; effective_line -= 8; }
            }
            u16 tile_location = 0x8000 + (effective_tile * 16) + (effective_line * 2);
            u8 d1 = vram[tile_location - 0x8000], d2 = vram[tile_location + 1 - 0x8000];
            for (int tile_pixel = 7; tile_pixel >= 0; tile_pixel--) {
                int bit = (flags & 0x20) ? 7 - tile_pixel : tile_pixel;
                u8 color_id = (BIT(d2, bit) << 1) | BIT(d1, bit);
                u16 palette = (flags & 0x10) ? 0xFF49 : 0xFF48;
                const int screen_x = x_pos + (7 - tile_pixel);
                if (color_id != 0 && screen_x >= 0 && screen_x < 160) {
                    framebuffer[ly * 160 + screen_x] = get_color(color_id, palette);
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

// ----------------------------------------------------
// Debug / UI Features
// ----------------------------------------------------

void PPU::init_window(bool debug, const std::string& rom_title) {
    debugMode = debug;
    if (debugMode) {
        window.create(sf::VideoMode({window_width, window_height}), "GB Emulator - Debug View");
        screenSprite.setPosition(sf::Vector2f(32.0f, 32.0f));
        screenSprite.setScale(sf::Vector2f(3.0f, 3.0f));
    } else {
        window.create(sf::VideoMode({screen_width * 4, screen_height * 4}), "GB Emulator");
        screenSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
        screenSprite.setScale(sf::Vector2f(4.0f, 4.0f));
    }
    window.setFramerateLimit(60);

    (void)screenTexture.resize({screen_width, screen_height});
    screenSprite.setTexture(screenTexture);

    fontLoaded = font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
    if (!fontLoaded && debugMode) {
        statusMessage = "Missing font: /usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
        statusTimer = status_display_seconds;
    }

    romInfo.mbc_name = mbcNameFromType(romInfo.type);

    std::ifstream bpFile("utility_scripts/breakpoints.txt");
    if (bpFile) {
        std::string line;
        while (std::getline(bpFile, line)) {
            std::stringstream ss(line);
            u32 value = 0;
            if (line.rfind("0x", 0) == 0 || line.rfind("0X", 0) == 0) {
                ss >> std::hex >> value;
            } else {
                ss >> std::hex >> value;
            }
            if (ss && value <= 0xFFFF) {
                breakpoints.push_back(static_cast<u16>(value));
            }
        }
    }
}

bool PPU::isOpen() const {
    return window.isOpen();
}

void PPU::handleEvents(JoypadState& joypad) {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
                case sf::Keyboard::Key::Right: joypad.right = true; break;
                case sf::Keyboard::Key::Left:  joypad.left = true; break;
                case sf::Keyboard::Key::Up:    joypad.up = true; break;
                case sf::Keyboard::Key::Down:  joypad.down = true; break;
                case sf::Keyboard::Key::Z:     joypad.a = true; break;
                case sf::Keyboard::Key::X:     joypad.b = true; break;
                case sf::Keyboard::Key::Enter: joypad.start = true; break;
                case sf::Keyboard::Key::RShift: joypad.select = true; break;
                case sf::Keyboard::Key::Escape: window.close(); break;
                
                // Debug actions
                case sf::Keyboard::Key::Space:
                    if (debugMode) {
                        paused = !paused;
                        statusMessage = paused ? "Paused" : "Running";
                        statusTimer = status_display_seconds;
                    }
                    break;
                case sf::Keyboard::Key::N:
                    if (debugMode && paused) {
                        stepRequested = true;
                        statusMessage = "Step";
                        statusTimer = status_display_seconds;
                    }
                    break;
                default:
                    if (debugMode && keyPressed->code >= sf::Keyboard::Key::Num0 && keyPressed->code <= sf::Keyboard::Key::Num9) {
                        int slot = static_cast<int>(keyPressed->code) - static_cast<int>(sf::Keyboard::Key::Num0);
                        bool ok = false;
                        if (keyPressed->shift || keyPressed->control) {
                            ok = saveStateSlot(slot);
                            statusMessage = ok ? "Saved state to slot " + std::to_string(slot)
                                               : "Failed to save slot " + std::to_string(slot);
                        } else {
                            ok = loadStateSlot(slot);
                            statusMessage = ok ? "Loaded state from slot " + std::to_string(slot)
                                               : "Failed to load slot " + std::to_string(slot);
                        }
                        statusTimer = status_display_seconds;
                    }
                    break;
            }
        }

        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            switch (keyReleased->code) {
                case sf::Keyboard::Key::Right: joypad.right = false; break;
                case sf::Keyboard::Key::Left:  joypad.left = false; break;
                case sf::Keyboard::Key::Up:    joypad.up = false; break;
                case sf::Keyboard::Key::Down:  joypad.down = false; break;
                case sf::Keyboard::Key::Z:     joypad.a = false; break;
                case sf::Keyboard::Key::X:     joypad.b = false; break;
                case sf::Keyboard::Key::Enter: joypad.start = false; break;
                case sf::Keyboard::Key::RShift: joypad.select = false; break;
                default: break;
            }
        }
    }
}

void PPU::update(const float dtSeconds, const u64 cyclesExecuted) {
    if (!debugMode) return;

    timeAccumulator += dtSeconds;
    if (statusTimer > 0.0f) {
        statusTimer -= dtSeconds;
        if (statusTimer <= 0.0f) {
            statusMessage.clear();
        }
    }

    if (timeAccumulator >= 0.5f) {
        timeAccumulator = 0.0f;
        lastCycles = cyclesExecuted;
        if (mmu) {
            lastReads = mmu->get_read_count();
            lastWrites = mmu->get_write_count();
        }
    }
}

void PPU::render() {
    if (debugMode) {
        window.clear(sf::Color(18, 20, 24));
        screenTexture.update(reinterpret_cast<const uint8_t*>(framebuffer.data()));
        window.draw(screenSprite);
        drawPanels();
        window.display();
    } else {
        window.clear();
        screenTexture.update(reinterpret_cast<const uint8_t*>(framebuffer.data()));
        window.draw(screenSprite);
        window.display();
    }
}

void PPU::recordOpcode(u16 pc, u8 opcode) {
    opcodeLog.push_front({pc, opcode});
    if (opcodeLog.size() > opcode_log_size) {
        opcodeLog.pop_back();
    }
}

void PPU::checkBreakpoint(const u16 pc) {
    for (const auto bp : breakpoints) {
        if (bp == pc) {
            paused = true;
            statusMessage = "Breakpoint hit: " + toHex(pc, 4);
            statusTimer = status_display_seconds;
            return;
        }
    }
}

bool PPU::saveStateSlot(const int slot) {
    if (!cpu || !mmu) return false;

    std::filesystem::create_directories("savestates");
    std::ostringstream path;
    path << "savestates/slot_" << slot << ".bin";

    std::ofstream out(path.str(), std::ios::binary);
    if (!out) {
        return false;
    }

    const u16 pc = cpu->get_pc();
    const u16 sp = cpu->get_sp();
    const u16 af = cpu->get_af();
    const u16 bc = cpu->get_bc();
    const u16 de = cpu->get_de();
    const u16 hl = cpu->get_hl();
    const bool ime = cpu->getIME();

    out.write(reinterpret_cast<const char*>(&pc), sizeof(pc));
    out.write(reinterpret_cast<const char*>(&sp), sizeof(sp));
    out.write(reinterpret_cast<const char*>(&af), sizeof(af));
    out.write(reinterpret_cast<const char*>(&bc), sizeof(bc));
    out.write(reinterpret_cast<const char*>(&de), sizeof(de));
    out.write(reinterpret_cast<const char*>(&hl), sizeof(hl));
    out.write(reinterpret_cast<const char*>(&ime), sizeof(ime));

    // Save PPU state
    out.write(reinterpret_cast<const char*>(vram.data()), vram.size());
    out.write(reinterpret_cast<const char*>(oam.data()), oam.size());
    out.write(reinterpret_cast<const char*>(&lcdc), sizeof(lcdc));
    out.write(reinterpret_cast<const char*>(&stat), sizeof(stat));
    out.write(reinterpret_cast<const char*>(&scy), sizeof(scy));
    out.write(reinterpret_cast<const char*>(&scx), sizeof(scx));
    out.write(reinterpret_cast<const char*>(&ly), sizeof(ly));
    out.write(reinterpret_cast<const char*>(&lyc), sizeof(lyc));
    out.write(reinterpret_cast<const char*>(&dma), sizeof(dma));
    out.write(reinterpret_cast<const char*>(&bgp), sizeof(bgp));
    out.write(reinterpret_cast<const char*>(&obp0), sizeof(obp0));
    out.write(reinterpret_cast<const char*>(&obp1), sizeof(obp1));
    out.write(reinterpret_cast<const char*>(&wy), sizeof(wy));
    out.write(reinterpret_cast<const char*>(&wx), sizeof(wx));
    out.write(reinterpret_cast<const char*>(&mode_clock), sizeof(mode_clock));
    out.write(reinterpret_cast<const char*>(&window_line_counter), sizeof(window_line_counter));
    out.write(reinterpret_cast<const char*>(&frame_ready), sizeof(frame_ready));

    auto dump = mmu->dump_memory();
    const u32 size = static_cast<u32>(dump.size());
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(reinterpret_cast<const char*>(dump.data()), static_cast<std::streamsize>(dump.size()));

    return true;
}

bool PPU::loadStateSlot(const int slot) {
    if (!cpu || !mmu) return false;

    std::ostringstream path;
    path << "savestates/slot_" << slot << ".bin";

    std::ifstream in(path.str(), std::ios::binary);
    if (!in) {
        return false;
    }

    u16 pc{};
    u16 sp{};
    u16 af{};
    u16 bc{};
    u16 de{};
    u16 hl{};
    bool ime{};

    in.read(reinterpret_cast<char*>(&pc), sizeof(pc));
    in.read(reinterpret_cast<char*>(&sp), sizeof(sp));
    in.read(reinterpret_cast<char*>(&af), sizeof(af));
    in.read(reinterpret_cast<char*>(&bc), sizeof(bc));
    in.read(reinterpret_cast<char*>(&de), sizeof(de));
    in.read(reinterpret_cast<char*>(&hl), sizeof(hl));
    in.read(reinterpret_cast<char*>(&ime), sizeof(ime));

    cpu->set_pc(pc);
    cpu->set_sp(sp);
    cpu->reg(ProcessingUnit::Register::A) = static_cast<u8>(af >> 8);
    cpu->reg(ProcessingUnit::Register::F) = static_cast<u8>(af & 0xFF);
    cpu->reg(ProcessingUnit::Register::B) = static_cast<u8>(bc >> 8);
    cpu->reg(ProcessingUnit::Register::C) = static_cast<u8>(bc & 0xFF);
    cpu->reg(ProcessingUnit::Register::D) = static_cast<u8>(de >> 8);
    cpu->reg(ProcessingUnit::Register::E) = static_cast<u8>(de & 0xFF);
    cpu->reg(ProcessingUnit::Register::H) = static_cast<u8>(hl >> 8);
    cpu->reg(ProcessingUnit::Register::L) = static_cast<u8>(hl & 0xFF);
    cpu->setIME(ime);

    // Restore PPU state
    in.read(reinterpret_cast<char*>(vram.data()), vram.size());
    in.read(reinterpret_cast<char*>(oam.data()), oam.size());
    in.read(reinterpret_cast<char*>(&lcdc), sizeof(lcdc));
    in.read(reinterpret_cast<char*>(&stat), sizeof(stat));
    in.read(reinterpret_cast<char*>(&scy), sizeof(scy));
    in.read(reinterpret_cast<char*>(&scx), sizeof(scx));
    in.read(reinterpret_cast<char*>(&ly), sizeof(ly));
    in.read(reinterpret_cast<char*>(&lyc), sizeof(lyc));
    in.read(reinterpret_cast<char*>(&dma), sizeof(dma));
    in.read(reinterpret_cast<char*>(&bgp), sizeof(bgp));
    in.read(reinterpret_cast<char*>(&obp0), sizeof(obp0));
    in.read(reinterpret_cast<char*>(&obp1), sizeof(obp1));
    in.read(reinterpret_cast<char*>(&wy), sizeof(wy));
    in.read(reinterpret_cast<char*>(&wx), sizeof(wx));
    in.read(reinterpret_cast<char*>(&mode_clock), sizeof(mode_clock));
    in.read(reinterpret_cast<char*>(&window_line_counter), sizeof(window_line_counter));
    in.read(reinterpret_cast<char*>(&frame_ready), sizeof(frame_ready));

    u32 size{};
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::vector<u8> dump(size);
    in.read(reinterpret_cast<char*>(dump.data()), size);
    mmu->load_memory(dump);

    return true;
}

void PPU::drawPanels() {
    const float panelStartX = 32.0f + screen_width * 3.0f + 32.0f;
    const float panelTop = 24.0f;
    const float panelWidth = window_width - panelStartX - 24.0f;

    sf::RectangleShape panelBg(sf::Vector2f(panelWidth, window_height - 48.0f));
    panelBg.setPosition(sf::Vector2f(panelStartX, panelTop));
    panelBg.setFillColor(sf::Color(26, 29, 35));
    panelBg.setOutlineColor(sf::Color(50, 55, 70));
    panelBg.setOutlineThickness(1.0f);
    window.draw(panelBg);

    if (!cpu) return;

    float cursorY = panelTop + 16.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "CPU Registers", 16, sf::Color(220, 220, 230));
    cursorY += 26.0f;

    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "AF: " + toHex(cpu->get_af(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "BC: " + toHex(cpu->get_bc(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "DE: " + toHex(cpu->get_de(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "HL: " + toHex(cpu->get_hl(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "SP: " + toHex(cpu->get_sp(), 4) + "  PC: " + toHex(cpu->get_pc(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 22.0f;

    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Flags: Z=" + std::to_string(cpu->get_flag_z()) +
                                          " N=" + std::to_string(cpu->get_flag_n()) +
                                          " H=" + std::to_string(cpu->get_flag_h()) +
                                          " C=" + std::to_string(cpu->get_flag_c()), 14, sf::Color(190, 190, 200));
    cursorY += 24.0f;

    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "ROM", 16, sf::Color(220, 220, 230));
    cursorY += 24.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Title: " + romInfo.title, 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Type: 0x" + toHex(romInfo.type, 2) + " (" + romInfo.mbc_name + ")", 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "ROM Size: 0x" + toHex(romInfo.rom_size, 2) + "  RAM Size: 0x" + toHex(romInfo.ram_size, 2), 14, sf::Color(180, 200, 240));
    cursorY += 24.0f;

    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Memory/Bus", 16, sf::Color(220, 220, 230));
    cursorY += 24.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Reads: " + std::to_string(lastReads) + "  Writes: " + std::to_string(lastWrites), 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Cycles: " + std::to_string(lastCycles), 14, sf::Color(180, 200, 240));
    cursorY += 24.0f;

    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Opcode Trace", 16, sf::Color(220, 220, 230));
    cursorY += 22.0f;
    int shown = 0;
    for (const auto& trace : opcodeLog) {
        if (shown++ >= 10) break;
        drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), toHex(trace.pc, 4) + ": " + toHex(trace.opcode, 2), 13, sf::Color(180, 200, 240));
        cursorY += 16.0f;
    }

    cursorY += 10.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "ROM Bytes", 16, sf::Color(220, 220, 230));
    cursorY += 22.0f;
    if (!romInfo.rom_bytes.empty()) {
        const size_t maxRows = std::min(rom_rows_visible, romInfo.rom_bytes.size() / rom_bytes_per_row + 1);
        for (size_t row = 0; row < maxRows; ++row) {
            std::ostringstream line;
            const size_t offset = row * rom_bytes_per_row;
            line << toHex(static_cast<u32>(offset), 4) << ": ";
            for (size_t i = 0; i < rom_bytes_per_row && offset + i < romInfo.rom_bytes.size(); ++i) {
                line << toHex(romInfo.rom_bytes[offset + i], 2) << " ";
            }
            drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), line.str(), 12, sf::Color(170, 190, 210));
            cursorY += 14.0f;
        }
    }

    cursorY = window_height - 96.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Save State: Shift+0-9", 13, sf::Color(200, 180, 120));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Load State: 0-9", 13, sf::Color(200, 180, 120));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Pause: Space  Step: N", 13, sf::Color(200, 180, 120));
    cursorY += 18.0f;
    if (!statusMessage.empty()) {
        drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), statusMessage, 13, sf::Color(220, 140, 140));
    }
}

void PPU::drawText(const sf::Vector2f& pos, const std::string& text, const unsigned size, const sf::Color& color) {
    if (!fontLoaded) {
        return;
    }
    sf::Text txt(font, text, size);
    txt.setFillColor(color);
    txt.setPosition(pos);
    window.draw(txt);
}

std::string PPU::toHex(const u32 value, const int width) const {
    std::ostringstream ss;
    ss << std::hex << std::uppercase << std::setw(width) << std::setfill('0') << value;
    return ss.str();
}
