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
// Stored slot implementation.

#include "midigrids/slot.h"
#include "midigrids/storage.h"
#include "midigrids/clock.h"
#include "midigrids/eemem.h"

namespace midigrids {

using namespace avrlib;

void Slot::Init() {
  size_ = sizeof(SlotData);
  memset(name_, '_', kNameLength);
  channel_ = kDefChannel;
  tempo_ = kDefTempo;
  swing_ = kDefSwing;
  step_length_ = kDefStepLength;
  clock_mode_ = kDefClockMode;
  clock_output_ = kDefClockOut;
  pattern_ = kDefPattern;
  map_x_ = kDefMapXY;
  map_y_ = kDefMapXY;
  map_chaos_ = kDefMapChaos;
  accent_ = kDefAccent;
  strobe_width_ = kDefStrobeWidth;

  memset(reserved_, 0, sizeof(reserved_));

  for (uint8_t n = 0; n < kNumTrk; n++) {
    trk_[n].Init();
  }

  trk_[0].set_note(kDefTrk1Note);
  trk_[1].set_note(kDefTrk2Note);
  trk_[2].set_note(kDefTrk3Note);
  trk_[3].set_note(kDefTrk4Note);

  trk_[0].set_fill(kDefTrk1Fill);
  trk_[1].set_fill(kDefTrk2Fill);
  trk_[2].set_fill(kDefTrk3Fill);
  trk_[3].set_fill(kDefTrk4Fill);
}

void Slot::Read(uint8_t slot) {
  Init();

  uint16_t address = storage.slot_address(slot);
  if (address == Storage::kBadAddress)
    return;

  uint8_t size;
  if (storage.Read((uint8_t*)&size, address, sizeof(size)) != sizeof(size))
    return;

  if (size > sizeof(SlotData))
    return;

  if (storage.Read((uint8_t*)this, address, size) != size) {
    Init();
    return;
  }

  Validate();
}

/* static */
void Slot::ReadName(uint8_t slot, uint8_t name[kNameLength]) {
  memset(name, '_', kNameLength);

  uint16_t address = storage.slot_address(slot);
  if (address == Storage::kBadAddress)
    return;

  uint8_t size;
  if (storage.Read((uint8_t*)&size, address, sizeof(size)) != sizeof(size))
    return;

  if (size > sizeof(SlotData))
    return;

  if (storage.Read(name, address + OFFSETOF(SlotData, name_), kNameLength) != kNameLength) {
    memset(name, '_', kNameLength);
    return;
  }

  ValidateName(name);
}

void Slot::Write(uint8_t slot) {
  uint16_t address = storage.slot_address(slot);
  if (address == Storage::kBadAddress)
    return;

  storage.Write((uint8_t*)this, address, sizeof(SlotData));
}

#define FIX_DEF_(name, Name) \
  if (name < kMin##Name || name > kMax##Name) { \
    name = kDef##Name; \
  }

#define FIX_DEF2(name, Name, Def) \
  if (name < kMin##Name || name > kMax##Name) { \
    name = Def; \
  }

void Slot::Validate() {
  ValidateName(name_);

  if (channel_ > kMaxChannel) {
Fix:Init();
    return;
  }

  FIX_DEF_(tempo_, Tempo)
  FIX_DEF_(swing_, Swing)
  FIX_DEF_(step_length_, StepLength)
  FIX_DEF_(clock_mode_, ClockMode);
  FIX_DEF_(clock_output_, ClockOut);
  FIX_DEF_(pattern_, Pattern)
  FIX_DEF_(map_x_, MapXY)
  FIX_DEF_(map_y_, MapXY)
  FIX_DEF_(map_chaos_, MapChaos)
  FIX_DEF_(accent_, Accent)
  FIX_DEF_(strobe_width_, StrobeWidth)

  for (uint8_t n = 0; n < kNumTrk; n++) {
    trk_[n].Validate();
  }
}

/* static */
void Slot::ValidateName(uint8_t name[kNameLength]) {
  for (uint8_t n = 0; n < kNameLength; n++) {
    if (name[n] < kMinNameChar || name[n] > kMaxNameChar) {
      name[n] = '_';
    }
  }
}

}  // namespace midigrids

