// Copyright 2017 Peter Kvitek.
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

#include "avrlib/gpio.h"
#include "avrlib/boot.h"
#include "avrlib/time.h"
#include "avrlib/timer.h"
#include "avrlib/serial.h"
#include "avrlib/watchdog_timer.h"

#include "midi/midi.h"

#include "midigrids/midigrids.h"
#include "midigrids/event_scheduler.h"
#include "midigrids/pattern_generator.h"
#include "midigrids/output_ports.h"
#include "midigrids/midi_handler.h"
#include "midigrids/midi_out.h"
#include "midigrids/display.h"
#include "midigrids/storage.h"
#include "midigrids/clock.h"
#include "midigrids/eemem.h"
#include "midigrids/leds.h"
#include "midigrids/dev.h"
#include "midigrids/ui.h"

#include <util/delay.h>

using namespace avrlib;
using namespace midi;
using namespace midigrids;

Serial<MidiPort, 31250, POLLED, POLLED> midi_io;
MidiStreamParser<MidiHandler> midi_parser;

inline void PollMidiIn() {
  // Receive midi input
  if (midi_io.readable()) {
    uint8_t byte = midi_io.ImmediateRead();
    if (byte != 0xfe) {
      if (byte != 0xf8) leds.FlashMidiIn();
      midi_parser.PushByte(byte);
    }
  }
}

inline void SendMidiOut() {
  // Send midi output
  if (MidiHandler::OutputBuffer::readable() && midi_io.writable()) {
    uint8_t byte = MidiHandler::OutputBuffer::ImmediateRead();
    if (byte != 0xf8) leds.FlashMidiOut();
    midi_io.Overwrite(byte);
  }
}

ISR(TIMER2_OVF_vect, ISR_NOBLOCK) {
  // 4.9KHz
  PollMidiIn();
  SendMidiOut();
  output_ports.Tick();

  // Handle low priority tasks
  static uint8_t sub_clock;
  ++sub_clock;
  if ((sub_clock & 1) == 0) {
    // 2.45KHz
    ui.Poll();
    if ((sub_clock & 3) == 0) {
      // 1.225KHz
      TickSystemClock();
      leds.Tick();
      if ((sub_clock & 7) == 0) {
        // 306Hz
        display.BlinkCursor();
      }
    }
  }
}

void Init() {
  sei();
  UCSR0B = 0;

  event_scheduler.Init();

  // System clock @4.9KHz
  Timer<2>::set_prescaler(2);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::Start();

  display.Init();
  storage.Init();
  midi_io.Init();
  midi_out.Init();
  pattern_generator.Init();
  output_ports.Init();
  clock.Init();
  eemem.Init();
  leds.Init();
  dev.Init();
  ui.Init();
}

int main(void) {
  ResetWatchdog();
  Init();

  while (1) {
    ui.DoEvents();
  }
}
