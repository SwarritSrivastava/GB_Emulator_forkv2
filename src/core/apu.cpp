#include "../../include/apu.hpp"
#include <algorithm>
#include <cstring>

// ============================================================
// Duty table definition
// ============================================================
constexpr u8 APU::DUTY_TABLE[4][8];

// ============================================================
// Constructor / Reset
// ============================================================
APU::APU() { reset(); }

void APU::reset() {
  ch1 = PulseChannel{};
  ch2 = PulseChannel{};
  ch3 = WaveChannel{};
  ch4 = NoiseChannel{};

  nr50 = 0x77;
  nr51 = 0xF3;
  nr52 = 0xF1;
  power_on = true;

  frame_sequencer_timer = 0;
  frame_sequencer_step = 0;
  sample_timer = 0;

  hpf_left = 0.0f;
  hpf_right = 0.0f;

  write_pos.store(0);
  read_pos.store(0);
  ring_buffer.fill(0);

  // Default wave RAM pattern (alternating 0x00/0xFF)
  for (int i = 0; i < 16; i++)
    ch3.wave_ram[i] = (i % 2 == 0) ? 0x00 : 0xFF;
}

// ============================================================
// PulseChannel
// ============================================================
u8 APU::PulseChannel::get_output() const {
  if (!enabled || !dac_enabled)
    return 0;
  return DUTY_TABLE[duty][duty_position] ? current_volume : 0;
}

void APU::PulseChannel::clock_length() {
  if (length_enable && length_counter > 0) {
    length_counter--;
    if (length_counter == 0)
      enabled = false;
  }
}

void APU::PulseChannel::clock_envelope() {
  if (env_period == 0)
    return;
  if (env_timer > 0)
    env_timer--;
  if (env_timer == 0) {
    env_timer = env_period ? env_period : 8;
    if (env_add && current_volume < 15)
      current_volume++;
    else if (!env_add && current_volume > 0)
      current_volume--;
  }
}

void APU::PulseChannel::clock_sweep(bool &channel_enabled) {
  if (sweep_timer > 0)
    sweep_timer--;
  if (sweep_timer != 0)
    return;

  sweep_timer = sweep_period ? sweep_period : 8;
  if (!sweep_enabled || sweep_period == 0)
    return;

  int new_freq = sweep_shadow >> sweep_shift;
  if (sweep_negate) {
    new_freq = sweep_shadow - new_freq;
    sweep_calculated_with_negate = true;
  } else {
    new_freq = sweep_shadow + new_freq;
  }

  if (new_freq > 2047) {
    channel_enabled = false;
    enabled = false;
    return;
  }

  if (sweep_shift > 0) {
    sweep_shadow = new_freq;
    frequency = static_cast<u16>(new_freq);
    // Overflow check again
    int check = sweep_shadow + (sweep_negate ? -(sweep_shadow >> sweep_shift)
                                             : (sweep_shadow >> sweep_shift));
    if (check > 2047) {
      channel_enabled = false;
      enabled = false;
    }
  }
}

void APU::PulseChannel::trigger(bool has_sweep) {
  enabled = true;
  if (length_counter == 0)
    length_counter = 64;

  frequency_timer = (2048 - frequency) * 4;
  env_timer = env_period ? env_period : 8;
  current_volume = env_initial;
  dac_enabled = (env_initial > 0 || env_add);

  if (!dac_enabled)
    enabled = false;

  if (has_sweep) {
    sweep_shadow = frequency;
    sweep_timer = sweep_period ? sweep_period : 8;
    sweep_enabled = (sweep_period > 0 || sweep_shift > 0);
    sweep_calculated_with_negate = false;
    if (sweep_shift > 0) {
      int new_freq = sweep_shadow >> sweep_shift;
      if (sweep_negate)
        new_freq = sweep_shadow - new_freq;
      else
        new_freq = sweep_shadow + new_freq;
      if (new_freq > 2047)
        enabled = false;
    }
  }
}

// ============================================================
// WaveChannel
// ============================================================
u8 APU::WaveChannel::get_output() const {
  if (!enabled || !dac_enabled)
    return 0;
  u8 sample = wave_ram[position / 2];
  sample = (position % 2 == 0) ? (sample >> 4) : (sample & 0x0F);

  static const u8 shift_table[4] = {4, 0, 1, 2};
  return sample >> shift_table[volume_code];
}

void APU::WaveChannel::clock_length() {
  if (length_enable && length_counter > 0) {
    length_counter--;
    if (length_counter == 0)
      enabled = false;
  }
}

void APU::WaveChannel::trigger() {
  enabled = true;
  if (length_counter == 0)
    length_counter = 256;
  frequency_timer = (2048 - frequency) * 2;
  position = 0;
  if (!dac_enabled)
    enabled = false;
}

// ============================================================
// NoiseChannel
// ============================================================
int APU::NoiseChannel::get_divisor() const {
  static const int divisors[8] = {8, 16, 32, 48, 64, 80, 96, 112};
  return divisors[divisor_code];
}

u8 APU::NoiseChannel::get_output() const {
  if (!enabled || !dac_enabled)
    return 0;
  return (~lfsr & 1) ? current_volume : 0;
}

void APU::NoiseChannel::clock_length() {
  if (length_enable && length_counter > 0) {
    length_counter--;
    if (length_counter == 0)
      enabled = false;
  }
}

void APU::NoiseChannel::clock_envelope() {
  if (env_period == 0)
    return;
  if (env_timer > 0)
    env_timer--;
  if (env_timer == 0) {
    env_timer = env_period ? env_period : 8;
    if (env_add && current_volume < 15)
      current_volume++;
    else if (!env_add && current_volume > 0)
      current_volume--;
  }
}

void APU::NoiseChannel::trigger() {
  enabled = true;
  if (length_counter == 0)
    length_counter = 64;
  frequency_timer = get_divisor() << clock_shift;
  env_timer = env_period ? env_period : 8;
  current_volume = env_initial;
  lfsr = 0x7FFF;
  dac_enabled = (env_initial > 0 || env_add);
  if (!dac_enabled)
    enabled = false;
}

// ============================================================
// Frame Sequencer (512 Hz)
// ============================================================
void APU::clock_frame_sequencer() {
  switch (frame_sequencer_step) {
  case 0: // Length
    ch1.clock_length();
    ch2.clock_length();
    ch3.clock_length();
    ch4.clock_length();
    break;
  case 1:
    break; // Nothing
  case 2:  // Length + Sweep
    ch1.clock_length();
    ch2.clock_length();
    ch3.clock_length();
    ch4.clock_length();
    {
      bool e = ch1.enabled;
      ch1.clock_sweep(e);
      ch1.enabled = e;
    }
    break;
  case 3:
    break; // Nothing
  case 4:  // Length
    ch1.clock_length();
    ch2.clock_length();
    ch3.clock_length();
    ch4.clock_length();
    break;
  case 5:
    break; // Nothing
  case 6:  // Length + Sweep
    ch1.clock_length();
    ch2.clock_length();
    ch3.clock_length();
    ch4.clock_length();
    {
      bool e = ch1.enabled;
      ch1.clock_sweep(e);
      ch1.enabled = e;
    }
    break;
  case 7: // Envelope
    ch1.clock_envelope();
    ch2.clock_envelope();
    ch4.clock_envelope();
    break;
  }
  frame_sequencer_step = (frame_sequencer_step + 1) & 7;
}

// ============================================================
// Sample generation and mixing
// ============================================================
void APU::generate_sample() {
  if (!power_on || cached_muted) {
    push_sample(0, 0);
    return;
  }

  // Use cached per-channel volumes (updated once per step() call)
  int s1 = static_cast<int>(ch1.get_output() * cached_ch_vol[0]);
  int s2 = static_cast<int>(ch2.get_output() * cached_ch_vol[1]);
  int s3 = static_cast<int>(ch3.get_output() * cached_ch_vol[2]);
  int s4 = static_cast<int>(ch4.get_output() * cached_ch_vol[3]);

  // Mix into left/right based on NR51 panning
  int left = 0, right = 0;
  if (nr51 & 0x10)
    left += s1;
  if (nr51 & 0x20)
    left += s2;
  if (nr51 & 0x40)
    left += s3;
  if (nr51 & 0x80)
    left += s4;
  if (nr51 & 0x01)
    right += s1;
  if (nr51 & 0x02)
    right += s2;
  if (nr51 & 0x04)
    right += s3;
  if (nr51 & 0x08)
    right += s4;

  // Apply master volume (NR50: bits 6-4 left, bits 2-0 right)
  int vol_left = ((nr50 >> 4) & 0x07) + 1;
  int vol_right = (nr50 & 0x07) + 1;
  left *= vol_left;
  right *= vol_right;

  // Scale to int16 range: max raw = 4*15*8 = 480, scale to ~32000
  float fl = static_cast<float>(left) / 480.0f;
  float fr = static_cast<float>(right) / 480.0f;

  // High-pass filter to remove DC offset
  float out_l = fl - hpf_left;
  hpf_left = fl - out_l * HPF_CHARGE_FACTOR;
  float out_r = fr - hpf_right;
  hpf_right = fr - out_r * HPF_CHARGE_FACTOR;

  // Apply user volume (cached)
  out_l *= cached_master_vol;
  out_r *= cached_master_vol;

  // Clamp and convert to int16
  auto clamp16 = [](float v) -> std::int16_t {
    int val = static_cast<int>(v * 32000.0f);
    if (val > 32767)
      val = 32767;
    if (val < -32768)
      val = -32768;
    return static_cast<std::int16_t>(val);
  };

  push_sample(clamp16(out_l), clamp16(out_r));
}

void APU::push_sample(std::int16_t left, std::int16_t right) {
  // Lock-free SPSC: only the emulation thread writes
  std::size_t wp = write_pos.load(std::memory_order_relaxed);
  std::size_t rp = read_pos.load(std::memory_order_acquire);

  // Overflow protection: ensure space for 2 samples before writing
  std::size_t used = (wp >= rp) ? (wp - rp) : (BUFFER_SIZE - rp + wp);
  if (used >= BUFFER_SIZE - 2) {
    return; // Buffer full, drop sample to prevent corruption
  }

  ring_buffer[wp] = left;
  wp = (wp + 1) % BUFFER_SIZE;
  ring_buffer[wp] = right;
  wp = (wp + 1) % BUFFER_SIZE;
  write_pos.store(wp, std::memory_order_release);
}

std::size_t APU::read_samples(std::int16_t *out, std::size_t count) {
  // Lock-free SPSC: only the audio thread reads
  std::size_t rp = read_pos.load(std::memory_order_relaxed);
  std::size_t wp = write_pos.load(std::memory_order_acquire);
  std::size_t available = (wp >= rp) ? (wp - rp) : (BUFFER_SIZE - rp + wp);
  std::size_t to_read = std::min(count, available);

  for (std::size_t i = 0; i < to_read; i++) {
    out[i] = ring_buffer[rp];
    rp = (rp + 1) % BUFFER_SIZE;
  }
  read_pos.store(rp, std::memory_order_release);
  return to_read;
}

void APU::cache_volume_state() {
  cached_muted = muted.load(std::memory_order_relaxed);
  cached_master_vol = master_volume.load(std::memory_order_relaxed);
  for (int i = 0; i < 4; i++) {
    cached_ch_vol[i] = channel_muted[i].load(std::memory_order_relaxed)
                           ? 0.0f
                           : channel_volume[i].load(std::memory_order_relaxed);
  }
}

// ============================================================
// Main step function
// ============================================================
void APU::step(int cycles) {
  // Cache volume state once per step (avoids atomic loads in hot inner loop)
  cache_volume_state();

  if (!power_on) {
    for (int i = 0; i < cycles; i++) {
      sample_timer += SAMPLE_RATE;
      if (sample_timer >= CPU_CLOCK) {
        sample_timer -= CPU_CLOCK;
        push_sample(0, 0);
      }
    }
    return;
  }

  for (int i = 0; i < cycles; i++) {
    // Frame sequencer (every 8192 T-cycles)
    frame_sequencer_timer++;
    if (frame_sequencer_timer >= 8192) {
      frame_sequencer_timer = 0;
      clock_frame_sequencer();
    }

    // Channel 1 frequency timer
    if (ch1.frequency_timer > 0)
      ch1.frequency_timer--;
    if (ch1.frequency_timer == 0) {
      ch1.frequency_timer = (2048 - ch1.frequency) * 4;
      ch1.duty_position = (ch1.duty_position + 1) & 7;
    }

    // Channel 2 frequency timer
    if (ch2.frequency_timer > 0)
      ch2.frequency_timer--;
    if (ch2.frequency_timer == 0) {
      ch2.frequency_timer = (2048 - ch2.frequency) * 4;
      ch2.duty_position = (ch2.duty_position + 1) & 7;
    }

    // Channel 3 frequency timer
    if (ch3.frequency_timer > 0)
      ch3.frequency_timer--;
    if (ch3.frequency_timer == 0) {
      ch3.frequency_timer = (2048 - ch3.frequency) * 2;
      ch3.position = (ch3.position + 1) & 31;
    }

    // Channel 4 frequency timer
    if (ch4.frequency_timer > 0)
      ch4.frequency_timer--;
    if (ch4.frequency_timer == 0) {
      ch4.frequency_timer = ch4.get_divisor() << ch4.clock_shift;
      // Clock LFSR
      u8 xor_result = (ch4.lfsr & 1) ^ ((ch4.lfsr >> 1) & 1);
      ch4.lfsr >>= 1;
      ch4.lfsr |= (xor_result << 14);
      if (ch4.width_mode) {
        ch4.lfsr &= ~(1 << 6);
        ch4.lfsr |= (xor_result << 6);
      }
    }

    // Downsample to 44100 Hz
    sample_timer += SAMPLE_RATE;
    if (sample_timer >= CPU_CLOCK) {
      sample_timer -= CPU_CLOCK;
      generate_sample();
    }
  }
}

// ============================================================
// Register reads
// ============================================================
u8 APU::read(u16 address) const {
  // OR masks for read-back (unused bits read as 1)
  switch (address) {
  // Channel 1
  case 0xFF10:
    return 0x80 | (ch1.sweep_period << 4) | (ch1.sweep_negate ? 0x08 : 0) |
           ch1.sweep_shift;
  case 0xFF11:
    return (ch1.duty << 6) | 0x3F;
  case 0xFF12:
    return (ch1.env_initial << 4) | (ch1.env_add ? 0x08 : 0) | ch1.env_period;
  case 0xFF13:
    return 0xFF; // Write-only
  case 0xFF14:
    return (ch1.length_enable ? 0x40 : 0) | 0xBF;

  // Channel 2
  case 0xFF15:
    return 0xFF; // NR20 doesn't exist
  case 0xFF16:
    return (ch2.duty << 6) | 0x3F;
  case 0xFF17:
    return (ch2.env_initial << 4) | (ch2.env_add ? 0x08 : 0) | ch2.env_period;
  case 0xFF18:
    return 0xFF; // Write-only
  case 0xFF19:
    return (ch2.length_enable ? 0x40 : 0) | 0xBF;

  // Channel 3
  case 0xFF1A:
    return (ch3.dac_enabled ? 0x80 : 0) | 0x7F;
  case 0xFF1B:
    return 0xFF; // Write-only
  case 0xFF1C:
    return (ch3.volume_code << 5) | 0x9F;
  case 0xFF1D:
    return 0xFF; // Write-only
  case 0xFF1E:
    return (ch3.length_enable ? 0x40 : 0) | 0xBF;

  // Channel 4
  case 0xFF1F:
    return 0xFF; // Unused
  case 0xFF20:
    return 0xFF; // Write-only
  case 0xFF21:
    return (ch4.env_initial << 4) | (ch4.env_add ? 0x08 : 0) | ch4.env_period;
  case 0xFF22:
    return (ch4.clock_shift << 4) | (ch4.width_mode ? 0x08 : 0) |
           ch4.divisor_code;
  case 0xFF23:
    return (ch4.length_enable ? 0x40 : 0) | 0xBF;

  // Control
  case 0xFF24:
    return nr50;
  case 0xFF25:
    return nr51;
  case 0xFF26: {
    u8 status = 0x70; // Bits 6-4 always read as 1
    if (power_on)
      status |= 0x80;
    if (ch1.enabled)
      status |= 0x01;
    if (ch2.enabled)
      status |= 0x02;
    if (ch3.enabled)
      status |= 0x04;
    if (ch4.enabled)
      status |= 0x08;
    return status;
  }

  // Unused range 0xFF27-0xFF2F
  default:
    if (address >= 0xFF27 && address <= 0xFF2F)
      return 0xFF;
    // Wave RAM (0xFF30 - 0xFF3F)
    if (address >= 0xFF30 && address <= 0xFF3F) {
      return ch3.wave_ram[address - 0xFF30];
    }
    return 0xFF;
  }
}

// ============================================================
// Register writes
// ============================================================
void APU::write(u16 address, u8 value) {
  // Wave RAM is always writable
  if (address >= 0xFF30 && address <= 0xFF3F) {
    ch3.wave_ram[address - 0xFF30] = value;
    return;
  }

  // NR52 power control
  if (address == 0xFF26) {
    bool new_power = (value & 0x80) != 0;
    if (!new_power && power_on) {
      // Power off: clear all registers
      for (u16 a = 0xFF10; a <= 0xFF25; a++)
        write(a, 0);
      power_on = false;
      ch1.enabled = false;
      ch2.enabled = false;
      ch3.enabled = false;
      ch4.enabled = false;
    } else if (new_power && !power_on) {
      power_on = true;
      frame_sequencer_step = 0;
      frame_sequencer_timer = 0;
    }
    return;
  }

  // If power is off, only NR52 and wave RAM are writable
  if (!power_on)
    return;

  switch (address) {
  // Channel 1 registers
  case 0xFF10:
    ch1.sweep_period = (value >> 4) & 0x07;
    ch1.sweep_negate = (value & 0x08) != 0;
    ch1.sweep_shift = value & 0x07;
    // Switching from negate to positive after a negate calc disables channel
    if (!ch1.sweep_negate && ch1.sweep_calculated_with_negate)
      ch1.enabled = false;
    break;
  case 0xFF11:
    ch1.duty = (value >> 6) & 0x03;
    ch1.length_load = value & 0x3F;
    ch1.length_counter = 64 - ch1.length_load;
    break;
  case 0xFF12:
    ch1.env_initial = (value >> 4) & 0x0F;
    ch1.env_add = (value & 0x08) != 0;
    ch1.env_period = value & 0x07;
    ch1.dac_enabled = (value & 0xF8) != 0;
    if (!ch1.dac_enabled)
      ch1.enabled = false;
    break;
  case 0xFF13:
    ch1.frequency = (ch1.frequency & 0x700) | value;
    break;
  case 0xFF14:
    ch1.frequency = (ch1.frequency & 0xFF) | ((value & 0x07) << 8);
    ch1.length_enable = (value & 0x40) != 0;
    if (value & 0x80)
      ch1.trigger(true);
    break;

  // Channel 2 registers
  case 0xFF16:
    ch2.duty = (value >> 6) & 0x03;
    ch2.length_load = value & 0x3F;
    ch2.length_counter = 64 - ch2.length_load;
    break;
  case 0xFF17:
    ch2.env_initial = (value >> 4) & 0x0F;
    ch2.env_add = (value & 0x08) != 0;
    ch2.env_period = value & 0x07;
    ch2.dac_enabled = (value & 0xF8) != 0;
    if (!ch2.dac_enabled)
      ch2.enabled = false;
    break;
  case 0xFF18:
    ch2.frequency = (ch2.frequency & 0x700) | value;
    break;
  case 0xFF19:
    ch2.frequency = (ch2.frequency & 0xFF) | ((value & 0x07) << 8);
    ch2.length_enable = (value & 0x40) != 0;
    if (value & 0x80)
      ch2.trigger(false);
    break;

  // Channel 3 registers
  case 0xFF1A:
    ch3.dac_enabled = (value & 0x80) != 0;
    if (!ch3.dac_enabled)
      ch3.enabled = false;
    break;
  case 0xFF1B:
    ch3.length_load = value;
    ch3.length_counter = 256 - ch3.length_load;
    break;
  case 0xFF1C:
    ch3.volume_code = (value >> 5) & 0x03;
    break;
  case 0xFF1D:
    ch3.frequency = (ch3.frequency & 0x700) | value;
    break;
  case 0xFF1E:
    ch3.frequency = (ch3.frequency & 0xFF) | ((value & 0x07) << 8);
    ch3.length_enable = (value & 0x40) != 0;
    if (value & 0x80)
      ch3.trigger();
    break;

  // Channel 4 registers
  case 0xFF20:
    ch4.length_load = value & 0x3F;
    ch4.length_counter = 64 - ch4.length_load;
    break;
  case 0xFF21:
    ch4.env_initial = (value >> 4) & 0x0F;
    ch4.env_add = (value & 0x08) != 0;
    ch4.env_period = value & 0x07;
    ch4.dac_enabled = (value & 0xF8) != 0;
    if (!ch4.dac_enabled)
      ch4.enabled = false;
    break;
  case 0xFF22:
    ch4.clock_shift = (value >> 4) & 0x0F;
    ch4.width_mode = (value & 0x08) != 0;
    ch4.divisor_code = value & 0x07;
    break;
  case 0xFF23:
    ch4.length_enable = (value & 0x40) != 0;
    if (value & 0x80)
      ch4.trigger();
    break;

  // Control registers
  case 0xFF24:
    nr50 = value;
    break;
  case 0xFF25:
    nr51 = value;
    break;

  default:
    break;
  }
}
