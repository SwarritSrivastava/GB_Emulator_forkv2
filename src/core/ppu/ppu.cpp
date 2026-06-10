#include "../../../include/ppu.hpp"
#include "../../../include/cartridge.hpp"
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <fstream>

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

PPU::PPU(ProcessingUnit& cpuRef, MMU& mmuRef, RomInfo info)
    : cpu(cpuRef)
    , mmu(mmuRef)
    , romInfo(std::move(info))
    , window(sf::VideoMode(sf::Vector2u(static_cast<unsigned>(window_width), static_cast<unsigned>(window_height))), "GB Emulator - Debug View")
    , screenTexture()
    , screenSprite(screenTexture)
{
    window.setFramerateLimit(60);

    screenPixels.resize(screen_width * screen_height * 4);
    screenTexture.resize(sf::Vector2u(static_cast<unsigned>(screen_width), static_cast<unsigned>(screen_height)));
    screenSprite.setPosition(sf::Vector2f(32.0f, 32.0f));
    screenSprite.setScale(sf::Vector2f(3.0f, 3.0f));

    fontLoaded = font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
    if (!fontLoaded) {
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
    updateFrameBufferPlaceholder();
}

bool PPU::isOpen() const {
    return window.isOpen();
}

void PPU::handleEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code >= sf::Keyboard::Key::Num0 && key->code <= sf::Keyboard::Key::Num9) {
                int slot = static_cast<int>(key->code) - static_cast<int>(sf::Keyboard::Key::Num0);
                bool ok = false;
                if (key->shift || key->control) {
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
            if (key->code == sf::Keyboard::Key::Space) {
                paused = !paused;
                statusMessage = paused ? "Paused" : "Running";
                statusTimer = status_display_seconds;
            }
            if (key->code == sf::Keyboard::Key::N) {
                if (paused) {
                    statusMessage = "Step";
                    statusTimer = status_display_seconds;
                }
            }
        }
    }
}

void PPU::update(const float dtSeconds, const u64 cyclesExecuted) {
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
        lastReads = mmu.get_read_count();
        lastWrites = mmu.get_write_count();
    }

    updateFrameBufferPlaceholder();
}

void PPU::render() {
    window.clear(sf::Color(18, 20, 24));
    window.draw(screenSprite);
    drawPanels();
    window.display();
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
    std::filesystem::create_directories("savestates");
    std::ostringstream path;
    path << "savestates/slot_" << slot << ".bin";

    std::ofstream out(path.str(), std::ios::binary);
    if (!out) {
        return false;
    }

    const u16 pc = cpu.get_pc();
    const u16 sp = cpu.get_sp();
    const u16 af = cpu.get_af();
    const u16 bc = cpu.get_bc();
    const u16 de = cpu.get_de();
    const u16 hl = cpu.get_hl();
    const bool ime = cpu.getIME();

    out.write(reinterpret_cast<const char*>(&pc), sizeof(pc));
    out.write(reinterpret_cast<const char*>(&sp), sizeof(sp));
    out.write(reinterpret_cast<const char*>(&af), sizeof(af));
    out.write(reinterpret_cast<const char*>(&bc), sizeof(bc));
    out.write(reinterpret_cast<const char*>(&de), sizeof(de));
    out.write(reinterpret_cast<const char*>(&hl), sizeof(hl));
    out.write(reinterpret_cast<const char*>(&ime), sizeof(ime));

    auto dump = mmu.dump_memory();
    const u32 size = static_cast<u32>(dump.size());
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(reinterpret_cast<const char*>(dump.data()), static_cast<std::streamsize>(dump.size()));

    return true;
}

bool PPU::loadStateSlot(const int slot) {
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

    cpu.set_pc(pc);
    cpu.set_sp(sp);
    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(af >> 8);
    cpu.reg(ProcessingUnit::Register::F) = static_cast<u8>(af & 0xFF);
    cpu.reg(ProcessingUnit::Register::B) = static_cast<u8>(bc >> 8);
    cpu.reg(ProcessingUnit::Register::C) = static_cast<u8>(bc & 0xFF);
    cpu.reg(ProcessingUnit::Register::D) = static_cast<u8>(de >> 8);
    cpu.reg(ProcessingUnit::Register::E) = static_cast<u8>(de & 0xFF);
    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>(hl >> 8);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(hl & 0xFF);
    cpu.setIME(ime);

    u32 size{};
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::vector<u8> dump(size);
    in.read(reinterpret_cast<char*>(dump.data()), size);
    mmu.load_memory(dump);

    return true;
}

void PPU::updateFrameBufferPlaceholder() {
    frameCounter++;
    for (u32 y = 0; y < screen_height; ++y) {
        for (u32 x = 0; x < screen_width; ++x) {
            const size_t idx = (y * screen_width + x) * 4;
            const u8 shade = static_cast<u8>((x + y + frameCounter) % 255);
            screenPixels[idx] = shade;
            screenPixels[idx + 1] = static_cast<u8>((shade + 40) % 255);
            screenPixels[idx + 2] = static_cast<u8>((shade + 80) % 255);
            screenPixels[idx + 3] = 255;
        }
    }
    screenTexture.update(screenPixels.data());
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

    float cursorY = panelTop + 16.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "CPU Registers", 16, sf::Color(220, 220, 230));
    cursorY += 26.0f;

    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "AF: " + toHex(cpu.get_af(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "BC: " + toHex(cpu.get_bc(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "DE: " + toHex(cpu.get_de(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "HL: " + toHex(cpu.get_hl(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 18.0f;
    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "SP: " + toHex(cpu.get_sp(), 4) + "  PC: " + toHex(cpu.get_pc(), 4), 14, sf::Color(180, 200, 240));
    cursorY += 22.0f;

    drawText(sf::Vector2f(panelStartX + 16.0f, cursorY), "Flags: Z=" + std::to_string(cpu.get_flag_z()) +
                                          " N=" + std::to_string(cpu.get_flag_n()) +
                                          " H=" + std::to_string(cpu.get_flag_h()) +
                                          " C=" + std::to_string(cpu.get_flag_c()), 14, sf::Color(190, 190, 200));
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
