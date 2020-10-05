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

#include "avrlib/timer.h"

#include "midigrids/clock.h"
#include "midigrids/resources.h"

using namespace avrlib;

namespace midigrids {

Clock clock;

/* static */
static ClockCallback _callback;

/* <static> */
uint16_t Clock::tick_count_;
uint16_t Clock::tick_count_limit_;
uint16_t Clock::intervals_[2];
uint16_t Clock::interval_;
uint8_t Clock::off_beat_;
/* </static> */

/* static */
void Clock::Init() {
  Update(120);

  tick_count_limit_ = kNumTicksPer8thNote;
  tick_count_ = -1;
  off_beat_ = 0;

  Timer<1>::set_prescaler(1);
  Timer<1>::set_mode(0, _BV(WGM12), 3);
  PwmChannel1A::set_frequency(6510);
  Timer<1>::StartCompare();
}

/* static */
void Clock::Start() {
  tick_count_ = -1;
  off_beat_ = 0;
  interval_ = intervals_[0];
}

/* static */
uint16_t Clock::Tick() {
  if (++tick_count_ >= tick_count_limit_) {
    tick_count_ = 0;
    off_beat_ = off_beat_ ? 0 : 1;
  }
  interval_ = intervals_[off_beat_];
  return interval_;
}

/* static */
void Clock::Update(uint16_t bpm, uint8_t bpm_tenth, int16_t swing) {
  int32_t base_tick_duration = 7812500 / (static_cast<uint32_t>(bpm) * 10 + bpm_tenth) - 1;

  int16_t swing_amount = swing - kNoSwing;
  if (swing_amount) {
    int32_t swing_delta = base_tick_duration * swing_amount / kNoSwing;
    intervals_[0] = base_tick_duration - swing_delta;
    intervals_[1] = base_tick_duration + swing_delta;
  } else {
    intervals_[0] = base_tick_duration;
    intervals_[1] = base_tick_duration;
  }
}

/* static */
ClockCallback Clock::set_callback(ClockCallback callback) {
  DisableInterrupts disableInterrupts;
  ClockCallback prev = _callback;
  _callback = callback;
  return prev;
}

/* global */
ISR(TIMER1_COMPA_vect) {
  PwmChannel1A::set_frequency(clock.Tick());
  if (_callback) {
    _callback();
  }
}

}  // namespace midigrids
