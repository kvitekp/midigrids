// Copyright 2011 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Modified for MidiGrids by Peter Kvitek (pete@kvitek.com)
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
// Pattern generator.
//
// OUTPUT MODE  BIT7  BIT6  BIT5  BIT4  BIT3  BIT2  BIT1  BIT0
// DRUMS        HHAC  TMAC  SDAC  BDAC    HH    TM    SD    BD
// EUCLIDEAN    RST4  RST3  RST2  RST1  EUC4  EUC3  EUC2  EUC1

#ifndef MIDIGRIDS_PATTERN_GENERATOR_H_
#define MIDIGRIDS_PATTERN_GENERATOR_H_

#include <string.h>

#include "avrlib/base.h"
#include "avrlib/random.h"

#include "midigrids/midigrids.h"

namespace midigrids {

const uint8_t kStepsPerPattern = 32;

class PatternGenerator {
 public:
  PatternGenerator() { }
  ~PatternGenerator() { }

  static void Init() {
    Reset();
  }

  static void Reset() {
    step_ = 0;
    tick_ = 0;
    memset(euclidean_step_, 0, sizeof(euclidean_step_));
  }

  static void Retrigger() {
    Evaluate();
  }

  static void Tick();

  static uint8_t state() { return state_; }
  static uint8_t step() { return step_; }

  static uint8_t ticks_per_step() { return ticks_per_step_; }
  static void set_ticks_per_step(uint8_t ticks_per_step) {
    ticks_per_step_ = ticks_per_step;
  }

  static bool on_first_beat() { return first_beat_; }
  static bool on_beat() { return beat_; }

 private:
  static void Evaluate();
  static void EvaluateEuclidean();
  static void EvaluateGrids();

  static uint8_t ReadDrumMap(
      uint8_t step,
      uint8_t instrument,
      uint8_t x,
      uint8_t y);

  static uint8_t ticks_per_step_;

  static uint8_t tick_;
  static uint8_t step_;
  static uint8_t euclidean_step_[kNumTrk];
  static bool first_beat_;
  static bool beat_;

  static uint8_t state_;
  static uint8_t part_perturbation_[kNumTrk];

  DISALLOW_COPY_AND_ASSIGN(PatternGenerator);
};

extern PatternGenerator pattern_generator;

}  // namespace midigrids

#endif // MIDIGRIDS_PATTERN_GENERATOR_H_
