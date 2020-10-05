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
// Stored slot interface.

#ifndef MIDIGRIDS_SLOT_H_
#define MIDIGRIDS_SLOT_H_

#include "midigrids/midigrids.h"
#include "midigrids/trk.h"

namespace midigrids {

struct SlotData {
  uint8_t size_;
  uint8_t name_[kNameLength];
  uint8_t channel_;
  uint8_t tempo_;
  int16_t swing_;
  uint8_t step_length_;
  uint8_t clock_mode_;
  uint8_t clock_output_;
  uint8_t pattern_;
  uint8_t map_x_;
  uint8_t map_y_;
  uint8_t map_chaos_;
  uint8_t accent_;
  uint8_t strobe_width_;
  uint8_t reserved_[32];
  Trk trk_[kNumTrk];
};

class Slot : public SlotData {
 public:
  Slot() { size_ = sizeof(SlotData); }

  void Init();
  void Read(uint8_t slot);
  void Write(uint8_t slot);

  static void ReadName(uint8_t slot, uint8_t name[kNameLength]);

private:
  void Validate();

  static void ValidateName(uint8_t name[kNameLength]);

};

}  // namespace midigrids

#endif // MIDIGRIDS_SLOT_H_
