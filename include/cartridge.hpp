#pragma once

#include <vector>
#include <string>
#include "common.hpp"

typedef struct {            // Memory Map
  u8 entry[4];              // 0x0100-0x0103
  u8 nintendo_logo[0x30];   // 0x0104-0x0133
  char title[16];           // 0x0134-0x0143
  u16 new_lic_code;         // 0x0144-0x0145
  u8 sgb_flag;              // 0x0146
  u8 type;                  // 0x0147
  u8 rom_size;              // 0x0148
  u8 ram_size;              // 0x0149
  u8 dest_code;             // 0x014A
  u8 old_lic_code;          // 0x014B
  u8 version;               // 0x014C
  u8 checksum;              // 0x014D
  u16 global_checksum;      // 0x014E-0x014F
} rom_header;

class Cartridge {
public:
    Cartridge();
    explicit Cartridge(const std::vector<u8>& rom_data);
    ~Cartridge() = default;

    bool load_rom(const std::vector<u8>& rom_data);
    
    u8 read(u16 address) const;
    void write(u16 address, u8 value);
    void reset();

    std::string get_title() const { return title; }
    u8 get_mbc_type() const { return header.type; }
    u8 get_rom_size_code() const { return header.rom_size; }
    u8 get_ram_size_code() const { return header.ram_size; }
    const std::vector<u8>& get_rom_bytes() const { return rom; }
    
    u8 get_current_rom_bank() const { return current_rom_bank; }
    void set_current_rom_bank(u8 bank) { current_rom_bank = bank; }

    const std::vector<u8>& get_ram_bytes() const { return ram; }
    std::vector<u8>& get_ram_bytes_mutable() { return ram; }
    void set_ram_bytes(const std::vector<u8>& ram_data) { ram = ram_data; }
    
    bool is_ram_enabled() const { return ram_enabled; }
    void set_ram_enabled(bool enabled) { ram_enabled = enabled; }

private:
    enum class MBCType {
        None,
        MBC1,
        MBC3,
        MBC5
    };
    MBCType mbc_type = MBCType::None;

    std::vector<u8> rom;
    std::vector<u8> ram;
    rom_header header{};
    std::string title;
    
    u8 current_rom_bank = 1;
    u8 current_ram_bank = 0;
    bool ram_enabled = false;
    u8 banking_mode = 0;
    u8 rom_bank_low = 1;
    u8 rom_bank_high = 0;
};
