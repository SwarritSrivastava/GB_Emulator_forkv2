#pragma once
#include "common.hpp"

typedef struct {            // Memory Map
  u8 entry[4];              // 0x0100-0x0103
  u8 nintendo_logo[0x30];   // 0x0104-0x0133
/*-----------------------------------------*/
  char title[11];           // 0x0134-0x013E
  u8 manufacturer_code[4];  // 0x013F-0x0142 - part of title (in older versions)
  u8 cgb_flag;              // 0x0143 - part of title
/*-------------------------------------------
# 0134-0143 — Title
These bytes contain the title of the game in upper case ASCII. If the title is less than 16 characters long, the remaining bytes should be padded with $00s.
Parts of this area actually have a different meaning on later cartridges, reducing the actual title size to 15 ($0134–$0142) or 11 ($0134–$013E) characters; see below.
# 013F-0142 — Manufacturer code
In older cartridges these bytes were part of the Title (see above). In newer cartridges they contain a 4-character manufacturer code (in uppercase ASCII). The purpose of the manufacturer code is unknown.
-------------------------------------------*/
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

bool cart_load(char *cart);
