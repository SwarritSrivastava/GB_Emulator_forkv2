#pragma once
#include "common.hpp"
#include <array>
#include <vector>

class MMU {
    private:
        u8 memory[0x10000]; // old flat memory array, will be phased out eventually

        // 8KB wram chip init to 0
        std::array<u8, 0x2000> wram{}; // (0xC000 - 0xDFFF)

        mutable u64 read_count{};
        mutable u64 write_count{};

    public:
        MMU();

        /**
         * @brief Maps a loaded ROM vector into the Game Boy's physical ROM space.
         * @param rom_data A vector containing the raw bytes of the .gb file.
         * @return true if successfully mapped, false if the vector was empty.
         */
        bool map_rom(const std::vector<u8>& rom_data);

        /**
         * @brief Reads a single byte from the emulated memory bus.
         * @param address The 16-bit memory address to read from.
         * @return The 8-bit value at the specified address.
         */
        u8 read(u16 address) const;

        /**
         * @brief Writes a single byte to the emulated memory bus.
         * Safeguards are in place to prevent overwriting the physical ROM.
         * @param address The 16-bit memory address to write to.
         * @param value The 8-bit value to store.
         */
        void write(u16 address, u8 value);

        u64 get_read_count() const { return read_count; }
        u64 get_write_count() const { return write_count; }

        std::vector<u8> dump_memory() const;
        bool load_memory(const std::vector<u8>& dump);
};
