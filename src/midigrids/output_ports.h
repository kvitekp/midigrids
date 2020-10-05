// Copyright 2015 Peter Kvitek.
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

#ifndef MIDIGRIDS_OUTPUT_PORTS_H_
#define MIDIGRIDS_OUTPUT_PORTS_H_

#include "midigrids/midigrids.h"

namespace midigrids {

static const uint8_t kNumOutputPorts = 4;

class OutputPorts {
 public:
  OutputPorts() {}
  static void Init();
  static void Tick();

  static void SetGate(uint8_t port, uint8_t value);
  static void SetStrobe(uint8_t port);

  static void SetStrobeWidth(uint8_t width);
  static uint8_t GetStrobeWidth();

  static uint8_t invert_mask() { return invert_mask_; }
  static void set_invert_mask_(uint8_t mask) { invert_mask_ = mask; }

 private:
  static uint8_t value_;
  static uint8_t latched_value_;
  static uint8_t strobe_count_[kNumOutputPorts];
  static uint8_t strobe_ticks_;
  static uint8_t strobe_mask_;
  static uint8_t invert_mask_;

  static OutputPortsPorts ports_;

  static void Write(uint8_t value);

  DISALLOW_COPY_AND_ASSIGN(OutputPorts);
};

extern OutputPorts output_ports;

}  // namespace midigrids

#endif  // MIDIGRIDS_OUTPUT_PORTS_H_
