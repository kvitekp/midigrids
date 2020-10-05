// Copyright 2011 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
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

#ifndef MIDIGRIDS_MIDI_HANDLER_H_
#define MIDIGRIDS_MIDI_HANDLER_H_

#include "midi/midi.h"
#include "midigrids/dev.h"

namespace midigrids {

struct MidiHandler : public midi::MidiDevice {

  enum {
    buffer_size = 128,
    data_size = 8,
  };
  typedef avrlib::RingBuffer<MidiHandler> OutputBuffer;
  typedef avrlib::DataTypeForSize<data_size>::Type Value;

  static void NoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    dev.OnNoteOn(channel, note, velocity);
  }

  static void NoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    dev.OnNoteOff(channel, note, velocity);
  }

  static void Aftertouch(uint8_t channel, uint8_t note, uint8_t velocity) {
    dev.OnAftertouch(channel, note, velocity);
  }

  static void Aftertouch(uint8_t channel, uint8_t velocity) {
    dev.OnAftertouch(channel, velocity);
  }

  static void ControlChange(uint8_t channel, uint8_t controller, uint8_t value) {
    dev.OnControlChange(channel, controller, value);
  }

  static void ProgramChange(uint8_t channel, uint8_t program) {
    dev.OnProgramChange(channel, program);
  }

  static void PitchBend(uint8_t channel, uint16_t pitch_bend) {
    dev.OnPitchBend(channel, pitch_bend);
  }

  static void SysExStart() {
    dev.OnSysExByte(0xf0);
  }

  static void SysExByte(uint8_t sysex_byte) {
    dev.OnSysExByte(sysex_byte);
  }

  static void SysExEnd() {
    dev.OnSysExByte(0xf7);
  }

  static void BozoByte(uint8_t bozo_byte) {
  }

  static void Clock() {
    dev.OnClock();
  }

  static void Start() {
    dev.OnStart();
  }

  static void Continue() {
    dev.OnContinue();
  }

  static void Stop() {
    dev.OnStop();
  }

  static uint8_t CheckChannel(uint8_t channel) {
    return 1; // dev.CheckChannel(channel);
  }

  static void RawByte(uint8_t byte) {
    dev.OnRawByte(byte);
  }

  static void RawMidiData(
      uint8_t status,
      uint8_t* data,
      uint8_t data_size,
      uint8_t accepted_channel) {
    dev.OnRawMidiData(
        status,
        data,
        data_size,
        accepted_channel);
  }
};

}  // namespace midigrids

#endif // MIDIGRIDS_MIDI_HANDLER_H_
