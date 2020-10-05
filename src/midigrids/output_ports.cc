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

#include "midigrids/output_ports.h"
#include "midigrids/slot.h"

namespace midigrids {

using namespace avrlib;

/* extern */
OutputPorts output_ports;

/* <static> */
uint8_t OutputPorts::value_;
uint8_t OutputPorts::latched_value_ = 0xff; // ensure initial write
uint8_t OutputPorts::strobe_count_[kNumOutputPorts];
uint8_t OutputPorts::strobe_ticks_;
uint8_t OutputPorts::strobe_mask_;
uint8_t OutputPorts::invert_mask_;
/* </static> */

const prog_uint8_t strobe_ticks_from_width[] PROGMEM = {
  5,  // 1ms
  10, // 2ms
  15, // 3ms
  20, // 4ms
  25, // 5ms
};

#ifdef ENABLE_ZCLOCK_QUIRKS
const prog_uint8_t reverse_nibble[16] PROGMEM = {
  0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
  0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf,
};
#endif

/* static */
void OutputPorts::Init() {
  ports_.set_mode(DIGITAL_OUTPUT);
  Write(0);
  SetStrobeWidth(0);
}

/* static */
void OutputPorts::Write(uint8_t value) {
  if (value == latched_value_)
    return;

#ifdef ENABLE_ZCLOCK_QUIRKS
  // zCLOCK module has output ports reversed, so we need
  // to compensate for that to preserve UI consistency.
  ports_.Write(ResourcesManager::Lookup<uint8_t, uint8_t>(
      reverse_nibble, value ^ invert_mask_));
#else
  ports_.Write(value ^ invert_mask_);
#endif
  latched_value_ = value;
}

/* static */
void OutputPorts::Tick() {
  if (value_ != latched_value_) {
    Write(value_);
  }

  // Update strobe state
  if (strobe_mask_) {
    uint8_t mask = 1;
    for (uint8_t n = 0; n < kNumOutputPorts; n++, mask<<= 1) {
      if (strobe_mask_ & mask) {
        if (!strobe_count_[n] || --strobe_count_[n] == 0) {
          CLRFLAG(strobe_mask_, mask);
          CLRFLAG(value_, mask);
        }
      }
    }
  }
}

/* static */
void OutputPorts::SetGate(uint8_t port, uint8_t value) {
  uint8_t mask = 1 << port;
  SETFLAGTO(value_, mask, value);
  Write(value_);
}

/* static */
void OutputPorts::SetStrobe(uint8_t port) {
  uint8_t mask = 1 << port;
  SETFLAG(value_, mask);
  SETFLAG(strobe_mask_, mask);
  strobe_count_[port] = strobe_ticks_;
  Write(value_);
}

/* static */
void OutputPorts::SetStrobeWidth(uint8_t width) {
  if (width < numbof(strobe_ticks_from_width)) {
    strobe_ticks_ = ResourcesManager::Lookup<uint8_t, uint8_t>(
        strobe_ticks_from_width, width);
  }
}

/* static */
uint8_t OutputPorts::GetStrobeWidth() {
  for (uint8_t n = 0; n < numbof(strobe_ticks_from_width); n++) {
    if (pgm_read_byte(&strobe_ticks_from_width[n]) == strobe_ticks_)
      return n;
  }
  return kDefStrobeWidth;
}

}  // namespace midigrids                           `
