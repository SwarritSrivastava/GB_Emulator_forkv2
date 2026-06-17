#pragma once

#include "ProcessingUnit.hpp"
#include "mmu.hpp"
#include "common.hpp"
#include "interrupt_controller.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <deque>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
class APU;
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
    explicit PPU(InterruptController& ic);

    void set_mmu(MMU* m) { mmu = m; }
    void set_cpu(ProcessingUnit* c) { cpu = c; }
    void set_apu(APU* a) { apu = a; }
    void set_rom_info(const RomInfo& info) { romInfo = info; }
    void step(int cycles);
    u8 read(u16 address) const;
    void write(u16 address, u8 value);

    [[nodiscard]] const u32* get_framebuffer() const { return framebuffer.data(); }
    [[nodiscard]] bool is_frame_ready() const { return frame_ready; }
    void clear_frame_ready() { frame_ready = false; }

    // Debug / Feature functions
    void init_window(bool debug, std::string_view rom_title, bool fullscreen = false);
    [[nodiscard]] bool isOpen() const;
    [[nodiscard]] bool isPaused() const { return paused; }
    void handleEvents(JoypadState& joypad);
    void update(float dtSeconds, u64 cyclesExecuted);
    void render();

    void recordOpcode(u16 pc, u8 opcode);
    void checkBreakpoint(u16 pc);

    [[nodiscard]] bool isStepRequested() const { return stepRequested; }
    void clearStepRequest() { stepRequested = false; }

    [[nodiscard]] bool isTurbo() const { return turbo; }
    void setTurbo(bool val) { turbo = val; }
    [[nodiscard]] int getActiveSlot() const { return activeSlot; }
    void setActiveSlot(int slot) { activeSlot = slot; }
    void reset();
    [[nodiscard]] bool isResetRequested() const { return resetRequested; }
    void clearResetRequest() { resetRequested = false; }

private:
    bool saveStateSlot(int slot);
    bool loadStateSlot(int slot);
    bool saveStatePath(std::string_view filepath);
    bool loadStatePath(std::string_view filepath);

    void drawPanels();
    void drawText(const sf::Vector2f& pos, std::string_view text, unsigned size, const sf::Color& color);
    [[nodiscard]] std::string toHex(u32 value, int width) const;

    // Emulation components
    std::unique_ptr<InterruptController> dummy_ic;
    InterruptController& ic;
    ProcessingUnit* cpu = nullptr;
    MMU* mmu = nullptr;
    APU* apu = nullptr;
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
    bool turbo{false};
    int activeSlot{0};
    bool resetRequested{false};
    std::string externalStatePath;
    JoypadState clickableJoypad{};
    JoypadState physicalJoypad{};
    JoypadState currentJoypadState{};

    float timeAccumulator{0.0f};
    u64 lastCycles{0};
    u64 lastReads{0};
    u64 lastWrites{0};

    std::string statusMessage;
    float statusTimer{0.0f};
    u32 frameCounter{0};

    // Audio mixer UI state (debug mode)
    float mixerMasterVol{1.0f};
    std::array<float, 4> mixerChannelVol{{1.0f, 1.0f, 1.0f, 1.0f}};
    std::array<bool, 4> mixerChannelMuted{{false, false, false, false}};
    int draggingSlider{-1}; // -1=none, 0=master, 1-4=channels
};
