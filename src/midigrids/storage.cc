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

#include "midigrids/midigrids.h"
#include "midigrids/storage.h"

using namespace avrlib;

namespace midigrids {

Eeprom eeprom;

/* extern */
Storage storage;

/* static */
uint8_t Storage::num_accessible_banks_;

/* static */
void Storage::Init() {
  uint16_t data;
  eeprom.Init();
  uint16_t address = kMaxNumBanks * kBankSize - 2;
  // Write the sequence 0xfadN at the end of each bank, where N is the
  // bank number.
  for (uint8_t i = kMaxNumBanks; i > 0; --i) {
    data = 0xfad0 + i;
    Write((uint8_t*)&data, address, 2);
    address -= kBankSize;
  }
  // Try to read back this data to figure out the actual number of banks.
  address = kMaxNumBanks * kBankSize - 2;
  num_accessible_banks_ = kMaxNumBanks;
  while (num_accessible_banks_ > 0) {
    Read((uint8_t*)&data, address, 2);
    if (data == 0xfad0 + num_accessible_banks_) {
      break;
    }
    --num_accessible_banks_;
    address -= kBankSize;
  }
#ifdef ENABLE_DEBUG_OUTPUT
  printf("Storage::Init: num_accessible_banks=%u\n", num_accessible_banks_);
#endif
}

/* static */
uint16_t Storage::Write(const uint8_t* data, uint16_t address, uint16_t size) {
  return eeprom.Write(address, data, size);
}

/* static */
uint16_t Storage::Read(uint8_t* data, uint16_t address, uint16_t size) {
  return eeprom.Read(address, size, data);
}

/* static */
uint16_t Storage::slot_address(uint8_t slot) {
  uint16_t address = kSlotSize * slot;
  if (address + kSlotSize > addressable_space_size())
    return kBadAddress;

  return address;
}

}  // midigrids
