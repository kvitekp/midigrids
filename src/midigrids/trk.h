// Copyright 2018 Peter Kvitek.
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
// Track model declarations.

#ifndef MIDIGRIDS_TRK_H_
#define MIDIGRIDS_TRK_H_

#include "midigrids/midigrids.h"

namespace midigrids {

class Trk {
public:

  Trk() {}

  void Init();
  void Validate();

  enum TrkFlags {
    kMuted    = 0x01,
  };

  // Accessors
  #define DEFINE_ACCESSORS(type, name) \
    type name() const { return name##_; } \
    void set_##name(type name) { name##_ = name; }

  #define DEFINE_FLAG_ACCESSORS(name, flag) \
    uint8_t name() const { return (flags##_ & flag) ? 1 : 0; } \
    void set_##name(uint8_t state) { SETFLAGTO(flags##_, flag, state); }

  DEFINE_ACCESSORS(uint8_t, fill)
  DEFINE_ACCESSORS(uint8_t, size)
  DEFINE_ACCESSORS(uint8_t, note)
  DEFINE_ACCESSORS(uint8_t, velocity)
  DEFINE_ACCESSORS(uint8_t, flags)

  DEFINE_FLAG_ACCESSORS(muted, kMuted)

  #undef DEFINE_ACCESSORS
  #undef DEFINE_FLAG_ACCESSORS

private:
  uint8_t fill_;
  uint8_t size_;
  uint8_t note_;
  uint8_t velocity_;
  uint8_t flags_;

};

}  // namespace midigrids

#endif // MIDIGRIDS_TRK_H_
