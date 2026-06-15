#pragma once

#include "common.hpp"
#include <array>
#include <atomic>
#include <cstdint>
#include <vector>

class APU {
public:
  APU();
  ~APU() = default;

  // Called every CPU step with the number of T-cycles consumed
  void step(int cycles);

  // Register access (0xFF10 - 0xFF3F)
  [[nodiscard]] u8 read(u16 address) const;
  void write(u16 address, u8 value);

  void reset();

  // Audio output buffer interface (thread-safe, used by GBSoundStream)
  // Returns the number of samples actually read
  std::size_t read_samples(std::int16_t *out, std::size_t count);

  // Volume control
  void set_volume(float vol) { master_volume.store(vol); }
  [[nodiscard]] float get_volume() const { return master_volume.load(); }

  // Per-channel volume control (0.0 to 1.0)
  void set_channel_volume(int channel, float vol) {
      if (channel >= 0 && channel < 4) channel_volume[channel].store(vol);
  }
  [[nodiscard]] float get_channel_volume(int channel) const {
      if (channel >= 0 && channel < 4) return channel_volume[channel].load();
      return 1.0f;
  }

  // Per-channel mute control
  void set_channel_muted(int channel, bool m) {
      if (channel >= 0 && channel < 4) channel_muted[channel].store(m);
  }
  [[nodiscard]] bool is_channel_muted(int channel) const {
      if (channel >= 0 && channel < 4) return channel_muted[channel].load();
      return false;
  }

  // Mute control
  void set_muted(bool m) { muted.store(m); }
  [[nodiscard]] bool is_muted() const { return muted.load(); }

  static constexpr unsigned int SAMPLE_RATE = 44100;
  static constexpr int CPU_CLOCK = 4194304;

private:
  // =========================================================
  // Channel structures
  // =========================================================

  // Pulse channel with optional sweep (Channel 1 has sweep, Channel 2 does not)
  struct PulseChannel {
    // Registers
    u8 sweep_period = 0;       // NRx0 bits 6-4
    bool sweep_negate = false; // NRx0 bit 3
    u8 sweep_shift = 0;        // NRx0 bits 2-0

    u8 duty = 0;        // NRx1 bits 7-6
    u8 length_load = 0; // NRx1 bits 5-0

    u8 env_initial = 0;   // NRx2 bits 7-4
    bool env_add = false; // NRx2 bit 3
    u8 env_period = 0;    // NRx2 bits 2-0

    u16 frequency = 0;          // NRx3 (low 8) + NRx4 bits 2-0 (high 3)
    bool length_enable = false; // NRx4 bit 6

    // Internal state
    bool enabled = false;
    bool dac_enabled = false;
    int length_counter = 0;
    int frequency_timer = 0;
    int duty_position = 0;
    int env_timer = 0;
    int current_volume = 0;

    // Sweep state (Channel 1 only)
    bool sweep_enabled = false;
    int sweep_timer = 0;
    int sweep_shadow = 0;
    bool sweep_calculated_with_negate = false;

    [[nodiscard]] u8 get_output() const;
    void clock_length();
    void clock_envelope();
    void clock_sweep(bool &channel_enabled);
    void trigger(bool has_sweep);
  };

  // Wave channel (Channel 3)
  struct WaveChannel {
    // Registers
    bool dac_enabled = false;   // NR30 bit 7
    u8 length_load = 0;         // NR31
    u8 volume_code = 0;         // NR32 bits 6-5
    u16 frequency = 0;          // NR33 + NR34 bits 2-0
    bool length_enable = false; // NR34 bit 6

    // Internal state
    bool enabled = false;
    int length_counter = 0;
    int frequency_timer = 0;
    int position = 0;
    u8 sample_buffer = 0;

    // Wave RAM (16 bytes = 32 4-bit samples)
    std::array<u8, 16> wave_ram{};

    [[nodiscard]] u8 get_output() const;
    void clock_length();
    void trigger();
  };

  // Noise channel (Channel 4)
  struct NoiseChannel {
    // Registers
    u8 length_load = 0;         // NR41 bits 5-0
    u8 env_initial = 0;         // NR42 bits 7-4
    bool env_add = false;       // NR42 bit 3
    u8 env_period = 0;          // NR42 bits 2-0
    u8 clock_shift = 0;         // NR43 bits 7-4
    bool width_mode = false;    // NR43 bit 3 (7-bit mode)
    u8 divisor_code = 0;        // NR43 bits 2-0
    bool length_enable = false; // NR44 bit 6

    // Internal state
    bool enabled = false;
    bool dac_enabled = false;
    int length_counter = 0;
    int frequency_timer = 0;
    u16 lfsr = 0x7FFF;
    int env_timer = 0;
    int current_volume = 0;

    [[nodiscard]] u8 get_output() const;
    void clock_length();
    void clock_envelope();
    void trigger();
    [[nodiscard]] int get_divisor() const;
  };

  // =========================================================
  // APU state
  // =========================================================

  PulseChannel ch1; // Channel 1: Pulse + Sweep
  PulseChannel ch2; // Channel 2: Pulse
  WaveChannel ch3;  // Channel 3: Wave
  NoiseChannel ch4; // Channel 4: Noise

  // Control registers
  u8 nr50 = 0x77; // Master volume / VIN panning
  u8 nr51 = 0xF3; // Channel panning
  u8 nr52 = 0xF1; // Sound on/off + channel status

  bool power_on = true;

  // Frame sequencer (512 Hz, ticks every 8192 T-cycles)
  int frame_sequencer_timer = 0;
  int frame_sequencer_step = 0;

  void clock_frame_sequencer();

  // =========================================================
  // Sample generation
  // =========================================================

  // Accumulator for downsampling from CPU clock to SAMPLE_RATE
  int sample_timer = 0;
  static constexpr int SAMPLE_PERIOD =
      CPU_CLOCK / SAMPLE_RATE; // 95 cycles per sample

  void generate_sample();

  // High-pass filter state (one per stereo channel)
  float hpf_left = 0.0f;
  float hpf_right = 0.0f;
  static constexpr float HPF_CHARGE_FACTOR =
      0.996014f; // capacitor discharge rate

  // =========================================================
  // Ring buffer for audio output thread
  // =========================================================

  static constexpr std::size_t BUFFER_SIZE = 32768;
  std::array<std::int16_t, BUFFER_SIZE> ring_buffer{};
  std::atomic<std::size_t> write_pos{0};
  std::atomic<std::size_t> read_pos{0};

  void push_sample(std::int16_t left, std::int16_t right);

  // Volume and mute controls
  std::atomic<float> master_volume{1.0f};
  std::atomic<bool> muted{false};

  // Per-channel volume and mute
  std::array<std::atomic<float>, 4> channel_volume{{{1.0f}, {1.0f}, {1.0f}, {1.0f}}};
  std::array<std::atomic<bool>, 4> channel_muted{{{false}, {false}, {false}, {false}}};

  // Cached volume values (updated once per step() call, used in generate_sample)
  float cached_master_vol{1.0f};
  bool cached_muted{false};
  float cached_ch_vol[4]{1.0f, 1.0f, 1.0f, 1.0f};

  void cache_volume_state();

  // Duty cycle waveform patterns
  static constexpr u8 DUTY_TABLE[4][8] = {
      {0, 0, 0, 0, 0, 0, 0, 1}, // 12.5%
      {1, 0, 0, 0, 0, 0, 0, 1}, // 25%
      {1, 0, 0, 0, 0, 1, 1, 1}, // 50%
      {0, 1, 1, 1, 1, 1, 1, 0}, // 75%
  };
};
