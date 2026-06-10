#pragma once

#include "ProcessingUnit.hpp"
#include "mmu.hpp"
#include "common.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <deque>
#include <string>
#include <vector>

class MMU;

class PPU {
public:
    struct RomInfo {
        std::string title;
        u8 type{};
        u8 rom_size{};
        u8 ram_size{};
        std::string mbc_name;
        std::vector<u8> rom_bytes;
    };

    struct OpcodeTrace {
        u16 pc{};
        u8 opcode{};
    };

    PPU();

    void set_mmu(MMU* m) { mmu = m; }
    void set_cpu(ProcessingUnit* c) { cpu = c; }
    void set_rom_info(const RomInfo& info) { romInfo = info; }
    void step(int cycles);
    u8 read(u16 address) const;
    void write(u16 address, u8 value);

    const u32* get_framebuffer() const { return framebuffer.data(); }
    bool is_frame_ready() const { return frame_ready; }
    void clear_frame_ready() { frame_ready = false; }

    // Debug / Feature functions
    void init_window(bool debug, const std::string& rom_title);
    bool isOpen() const;
    bool isPaused() const { return paused; }
    void handleEvents(JoypadState& joypad);
    void update(float dtSeconds, u64 cyclesExecuted);
    void render();

    void recordOpcode(u16 pc, u8 opcode);
    void checkBreakpoint(u16 pc);

    bool isStepRequested() const { return stepRequested; }
    void clearStepRequest() { stepRequested = false; }

private:
    bool saveStateSlot(int slot);
    bool loadStateSlot(int slot);

    void drawPanels();
    void drawText(const sf::Vector2f& pos, const std::string& text, unsigned size, const sf::Color& color);
    std::string toHex(u32 value, int width) const;

    // Emulation components
    ProcessingUnit* cpu = nullptr;
    MMU* mmu = nullptr;
    RomInfo romInfo;

    // Rendering / SFML Window components
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded{false};
    bool debugMode{false};

    sf::Texture screenTexture;
    sf::Sprite screenSprite;

    // Emulation state
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

    // Debugging state
    std::deque<OpcodeTrace> opcodeLog;
    std::vector<u16> breakpoints;
    bool paused{false};
    bool stepRequested{false};

    float timeAccumulator{0.0f};
    u64 lastCycles{0};
    u64 lastReads{0};
    u64 lastWrites{0};

    std::string statusMessage;
    float statusTimer{0.0f};
    u32 frameCounter{0};
};
