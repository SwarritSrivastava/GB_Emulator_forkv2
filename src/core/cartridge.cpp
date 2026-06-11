#include "../../include/cartridge.hpp"
#include <cstring>
#include <stdexcept>
#include <algorithm>

Cartridge::Cartridge() {
    rom.clear();
    ram.resize(32768, 0);
    ram_enabled = true;
    std::memset(&header, 0, sizeof(header));
    title = "";
}

Cartridge::Cartridge(const std::vector<u8>& rom_data) {
    load_rom(rom_data);
}

bool Cartridge::load_rom(const std::vector<u8>& rom_data) {
    if (rom_data.empty()) {
        return false;
    }

    rom = rom_data;
    mbc_type = MBCType::None;

    if (rom.size() >= 0x150) {
        std::memcpy(&header, &rom[0x100], sizeof(rom_header));

        // Safely copy the title
        char title_buf[17]{};
        std::memcpy(title_buf, header.title, 16);
        title = title_buf;

        // Determine MBC type
        u8 type = header.type;
        if (type >= 0x01 && type <= 0x03) {
            mbc_type = MBCType::MBC1;
        } else if (type >= 0x0F && type <= 0x13) {
            mbc_type = MBCType::MBC3;
        } else if (type >= 0x19 && type <= 0x1E) {
            mbc_type = MBCType::MBC5;
        }

        // Calculate RAM size based on header code
        u32 ram_size_bytes = 0;
        switch (header.ram_size) {
            case 1: ram_size_bytes = 2048; break;       // 2KB
            case 2: ram_size_bytes = 8192; break;       // 8KB
            case 3: ram_size_bytes = 32768; break;      // 32KB
            case 4: ram_size_bytes = 131072; break;     // 128KB
            case 5: ram_size_bytes = 65536; break;      // 64KB
            default: ram_size_bytes = 0; break;
        }

        if (ram_size_bytes > 0) {
            ram.resize(ram_size_bytes, 0);
        } else {
            ram.clear();
        }
    } else {
        std::memset(&header, 0, sizeof(rom_header));
        title = "Test ROM";
        ram.clear();
    }

    current_rom_bank = 1;
    current_ram_bank = 0;
    ram_enabled = false;
    banking_mode = 0;
    rom_bank_low = 1;
    rom_bank_high = 0;
    return true;
}

u8 Cartridge::read(u16 address) const {
    if (address <= 0x3FFF) {
        if (mbc_type == MBCType::MBC1 && banking_mode == 1) {
            u32 bank = (rom_bank_high << 5);
            u32 address_in_rom = (bank * 0x4000) + address;
            if (address_in_rom < rom.size()) return rom[address_in_rom];
        }
        if (address < rom.size()) return rom[address];
        return 0x00;
    }
    if (address >= 0x4000 && address <= 0x7FFF) {
        u32 bank = current_rom_bank;
        u32 address_in_rom = (address - 0x4000) + (bank * 0x4000);
        if (address_in_rom < rom.size()) return rom[address_in_rom];
        return 0x00;
    }
    if (address >= 0xA000 && address <= 0xBFFF) {
        if (ram_enabled && !ram.empty()) {
            u8 ram_bank = 0;
            if (mbc_type == MBCType::MBC1 && banking_mode == 1) {
                ram_bank = current_ram_bank;
            } else if (mbc_type == MBCType::MBC3 || mbc_type == MBCType::MBC5) {
                ram_bank = current_ram_bank;
            }
            u32 ram_address = (ram_bank * 0x2000) + (address - 0xA000);
            if (ram_address < ram.size()) return ram[ram_address];
        }
        return 0xFF;
    }
    return 0xFF;
}

void Cartridge::write(u16 address, u8 value) {
    // RAM Enable (0x0000 - 0x1FFF)
    if (address <= 0x1FFF) {
        if (mbc_type != MBCType::None) {
            ram_enabled = ((value & 0x0F) == 0x0A);
        }
        return;
    }
    // ROM Bank Number (0x2000 - 0x3FFF)
    if (address >= 0x2000 && address <= 0x3FFF) {
        if (mbc_type == MBCType::MBC1) {
            rom_bank_low = value & 0x1F;
            if (rom_bank_low == 0) rom_bank_low = 1;
            current_rom_bank = (rom_bank_high << 5) | rom_bank_low;
        } else if (mbc_type == MBCType::MBC3) {
            current_rom_bank = value & 0x7F;
            if (current_rom_bank == 0) current_rom_bank = 1;
        } else if (mbc_type == MBCType::MBC5) {
            if (address <= 0x2FFF) {
                rom_bank_low = value;
            } else {
                rom_bank_high = value & 0x01;
            }
            current_rom_bank = (rom_bank_high << 8) | rom_bank_low;
        }
        return;
    }
    // RAM Bank Number / ROM Bank Extension (0x4000 - 0x5FFF)
    if (address >= 0x4000 && address <= 0x5FFF) {
        if (mbc_type == MBCType::MBC1) {
            rom_bank_high = value & 0x03;
            current_ram_bank = value & 0x03;
            current_rom_bank = (rom_bank_high << 5) | rom_bank_low;
        } else if (mbc_type == MBCType::MBC3) {
            current_ram_bank = value & 0x0F;
        } else if (mbc_type == MBCType::MBC5) {
            current_ram_bank = value & 0x0F;
        }
        return;
    }
    // Banking Mode Select (0x6000 - 0x7FFF)
    if (address >= 0x6000 && address <= 0x7FFF) {
        if (mbc_type == MBCType::MBC1) {
            banking_mode = value & 0x01;
        }
        return;
    }
    // External RAM (0xA000 - 0xBFFF)
    if (address >= 0xA000 && address <= 0xBFFF) {
        if (ram_enabled && !ram.empty()) {
            u8 ram_bank = 0;
            if (mbc_type == MBCType::MBC1 && banking_mode == 1) {
                ram_bank = current_ram_bank;
            } else if (mbc_type == MBCType::MBC3 || mbc_type == MBCType::MBC5) {
                ram_bank = current_ram_bank;
            }
            u32 ram_address = (ram_bank * 0x2000) + (address - 0xA000);
            if (ram_address < ram.size()) ram[ram_address] = value;
        }
    }
}

void Cartridge::reset() {
    current_rom_bank = 1;
    current_ram_bank = 0;
    ram_enabled = false;
    banking_mode = 0;
    rom_bank_low = 1;
    rom_bank_high = 0;
    if (!ram.empty()) {
        std::fill(ram.begin(), ram.end(), 0);
    }
}
