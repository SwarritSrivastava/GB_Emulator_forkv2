#pragma once

#include "ProcessingUnit.hpp"
#include "mmu.hpp"
#include "common.hpp"
#include <SFML/Graphics.hpp>
#include <deque>
#include <string>
#include <vector>

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

private:
    bool saveStateSlot(int slot);
    bool loadStateSlot(int slot);

    void updateFrameBufferPlaceholder();
    void drawPanels();
    void drawText(const sf::Vector2f& pos, const std::string& text, unsigned size, const sf::Color& color);
    std::string toHex(u32 value, int width) const;

    ProcessingUnit& cpu;
    MMU& mmu;
    RomInfo romInfo;

    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded{false};

    sf::Texture screenTexture;
    sf::Sprite screenSprite;
    std::vector<u8> screenPixels;

    std::deque<OpcodeTrace> opcodeLog;
    std::vector<u16> breakpoints;
    bool paused{false};

    float timeAccumulator{0.0f};
    u64 lastCycles{0};
    u64 lastReads{0};
    u64 lastWrites{0};

    std::string statusMessage;
    float statusTimer{0.0f};
    u32 frameCounter{0};

public:
    PPU(ProcessingUnit& cpu, MMU& mmu, RomInfo info);

    bool isOpen() const;
    bool isPaused() const { return paused; }
    void handleEvents();
    void update(float dtSeconds, u64 cyclesExecuted);
    void render();

    void recordOpcode(u16 pc, u8 opcode);
    void checkBreakpoint(u16 pc);
};
