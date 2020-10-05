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

#include "midigrids/midi_out.h"
#include "midigrids/leds.h"

namespace midigrids {

using namespace avrlib;

/* extern */
MidiOut midi_out;

/* static */
static Serial<MidiPort, 31250, DISABLED, POLLED> _midi_out;

/* static */
void MidiOut::Init() {
  _midi_out.Init();
}

/* static */
void MidiOut::Send(uint8_t byte) {
  _midi_out.Write(byte);
  leds.FlashMidiOut();
}

}  // namespace midigrids                           `
