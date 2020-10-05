// Copyright 2015 Peter Kvitek.
//
// Author: Peter Kvitek (pete@kvitek.com)
//
// Based on: Olivier Gillet (ol.gillet@gmail.com)
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
// Global clock.

#ifndef MIDIGRIDS_CLOCK_H_
#define MIDIGRIDS_CLOCK_H_

#include "midigrids/midigrids.h"

namespace midigrids {

static const uint16_t kNumTicksPerClock = 1; // 24PPQN
static const uint16_t kNumTicksPer4thNote = kNumTicksPerClock * 24;       // 24
static const uint16_t kNumTicksPer8thNote = kNumTicksPer4thNote / 2;      // 12
static const uint16_t kNumTicksPer16thNote = kNumTicksPer8thNote / 2;     // 6
static const uint16_t kNumTicksPer32ndNote = kNumTicksPer16thNote / 2;    // 3

static const int16_t kNoSwing = 500;

typedef void (*ClockCallback)();

class Clock {
 public:
  static void Init();
  static void Start();

  static uint16_t tick_count_limit() { return tick_count_limit_; }
  static void set_tick_count_limit(uint16_t tick_count_limit) {
    tick_count_limit_ = tick_count_limit;
  }

  static uint16_t Tick();

  static uint16_t interval() { return interval_; }
  static uint16_t interval(uint8_t n) { return intervals_[n]; }

  static void Update(uint16_t bpm, uint8_t bpm_10th, int16_t swing);

  static void Update(uint16_t bpm, int16_t swing) {
    Update(bpm, 0, swing);
  }

  static void Update(uint16_t bpm) {
    Update(bpm, 0, kNoSwing);
  }

  static ClockCallback set_callback(ClockCallback callback);

 private:
  static uint16_t tick_count_;
  static uint16_t tick_count_limit_;
  static uint16_t intervals_[2];
  static uint16_t interval_;
  static uint8_t off_beat_;
};

extern Clock clock;

}  // namespace midigrids

#endif // MIDIGRIDS_CLOCK_H_
