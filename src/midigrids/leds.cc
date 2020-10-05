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

#include "midigrids/leds.h"

namespace midigrids {

using namespace avrlib;

/* extern */
Leds leds;

static FlashLed< MidiInLed, 2> midiInLed;
static FlashLed< MidiOutLed, 2> midiOutLed;

/* static */
void Leds::Init() {
  midiInLed.Init();
  midiOutLed.Init();
}

/* static */
void Leds::Tick() {
  midiInLed.Tick();
  midiOutLed.Tick();
}

/* static */
void Leds::FlashMidiIn() {
  midiInLed.On();
}

/* static */
void Leds::FlashMidiOut() {
  midiOutLed.On();
}

}  // namespace midigrids
