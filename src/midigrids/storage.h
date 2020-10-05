// Copyright 2012 Peter Kvitek.
//
// Author: Peter Kvitek (pete@kvitek.com)
//
// Based on Shruthi-1 code by Olivier Gillet (ol.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
//
// Program storage in external EEPROM.

#ifndef MIDIGRIDS_STORAGE_H_
#define MIDIGRIDS_STORAGE_H_

#include <string.h>

#include "avrlib/base.h"

#include "midigrids/hardware_config.h"

namespace midigrids {

enum {
  kSlotSize = 512,
  kMaxNumSlots = 256,
};

// Note that 2 bytes at the end of the 256 block are garbled by
// eeprom memory availability check!

class Storage {
 public:
  Storage() {}
  static void Init();

  static const uint16_t kBadAddress = (uint16_t)-1;

  static uint16_t slot_address(uint8_t slot);

  static uint32_t addressable_space_size() {
    return (uint32_t)num_accessible_banks_ * kBankSize;
  }

  static uint16_t num_slots() {
    return addressable_space_size() / kSlotSize;
  }

  static uint16_t Write(const uint8_t* data, uint16_t address, uint16_t size);
  static uint16_t Read(uint8_t* data, uint16_t address, uint16_t size);

 private:
  static uint8_t num_accessible_banks_;

  DISALLOW_COPY_AND_ASSIGN(Storage);
};

extern Storage storage;

}  // namespace midigrids

#endif // MIDIGRIDS_STORAGE_H_
