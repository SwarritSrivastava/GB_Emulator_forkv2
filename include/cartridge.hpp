#pragma once
#include <common.hpp>

typedef struct {
  u8 entry[4];              // 0x0100-0x0103
  u8 nintendo_logo[0x30];   // 0x0104-0x0133
/*-----------------------------------------*/
  char title[16];           // 0x0134-0x0143
  u8 manufacturer_code[4];  // 0x013F-0x0142
  u8 cgb_flag;              // 0x0143
  u16 new_lic_code;         // 0x0144-0x0145
  u8 sgb_flag;              // 0x0146
  u8 type;                  // 0x0147
  u8 rom_size;              // 0x0148
  u8 ram_size;              // 0x0149
  u8 dest_code;             // 0x014A
  u8 lic_code;              // 0x014B
  u8 version;               // 0x014C
  u8 checksum;              // 0x014D
  u16 global_checksum;      // 0x014E-0x014F
} rom_header;

bool cart_load(char *cart);
