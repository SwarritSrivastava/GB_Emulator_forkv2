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
constexpr u32 ui_width = 600;
constexpr u32 ui_height = 420;
constexpr u32 window_width = 1600;
constexpr u32 window_height = 900;
constexpr size_t opcode_log_size = 28;
constexpr float status_display_seconds = 2.5f;
constexpr size_t rom_bytes_per_row = 8;
constexpr size_t rom_rows_visible = 8;

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
    : dummy_ic(std::make_unique<InterruptController>())
    , ic(*dummy_ic)
    , screenSprite(screenTexture)
{
    framebuffer.fill(0xFFFFFFFF);
    lcdc = 0x91;
    stat = 0x85;
    bgp = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;
}

PPU::PPU(InterruptController& interrupt_controller)
    : ic(interrupt_controller)
    , screenSprite(screenTexture)
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
    if (mode == Mode::VBlank) {
        ic.request_interrupt(InterruptType::VBlank);
    }
    bool interrupt = false;
    if (mode == Mode::HBlank && (stat & 0x08)) interrupt = true;
    if (mode == Mode::VBlank && (stat & 0x10)) interrupt = true;
    if (mode == Mode::OAMSearch && (stat & 0x20)) interrupt = true;
    if (interrupt) {
        ic.request_interrupt(InterruptType::LCDStat);
    }
}

void PPU::update_stat() {
    if (ly == lyc) {
        stat |= 0x04;
        if (stat & 0x40) {
            ic.request_interrupt(InterruptType::LCDStat);
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

void PPU::init_window(bool debug, std::string_view rom_title, bool fullscreen) {
    debugMode = debug;
    sf::State state = fullscreen ? sf::State::Fullscreen : sf::State::Windowed;

    if (debugMode) {
        sf::VideoMode mode = fullscreen ? sf::VideoMode({1920, 1080}) : sf::VideoMode({window_width, window_height});
        window.create(mode, "GB Emulator - Debug View", state);
        
        sf::View view(sf::FloatRect({0.0f, 0.0f}, {static_cast<float>(window_width), static_cast<float>(window_height)}));
        window.setView(view);

        screenSprite.setPosition(sf::Vector2f(32.0f, 32.0f));
        screenSprite.setScale(sf::Vector2f(5.0f, 5.0f));
    } else {
        sf::VideoMode mode = fullscreen ? sf::VideoMode({1920, 1080}) : sf::VideoMode({screen_width * 4, screen_height * 4});
        window.create(mode, "GB Emulator", state);
        
        sf::View view(sf::FloatRect({0.0f, 0.0f}, {static_cast<float>(screen_width), static_cast<float>(screen_height)}));
        window.setView(view);

        screenSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
        screenSprite.setScale(sf::Vector2f(1.0f, 1.0f));
    }
    window.setFramerateLimit(60);

    (void)screenTexture.resize({screen_width, screen_height});
    screenSprite.setTexture(screenTexture);
    screenSprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(screen_width), static_cast<int>(screen_height)}));

    const std::vector<std::string> fontPaths = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/liberation/LiberationMono-Regular.ttf",
        "/usr/share/fonts/liberation-fonts/LiberationMono-Regular.ttf",
        "/usr/share/fonts/Adwaita/AdwaitaMono-Regular.ttf",
        "DejaVuSansMono.ttf",
        "LiberationMono-Regular.ttf"
    };

    for (const auto& path : fontPaths) {
        fontLoaded = font.openFromFile(path);
        if (fontLoaded) break;
    }

    if (!fontLoaded && debugMode) {
        statusMessage = "Missing monospace font. Debug text won't render.";
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
                case sf::Keyboard::Key::Right: physicalJoypad.right = true; break;
                case sf::Keyboard::Key::Left:  physicalJoypad.left = true; break;
                case sf::Keyboard::Key::Up:    physicalJoypad.up = true; break;
                case sf::Keyboard::Key::Down:  physicalJoypad.down = true; break;
                case sf::Keyboard::Key::Z:     physicalJoypad.a = true; break;
                case sf::Keyboard::Key::X:     physicalJoypad.b = true; break;
                case sf::Keyboard::Key::Enter: physicalJoypad.start = true; break;
                case sf::Keyboard::Key::RShift: physicalJoypad.select = true; break;
                case sf::Keyboard::Key::Escape: window.close(); break;
                
                // Debug actions
                case sf::Keyboard::Key::Space:
                    paused = !paused;
                    statusMessage = paused ? "Paused" : "Running";
                    statusTimer = status_display_seconds;
                    break;
                case sf::Keyboard::Key::N:
                    if (paused) {
                        stepRequested = true;
                        statusMessage = "Step";
                        statusTimer = status_display_seconds;
                    }
                    break;
                case sf::Keyboard::Key::T:
                    turbo = !turbo;
                    statusMessage = turbo ? "Turbo Mode 2x" : "Normal Mode 1x";
                    statusTimer = status_display_seconds;
                    break;
                case sf::Keyboard::Key::R:
                    resetRequested = true;
                    statusMessage = "Reset requested";
                    statusTimer = status_display_seconds;
                    break;
                case sf::Keyboard::Key::E: {
                    activeSlot = -1; // -1 represents 'e'
                    bool ok = false;
                    if (keyPressed->shift || keyPressed->control) {
                        std::string filepath = "savestates/external.bin";
                        FILE* f = popen("zenity --file-selection --save --confirm-overwrite --file-filter='Save States (*.bin) | *.bin' 2>/dev/null", "r");
                        if (f) {
                            char buf[1024];
                            if (fgets(buf, sizeof(buf), f)) {
                                std::string s(buf);
                                if (!s.empty() && s.back() == '\n') s.pop_back();
                                if (!s.empty()) filepath = s;
                            }
                            pclose(f);
                        }
                        externalStatePath = filepath;
                        ok = saveStatePath(filepath);
                        std::string filename = std::filesystem::path(filepath).filename().string();
                        statusMessage = ok ? "Saved: " + filename : "Save failed";
                    } else {
                        std::string filepath = "savestates/external.bin";
                        FILE* f = popen("zenity --file-selection --file-filter='Save States (*.bin) | *.bin' 2>/dev/null", "r");
                        if (f) {
                            char buf[1024];
                            if (fgets(buf, sizeof(buf), f)) {
                                std::string s(buf);
                                if (!s.empty() && s.back() == '\n') s.pop_back();
                                if (!s.empty()) filepath = s;
                            }
                            pclose(f);
                        }
                        externalStatePath = filepath;
                        ok = loadStatePath(filepath);
                        std::string filename = std::filesystem::path(filepath).filename().string();
                        statusMessage = ok ? "Loaded: " + filename : "Load failed";
                    }
                    statusTimer = status_display_seconds;
                    break;
                }
                default:
                    if (keyPressed->code >= sf::Keyboard::Key::Num0 && keyPressed->code <= sf::Keyboard::Key::Num9) {
                        int slot = static_cast<int>(keyPressed->code) - static_cast<int>(sf::Keyboard::Key::Num0);
                        activeSlot = slot;
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
                case sf::Keyboard::Key::Right: physicalJoypad.right = false; break;
                case sf::Keyboard::Key::Left:  physicalJoypad.left = false; break;
                case sf::Keyboard::Key::Up:    physicalJoypad.up = false; break;
                case sf::Keyboard::Key::Down:  physicalJoypad.down = false; break;
                case sf::Keyboard::Key::Z:     physicalJoypad.a = false; break;
                case sf::Keyboard::Key::X:     physicalJoypad.b = false; break;
                case sf::Keyboard::Key::Enter: physicalJoypad.start = false; break;
                case sf::Keyboard::Key::RShift: physicalJoypad.select = false; break;
                default: break;
            }
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mPos = window.mapPixelToCoords(mousePressed->position);
                const float rp2X = 1238.0f; 
                const float rp2W = window_width - rp2X - 32.0f;
                const float gpX = rp2X + (rp2W - 220.0f) / 2.0f;
                const float gpY = 32.0f + 60.0f;

                auto rectContains = [](float rx, float ry, float rw, float rh, float px, float py) {
                    return px >= rx && px <= rx + rw && py >= ry && py <= ry + rh;
                };

                if (rectContains(gpX + 40.0f, gpY + 0.0f, 30.0f, 30.0f, mPos.x, mPos.y)) { clickableJoypad.up = !clickableJoypad.up; }
                if (rectContains(gpX + 40.0f, gpY + 80.0f, 30.0f, 30.0f, mPos.x, mPos.y)) { clickableJoypad.down = !clickableJoypad.down; }
                if (rectContains(gpX + 0.0f, gpY + 40.0f, 30.0f, 30.0f, mPos.x, mPos.y)) { clickableJoypad.left = !clickableJoypad.left; }
                if (rectContains(gpX + 80.0f, gpY + 40.0f, 30.0f, 30.0f, mPos.x, mPos.y)) { clickableJoypad.right = !clickableJoypad.right; }
                if (rectContains(gpX + 185.0f, gpY + 20.0f, 34.0f, 34.0f, mPos.x, mPos.y)) { clickableJoypad.a = !clickableJoypad.a; }
                if (rectContains(gpX + 140.0f, gpY + 50.0f, 34.0f, 34.0f, mPos.x, mPos.y)) { clickableJoypad.b = !clickableJoypad.b; }
                if (rectContains(gpX + 30.0f, gpY + 140.0f, 70.0f, 24.0f, mPos.x, mPos.y)) { clickableJoypad.select = !clickableJoypad.select; }
                if (rectContains(gpX + 120.0f, gpY + 140.0f, 70.0f, 24.0f, mPos.x, mPos.y)) { clickableJoypad.start = !clickableJoypad.start; }
            }
        }
    }

    joypad.up = physicalJoypad.up || clickableJoypad.up;
    joypad.down = physicalJoypad.down || clickableJoypad.down;
    joypad.left = physicalJoypad.left || clickableJoypad.left;
    joypad.right = physicalJoypad.right || clickableJoypad.right;
    joypad.a = physicalJoypad.a || clickableJoypad.a;
    joypad.b = physicalJoypad.b || clickableJoypad.b;
    joypad.select = physicalJoypad.select || clickableJoypad.select;
    joypad.start = physicalJoypad.start || clickableJoypad.start;
    currentJoypadState = joypad;
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
        window.clear(sf::Color(0, 0, 0));
        screenTexture.update(reinterpret_cast<const uint8_t*>(framebuffer.data()));
        window.draw(screenSprite);
        drawPanels();
        window.display();
    } else {
        window.clear();
        screenTexture.update(reinterpret_cast<const uint8_t*>(framebuffer.data()));
        window.draw(screenSprite);
        if (!statusMessage.empty()) {
            window.setView(window.getDefaultView());
            drawText(sf::Vector2f(20.0f, 20.0f), statusMessage, 28, sf::Color(255, 50, 50));
            sf::View view(sf::FloatRect({0.0f, 0.0f}, {static_cast<float>(screen_width), static_cast<float>(screen_height)}));
            window.setView(view);
        }
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

void PPU::reset() {
    vram.fill(0);
    oam.fill(0);
    lcdc = 0x91;
    stat = 0x85;
    bgp = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;
    scy = 0x00;
    scx = 0x00;
    ly = 0x00;
    lyc = 0x00;
    wy = 0x00;
    wx = 0x00;
    mode_clock = 0;
    window_line_counter = 0;
    frame_ready = false;
    framebuffer.fill(0xFFFFFFFF);
}

bool PPU::saveStatePath(std::string_view filepath) {
    if (!cpu || !mmu) return false;

    try {
        std::filesystem::create_directories(std::filesystem::path(filepath).parent_path());
    } catch (...) {}

    std::ofstream out(std::string(filepath), std::ios::binary);
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

bool PPU::saveStateSlot(const int slot) {
    std::ostringstream path;
    path << "savestates/slot_" << slot << ".bin";
    return saveStatePath(path.str());
}

bool PPU::loadStatePath(std::string_view filepath) {
    if (!cpu || !mmu) return false;

    std::ifstream in(std::string(filepath), std::ios::binary);
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

bool PPU::loadStateSlot(const int slot) {
    std::ostringstream path;
path << "savestates/slot_" << slot << ".bin";
    return loadStatePath(path.str());
}

void PPU::drawPanels() {
    if (!cpu) return;

    const sf::Color panelBgColor(12, 12, 15);
    const sf::Color panelBorderColor(40, 40, 50);
    const sf::Color titleColor(0, 255, 128); // Neon Green
    const sf::Color textColor(200, 200, 200);
    const sf::Color highlightColor(0, 200, 255); // Neon Blue
    const sf::Color errorColor(255, 50, 50); // Neon Red

    // Right Panel 1 (CPU, ROM, Memory, Trace)
    const float rp1X = 32.0f + 800.0f + 32.0f; // 864.0f
    const float rp1Y = 32.0f;
    const float rp1W = 350.0f;
    const float rp1H = window_height - 64.0f; // 836.0f

    sf::RectangleShape rp1Bg(sf::Vector2f(rp1W, rp1H));
    rp1Bg.setPosition(sf::Vector2f(rp1X, rp1Y));
    rp1Bg.setFillColor(panelBgColor);
    rp1Bg.setOutlineColor(panelBorderColor);
    rp1Bg.setOutlineThickness(1.0f);
    window.draw(rp1Bg);

    float cursorY = rp1Y + 16.0f;
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "CPU Registers", 18, titleColor);
    cursorY += 28.0f;

    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "AF: " + toHex(cpu->get_af(), 4), 14, highlightColor);
    cursorY += 20.0f;
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "BC: " + toHex(cpu->get_bc(), 4), 14, highlightColor);
    cursorY += 20.0f;
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "DE: " + toHex(cpu->get_de(), 4), 14, highlightColor);
    cursorY += 20.0f;
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "HL: " + toHex(cpu->get_hl(), 4), 14, highlightColor);
    cursorY += 20.0f;
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "SP: " + toHex(cpu->get_sp(), 4) + "  PC: " + toHex(cpu->get_pc(), 4), 14, highlightColor);
    cursorY += 20.0f;
    
    std::string flags = "Z=" + std::to_string(cpu->get_flag_z()) +
                        " N=" + std::to_string(cpu->get_flag_n()) +
                        " H=" + std::to_string(cpu->get_flag_h()) +
                        " C=" + std::to_string(cpu->get_flag_c());
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "Flags: " + flags, 14, textColor);
    cursorY += 28.0f;

    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "ROM Info", 18, titleColor);
    cursorY += 28.0f;
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "Title: " + romInfo.title, 14, textColor);
    cursorY += 20.0f;
    std::string shortMbc = mbcNameFromType(romInfo.type);
    if (shortMbc.length() > 15) shortMbc = shortMbc.substr(0, 12) + "...";
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "Type: 0x" + toHex(romInfo.type, 2) + " (" + shortMbc + ")", 14, textColor);
    cursorY += 20.0f;
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "ROM: 0x" + toHex(romInfo.rom_size, 2) + "  RAM: 0x" + toHex(romInfo.ram_size, 2), 14, textColor);
    cursorY += 28.0f;

    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "Memory/Bus", 18, titleColor);
    cursorY += 28.0f;
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "Rds: " + std::to_string(lastReads) + "  Wrs: " + std::to_string(lastWrites), 14, textColor);
    cursorY += 20.0f;
    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "Cycles: " + std::to_string(lastCycles), 14, textColor);
    cursorY += 28.0f;

    drawText(sf::Vector2f(rp1X + 16.0f, cursorY), "Opcode Trace", 18, titleColor);
    cursorY += 26.0f;
    int shown = 0;
    for (const auto& trace : opcodeLog) {
        if (shown++ >= 14) break; 
        drawText(sf::Vector2f(rp1X + 16.0f, cursorY), toHex(trace.pc, 4) + ": " + toHex(trace.opcode, 2), 14, highlightColor);
        cursorY += 18.0f;
    }

    if (!statusMessage.empty()) {
        drawText(sf::Vector2f(rp1X + 16.0f, rp1Y + rp1H - 24.0f), statusMessage, 14, errorColor);
    }

    // Right Panel 2 (Gamepad, ROM Bytes)
    const float rp2X = rp1X + rp1W + 24.0f; // 1238.0f
    const float rp2Y = 32.0f;
    const float rp2W = window_width - rp2X - 32.0f; // 330.0f
    const float rp2H = window_height - 64.0f; // 836.0f

    sf::RectangleShape rp2Bg(sf::Vector2f(rp2W, rp2H));
    rp2Bg.setPosition(sf::Vector2f(rp2X, rp2Y));
    rp2Bg.setFillColor(panelBgColor);
    rp2Bg.setOutlineColor(panelBorderColor);
    rp2Bg.setOutlineThickness(1.0f);
    window.draw(rp2Bg);

    drawText(sf::Vector2f(rp2X + 16.0f, rp2Y + 16.0f), "Virtual Gamepad", 18, titleColor);
    
    const float gpX = rp2X + (rp2W - 220.0f) / 2.0f;
    const float gpY = rp2Y + 60.0f;

    auto drawButton = [&](float x, float y, float w, float h, const std::string& label, bool isActive) {
        sf::RectangleShape btn(sf::Vector2f(w, h));
        btn.setPosition(sf::Vector2f(x, y));
        if (isActive) {
            btn.setFillColor(titleColor); 
            btn.setOutlineColor(sf::Color(255, 255, 255));
        } else {
            btn.setFillColor(sf::Color(20, 20, 25)); 
            btn.setOutlineColor(highlightColor);
        }
        btn.setOutlineThickness(1.0f);
        window.draw(btn);

        float textX = x + (w - label.length() * 7.5f) / 2.0f;
        float textY = y + (h - 14.0f) / 2.0f - 1.0f;
        drawText(sf::Vector2f(textX, textY), label, 12, isActive ? sf::Color(0, 0, 0) : textColor);
    };

    // D-Pad
    drawButton(gpX + 40.0f, gpY + 0.0f, 30.0f, 30.0f, "U", currentJoypadState.up);
    drawButton(gpX + 0.0f, gpY + 40.0f, 30.0f, 30.0f, "L", currentJoypadState.left);
    drawButton(gpX + 80.0f, gpY + 40.0f, 30.0f, 30.0f, "R", currentJoypadState.right);
    drawButton(gpX + 40.0f, gpY + 80.0f, 30.0f, 30.0f, "D", currentJoypadState.down);

    // Action
    drawButton(gpX + 140.0f, gpY + 50.0f, 34.0f, 34.0f, "B", currentJoypadState.b);
    drawButton(gpX + 185.0f, gpY + 20.0f, 34.0f, 34.0f, "A", currentJoypadState.a);

    // Select/Start
    drawButton(gpX + 30.0f, gpY + 140.0f, 70.0f, 24.0f, "SELECT", currentJoypadState.select);
    drawButton(gpX + 120.0f, gpY + 140.0f, 70.0f, 24.0f, "START", currentJoypadState.start);

    // ROM Bytes
    float romY = gpY + 200.0f;
    drawText(sf::Vector2f(rp2X + 16.0f, romY), "ROM Bytes", 18, titleColor);
    romY += 28.0f;
    if (!romInfo.rom_bytes.empty()) {
        const size_t maxRows = std::min(rom_rows_visible * 3, romInfo.rom_bytes.size() / rom_bytes_per_row + 1);
        for (size_t row = 0; row < maxRows; ++row) {
            std::ostringstream line;
            const size_t offset = row * rom_bytes_per_row;
            line << toHex(static_cast<u32>(offset), 4) << ": ";
            for (size_t i = 0; i < rom_bytes_per_row && offset + i < romInfo.rom_bytes.size(); ++i) {
                line << toHex(romInfo.rom_bytes[offset + i], 2) << " ";
            }
            drawText(sf::Vector2f(rp2X + 16.0f, romY), line.str(), 14, highlightColor);
            romY += 18.0f;
        }
    }

    // Emulator Control Panel (Below Game Screen)
    const float cpX = 32.0f;
    const float cpY = 32.0f + 720.0f + 24.0f; // 776.0f
    const float cpW = 800.0f;
    const float cpH = window_height - cpY - 32.0f; // 92.0f

    sf::RectangleShape cpBg(sf::Vector2f(cpW, cpH));
    cpBg.setPosition(sf::Vector2f(cpX, cpY));
    cpBg.setFillColor(panelBgColor);
    cpBg.setOutlineColor(panelBorderColor);
    cpBg.setOutlineThickness(1.0f);
    window.draw(cpBg);

    float col1X = cpX + 16.0f;
    drawText(sf::Vector2f(col1X, cpY + 12.0f), "Emulator Status", 16, titleColor);
    std::string speedStr = turbo ? "2x (Turbo)" : "1x (Normal)";
    drawText(sf::Vector2f(col1X, cpY + 36.0f), "Speed: " + speedStr, 14, textColor);
    std::string slotStr = (activeSlot == -1) ? "E (External)" : std::to_string(activeSlot);
    drawText(sf::Vector2f(col1X, cpY + 56.0f), "Slot:  [" + slotStr + "]", 14, textColor);
    
    if (activeSlot == -1 && !externalStatePath.empty()) {
        std::string filename = std::filesystem::path(externalStatePath).filename().string();
        if (filename.length() > 20) filename = filename.substr(0, 17) + "...";
        drawText(sf::Vector2f(col1X + 120.0f, cpY + 56.0f), "File: " + filename, 12, sf::Color(140, 160, 180));
    }

    float col2X = cpX + 260.0f;
    drawText(sf::Vector2f(col2X, cpY + 12.0f), "State Hotkeys", 16, titleColor);
    drawText(sf::Vector2f(col2X, cpY + 36.0f), "0-9 / Shift+0-9: Load/Save Slot", 13, highlightColor);
    drawText(sf::Vector2f(col2X, cpY + 56.0f), "E / Shift+E: Load/Save External", 13, highlightColor);

    float col3X = cpX + 540.0f;
    drawText(sf::Vector2f(col3X, cpY + 12.0f), "System Hotkeys", 16, titleColor);
    drawText(sf::Vector2f(col3X, cpY + 36.0f), "R: Reset Emulator", 13, highlightColor);
    drawText(sf::Vector2f(col3X, cpY + 56.0f), "Space: Pause | N: Step", 13, highlightColor);
}

void PPU::drawText(const sf::Vector2f& pos, std::string_view text, const unsigned size, const sf::Color& color) {
    if (!fontLoaded) {
        return;
    }
    sf::Text txt(font, std::string(text), size);
    txt.setFillColor(color);
    txt.setPosition(pos);
    window.draw(txt);
}

std::string PPU::toHex(const u32 value, const int width) const {
    std::ostringstream ss;
    ss << std::hex << std::uppercase << std::setw(width) << std::setfill('0') << value;
    return ss.str();
}
