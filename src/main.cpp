#include "../include/ProcessingUnit.hpp"
#include "../include/apu.hpp"
#include "../include/audio_stream.hpp"
#include "../include/cartridge.hpp"
#include "../include/interrupt_controller.hpp"
#include "../include/mmu.hpp"
#include "../include/ppu.hpp"
#include "../include/readROM.hpp"
#include "../include/success.hpp"
#include "../include/timer.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstring>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <thread>
#include <stdexcept>

static void update_joypad(MMU &mmu, const JoypadState &joy) {
  u8 action = 0x0F;
  u8 direction = 0x0F;

  if (joy.right)
    direction &= ~0x01;
  if (joy.left)
    direction &= ~0x02;
  if (joy.up)
    direction &= ~0x04;
  if (joy.down)
    direction &= ~0x08;

  if (joy.a)
    action &= ~0x01;
  if (joy.b)
    action &= ~0x02;
  if (joy.select)
    action &= ~0x04;
  if (joy.start)
    action &= ~0x08;

  mmu.set_joypad_state(action, direction);
}

int main(const int argc, char **argv) {
  bool debug_mode = false;
  bool fullscreen_mode = false;
  std::string rom_path;
  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "--debug") == 0 ||
        std::strcmp(argv[i], "-d") == 0) {
      debug_mode = true;
    } else if (std::strcmp(argv[i], "--fullscreen") == 0 ||
               std::strcmp(argv[i], "-f") == 0) {
      fullscreen_mode = true;
    } else {
      rom_path = argv[i];
    }
  }

  if (rom_path.empty()) {
    std::cerr << "Usage: gb_emu <rom_file> [--debug|-d] [--fullscreen|-f]\n";
    return 1;
  }

  try {
    const std::vector<u8> rom_data = load_rom(rom_path.c_str());
    Cartridge cartridge(rom_data);
    InterruptController ic;
    Timer timer(ic);
    PPU ppu(ic);
    APU apu;
    MMU mmu(cartridge, ppu, timer, ic, apu);

    ProcessingUnit cpu;
    ppu.set_cpu(&cpu);
    ppu.set_mmu(&mmu);
    ppu.set_apu(&apu);

    // Populate ROM Info for the debugger panel
    rom_header header{};
    std::memcpy(&header, &rom_data[0x100], sizeof(rom_header));
    char title[17]{};
    std::memcpy(title, header.title, 16);

    PPU::RomInfo info{};
    info.title = title;
    info.type = header.type;
    info.rom_size = header.rom_size;
    info.ram_size = header.ram_size;
    info.rom_bytes = rom_data;
    ppu.set_rom_info(info);

    ppu.init_window(debug_mode, title, fullscreen_mode);
    std::cout << "Running in " << (debug_mode ? "Debug/Feature" : "Simple")
              << " Mode...\n";

    JoypadState joypad;
    sf::Clock clock;
    u64 cycles = 0;
    int frame_count = 0;

    // Start audio playback
    GBSoundStream audio_stream(apu);
    audio_stream.play();

    // Audio-driven pacing: target buffer fill level (stereo int16 samples).
    // ~2048 samples ≈ 23ms of audio at 44100 Hz — low enough for responsive
    // controls, high enough to absorb scheduling jitter.
    constexpr std::size_t AUDIO_TARGET_SAMPLES = 2048;

    while (ppu.isOpen()) {
      ppu.handleEvents(joypad);
      update_joypad(mmu, joypad);

      if (ppu.isResetRequested()) {
        mmu.reset();
        cpu.reset();
        ppu.clearResetRequest();
      }

      if (!ppu.isPaused()) {
        const int loops = ppu.isTurbo() ? 2 : 1;
        for (int loop = 0; loop < loops; ++loop) {
          int frame_cycles = 0;
          static int instr_count = 0;
          static bool trace_enabled = (std::getenv("EMU_TRACE") != nullptr);
          static std::ofstream trace_file;
          if (trace_enabled && !trace_file.is_open()) {
            trace_file.open("cpu_trace.log");
          }
          while (!ppu.is_frame_ready() &&
                 ((ppu.read(0xFF40) & 0x80) || frame_cycles < 70224)) {
            const u16 pc_before = cpu.get_pc();
            const u8 opcode = mmu.read(pc_before);

            if (trace_enabled && instr_count % 10000 == 0 &&
                instr_count < 1000000) {
              trace_file << "Instr: " << std::dec << instr_count << " | PC: 0x"
                         << std::hex << pc_before << " | Op: 0x" << (int)opcode
                         << " | AF: 0x" << cpu.get_af() << " | BC: 0x"
                         << cpu.get_bc() << " | DE: 0x" << cpu.get_de()
                         << " | HL: 0x" << cpu.get_hl() << " | SP: 0x"
                         << cpu.get_sp() << " | LY: 0x" << (int)ppu.read(0xFF44)
                         << " | Bank: " << (int)mmu.get_current_rom_bank()
                         << "\n";
              trace_file.flush();
            }
            instr_count++;

            int used = cpu.step(mmu);
            cycles += static_cast<u64>(used);
            frame_cycles += used;
            ppu.step(used);
            mmu.step_timer(used);
            apu.step(used);

            ppu.recordOpcode(pc_before, opcode);
            ppu.checkBreakpoint(pc_before);

            if (ppu.isPaused()) {
              break;
            }
          }
          if (ppu.is_frame_ready()) {
            ppu.clear_frame_ready();
          }
          frame_count++;
        }
        char *frame_limit_env = std::getenv("EMU_FRAME_LIMIT");
        int limit = frame_limit_env ? std::atoi(frame_limit_env) : -1;
        if (limit > 0 && frame_count >= limit) {
          std::cout << "Reached frame limit of " << limit
                    << ", exiting cleanly.\n";
          break;
        }

        // ── Audio-driven frame pacing ──
        // Sleep until the audio thread has consumed enough samples.
        // This naturally locks emulation to ~59.7275 Hz (the Game Boy's
        // true frame rate) instead of the host's 60 Hz vsync.
        if (!ppu.isTurbo()) {
          while (apu.buffered_samples() > AUDIO_TARGET_SAMPLES &&
                 ppu.isOpen()) {
            std::this_thread::sleep_for(std::chrono::microseconds(500));
          }
        }
      } else {
        if (ppu.isStepRequested()) {
          const u16 pc_before = cpu.get_pc();
          const u8 opcode = mmu.read(pc_before);

          int used = cpu.step(mmu);
          cycles += static_cast<u64>(used);
          ppu.step(used);
          mmu.step_timer(used);
          apu.step(used);

          ppu.recordOpcode(pc_before, opcode);
          ppu.checkBreakpoint(pc_before);

          ppu.clearStepRequest();
        }
        // Sleep while paused to avoid busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
      }

      const float dt = clock.restart().asSeconds();
      ppu.update(dt, cycles);
      ppu.render();
    }
    if (std::getenv("EMU_SAVE_PNG") != nullptr) {
      sf::Image img({160, 144},
                    reinterpret_cast<const uint8_t *>(ppu.get_framebuffer()));
      (void)img.saveToFile("framebuffer.png");
      std::cout << "Saved framebuffer.png\n";
    }

    std::cout << std::dec << "\n" << cycles << " cycles executed" << std::endl;

    // Stop audio
    audio_stream.stop();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
