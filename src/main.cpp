#include "../include/cartridge.hpp"
#include "../include/success.hpp"
#include "../include/readROM.hpp"
#include "../include/ProcessingUnit.hpp"
#include "../include/mmu.hpp"
#include "../include/ppu.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <iterator>
#include <SFML/Graphics.hpp>

// Game Boy button state (active = true = pressed)
struct JoypadState {
    bool right = false, left = false, up = false, down = false;
    bool a = false, b = false, select = false, start = false;
};

static void update_joypad(MMU& mmu, const JoypadState& joy) {
    u8 action = 0x0F;
    u8 direction = 0x0F;

    if (joy.right) direction &= ~0x01;
    if (joy.left)  direction &= ~0x02;
    if (joy.up)    direction &= ~0x04;
    if (joy.down)  direction &= ~0x08;

    if (joy.a)      action &= ~0x01;
    if (joy.b)      action &= ~0x02;
    if (joy.select) action &= ~0x04;
    if (joy.start)  action &= ~0x08;

    mmu.set_joypad_state(action, direction);
}

int main(const int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: gb_emu <rom_file>\n";
        return 1;
    }

    try {
        const std::vector<u8> rom_data = load_rom(argv[1]);
        MMU mmu;
        PPU ppu;
        mmu.set_ppu(&ppu);
        ppu.set_mmu(&mmu);
        
        if(!mmu.map_rom(rom_data)) return 1;

        ProcessingUnit cpu;
        sf::RenderWindow window(sf::VideoMode({160 * 4, 144 * 4}), "GB Emulator");
        window.setFramerateLimit(60);

        sf::Texture texture;
        if (!texture.resize({160, 144})) return 1;
        sf::Sprite sprite(texture);
        sprite.setScale({4.0f, 4.0f});

        JoypadState joypad;

        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();

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
                        default: break;
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

            update_joypad(mmu, joypad);

            while (!ppu.is_frame_ready()) {
                int cycles = cpu.step(mmu);
                ppu.step(cycles);
                mmu.step_timer(cycles);
            }
            ppu.clear_frame_ready();

            texture.update(reinterpret_cast<const uint8_t*>(ppu.get_framebuffer()));
            window.clear();
            window.draw(sprite);
            window.display();
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
