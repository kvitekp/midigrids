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

#include "midigrids/pattern_generator.h"

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "avrlib/op.h"

#include "midigrids/dev.h"

namespace midigrids {

using namespace avrlib;

/* extern */
PatternGenerator pattern_generator;

/* static */
uint8_t PatternGenerator::ticks_per_step_;
uint8_t PatternGenerator::tick_;
uint8_t PatternGenerator::step_;

bool PatternGenerator::first_beat_;
bool PatternGenerator::beat_;

uint8_t PatternGenerator::euclidean_step_[kNumTrk];
uint8_t PatternGenerator::part_perturbation_[kNumTrk];
uint8_t PatternGenerator::state_;

static const prog_uint8_t* drum_map[5][5] = {
  { node_10, node_8, node_0, node_9, node_11 },
  { node_15, node_7, node_13, node_12, node_6 },
  { node_18, node_14, node_4, node_5, node_3 },
  { node_23, node_16, node_21, node_1, node_2 },
  { node_24, node_19, node_17, node_20, node_22 },
};

/* static */
uint8_t PatternGenerator::ReadDrumMap(
    uint8_t step,
    uint8_t instrument,
    uint8_t x,
    uint8_t y) {
  uint8_t i = x >> 6;
  uint8_t j = y >> 6;
  const prog_uint8_t* a_map = drum_map[i][j];
  const prog_uint8_t* b_map = drum_map[i + 1][j];
  const prog_uint8_t* c_map = drum_map[i][j + 1];
  const prog_uint8_t* d_map = drum_map[i + 1][j + 1];
  uint8_t offset = (instrument * kStepsPerPattern) + step;
  uint8_t a = pgm_read_byte(a_map + offset);
  uint8_t b = pgm_read_byte(b_map + offset);
  uint8_t c = pgm_read_byte(c_map + offset);
  uint8_t d = pgm_read_byte(d_map + offset);
  return U8Mix(U8Mix(a, b, x << 2), U8Mix(c, d, x << 2), y << 2);
}

/* static */
void PatternGenerator::EvaluateGrids() {
  // At the beginning of a pattern, decide on perturbation levels.
  if (step_ == 0) {
    for (uint8_t i = 0; i < kNumTrk; ++i) {
      part_perturbation_[i] = U8U8MulShift8(Random::GetByte(), dev.map_chaos());
    }
  }

  uint8_t instrument_mask = 1;
  uint8_t x = dev.map_x();
  uint8_t y = dev.map_y();
  uint8_t accent_bits = 0;
  for (uint8_t i = 0; i < kNumTrk; ++i) {
    uint8_t level = ReadDrumMap(step_, i, x, y);
    if (level < 255 - part_perturbation_[i]) {
      level += part_perturbation_[i];
    } else {
      // The sequencer from Anushri uses a weird clipping rule here. Comment
      // this line to reproduce its behavior.
      level = 255;
    }
    uint8_t threshold = ~dev.trk_fill(i);
    if (level > threshold) {
      if (level > 192) {
        accent_bits |= instrument_mask;
      }
      state_ |= instrument_mask;
    }
    instrument_mask <<= 1;
  }

  state_ |= accent_bits << kNumTrk;
}

/* static */
void PatternGenerator::EvaluateEuclidean() {
  // Refresh only on sixteenth notes.
  if (step_ & 1) {
    return;
  }

  // Euclidean pattern generation
  uint8_t instrument_mask = 1;
  uint8_t reset_bits = 0;
  for (uint8_t i = 0; i < kNumTrk; ++i) {
    uint8_t length = (dev.trk_size(i) >> 3) + 1;
    uint8_t fill = dev.trk_fill(i) >> 3;
    uint16_t address = U8U8Mul(length - 1, 32) + fill;
    while (euclidean_step_[i] >= length) {
      euclidean_step_[i] -= length;
    }
    uint32_t step_mask = 1L << static_cast<uint32_t>(euclidean_step_[i]);
    uint32_t pattern_bits = pgm_read_dword(lut_res_euclidean + address);
    if (pattern_bits & step_mask) {
      state_ |= instrument_mask;
    }
    if (euclidean_step_[i] == 0) {
      reset_bits |= instrument_mask;
    }
    instrument_mask <<= 1;
  }

  state_ |= reset_bits << kNumTrk;
}

/* static */
void PatternGenerator::Evaluate() {
  state_ = 0;

  Random::Update();

  // Refresh only at step changes.
  //if (tick_ != 0)
  //  return;

  switch (dev.pattern()) {
    case PATTERN_GRIDS: EvaluateGrids(); break;
    case PATTERN_EUCLIDEAN: EvaluateEuclidean(); break;
  }
}

/* static */
void PatternGenerator::Tick() {
  Evaluate();
  beat_ = (step_ & 0x7) == 0;
  first_beat_ = step_ == 0;

  ++tick_;

  // Wrap into steps.
  while (tick_ >= ticks_per_step_) {
    tick_ -= ticks_per_step_;
    if (!(step_ & 1)) {
      for (uint8_t i = 0; i < kNumTrk; ++i) {
        ++euclidean_step_[i];
      }
    }
    ++step_;
  }

  // Wrap into pattern steps.
  if (step_ >= kStepsPerPattern) {
    step_ -= kStepsPerPattern;
  }
}

}  // namespace midigrids

