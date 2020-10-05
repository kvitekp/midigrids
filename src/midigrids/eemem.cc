// Copyright 2017 Peter Kvitek.
//
// Author: Peter Kvitek (pete@kvitek.com)
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
// Onboard EEPROM interface.

#include "midigrids/eemem.h"
#include "midigrids/slot.h"

#include <avr/eeprom.h>

namespace midigrids {

using namespace avrlib;

/* extern */
Eemem eemem;

static const uint16_t kEememMagic = 0xfee0;

struct EememData {
  uint16_t magic_;
  uint16_t size_;
  uint8_t slot_;
};

EememData EEMEM eememData;

/* static */
void Eemem::Init() {
  // Check if already initialized
  uint16_t initialized_size = 0;
  if (eeprom_read_word(&eememData.magic_) == kEememMagic) {
    initialized_size = eeprom_read_word(&eememData.size_);
    if (initialized_size == sizeof(EememData))
      return;
  }

  #define FIXSIZE(field) \
    if (initialized_size > sizeof(field)) initialized_size-= sizeof(field); \
    else initialized_size = 0;

  // Init initialization controls
  eeprom_update_word(&eememData.magic_, kEememMagic); FIXSIZE(eememData.magic_);
  eeprom_update_word(&eememData.size_, sizeof(EememData)); FIXSIZE(eememData.size_);

  // Init slot
  if (initialized_size < sizeof(eememData.slot_)) {
    set_slot(0);
  } FIXSIZE(eememData.slot_);

  #undef FIXSIZE
}

/* static */
void Eemem::Reset() {
  eeprom_update_word(&eememData.magic_, ~kEememMagic);
  Init();
}

/* static */
uint8_t Eemem::slot() {
  uint8_t slot = eeprom_read_byte(&eememData.slot_);
  if (slot >= kNumSlots) slot = 0;
  return slot;
}

/* static */
void Eemem::set_slot(uint8_t slot) {
  eeprom_update_byte(&eememData.slot_, slot);
}

}  // namespace midigrids

