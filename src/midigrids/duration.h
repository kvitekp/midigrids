// Copyright 2013 Peter Kvitek.
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

#ifndef MIDIGRIDS_DURATION_H_
#define MIDIGRIDS_DURATION_H_

#include "avrlib/base.h"
#include "midigrids/resources.h"

namespace midigrids {

enum NoteDuration {
  k64tNote,
  k32tNote,
  k32ndNote,
  k16tNote,
  k16thNote,
  k8tNote,
  k16dNote,
  k8thNote,
  k4tNote,
  k8dNote,
  k4thNote,
  k2tNote,
  k4dNote,
  k2ndNote,
  k1btNote,
  k2dNote,
  k1bNote,
  k2btNote,
  k15bNote,
  k2bNote,
  kNoteDurationCount
};

class Duration {
 public:
  Duration() {}

  static uint8_t GetMidiClockTicks(uint8_t duration);

private:

  DISALLOW_COPY_AND_ASSIGN(Duration);
};

extern const prog_uint8_t midi_clock_ticks_per_note[kNoteDurationCount];

static const uint8_t kNoteDurationStrLen = 5;

extern const prog_char midi_clock_ticks_per_note_str[kNoteDurationCount * kNoteDurationStrLen + 1];

}  // namespace midigrids

#endif  // MIDIGRIDS_DURATION_H_
