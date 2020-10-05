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

#include "midigrids/duration.h"

namespace midigrids {

using namespace avrlib;

/*
static const uint8_t k64tNoteTicks  = 1;   // 64th triplet
static const uint8_t k32tNoteTicks  = 2;   // 32nd triplet
static const uint8_t k32ndNoteTicks = 3;   // 32nd
static const uint8_t k16tNoteTicks  = 4;   // 16th triplet
static const uint8_t k16thNoteTicks = 6;   // 16th
static const uint8_t k8tNoteTicks   = 8;   // 8th triplet
static const uint8_t k16dNoteTicks  = 9;   // 16th dotted
static const uint8_t k8thNoteTicks  = 12;  // 8th
static const uint8_t k4tNoteTicks   = 16;  // 4th triplet
static const uint8_t k8dNoteTicks   = 18;  // 8th dotted
static const uint8_t k4thNoteTicks  = 24;  // 4th
static const uint8_t k2tNoteTicks   = 32;  // 2nd triplet
static const uint8_t k4dNoteTicks   = 36;  // 4th dotted
static const uint8_t k2ndNoteTicks  = 48;  // 2nd
static const uint8_t k1btNoteTicks  = 64;  // 1 Bar triplet
static const uint8_t k2dNoteTicks   = 72;  // 2nd dotted
static const uint8_t k1bNoteTicks   = 96;  // 1 Bar
static const uint8_t k2btNoteTicks  = 128; // 2 Bar triplet
static const uint8_t k15bNoteTicks  = 144; // 1 Bar dotted
static const uint8_t k2bNoteTicks   = 192; // 2 Bars
*/

/* extern */
const prog_uint8_t midi_clock_ticks_per_note[kNoteDurationCount] PROGMEM = {
   1,   2,   3,   4,   6,   8,   9,   12,  16,  18,  24,  32,  36,  48,  64,  72,  96,  128, 144, 192,
// |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----
};

/* extern */
const prog_char midi_clock_ticks_per_note_str[kNoteDurationCount * kNoteDurationStrLen + 1] PROGMEM =
  "1/64T1/32T1/32 1/16T1/16 1/8T 1/16.1/8  1/4T 1/8. 1/4  1/2T 1/4. 1/2  1T   1/2. 1    2T   1.   2    ";
// |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----

/* static */
uint8_t Duration::GetMidiClockTicks(uint8_t duration) {
  return ResourcesManager::Lookup<uint8_t, uint8_t>(midi_clock_ticks_per_note, duration);
}

}  // namespace midigrids
