// Copyright 2018 Peter Kvitek.
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
//
// -----------------------------------------------------------------------------
//
// Device model declaration.

#ifndef MIDIGRIDS_DEV_H_
#define MIDIGRIDS_DEV_H_

#include "avrlib/base.h"

#include "midigrids/midigrids.h"
#include "midigrids/midi_out.h"
#include "midigrids/note_stack.h"
#include "midigrids/trk.h"

namespace midigrids {

typedef uint8_t (*NoteCallback)(uint8_t channel, uint8_t note, uint8_t velocity);
typedef uint8_t (*ControlChangeCallback)(uint8_t channel, uint8_t controller, uint8_t value);
typedef void (*RunningCallback)(uint8_t running);
typedef void (*TickCallback)();

class Dev {
 public:
  Dev() {}

  static void Init();

  // MIDI event handlers
  static void OnNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
  static void OnNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
  static void OnAftertouch(uint8_t channel, uint8_t note, uint8_t velocity);
  static void OnAftertouch(uint8_t channel, uint8_t velocity);
  static void OnControlChange(uint8_t channel, uint8_t controller, uint8_t value);
  static void OnProgramChange(uint8_t channel, uint8_t program);
  static void OnPitchBend(uint8_t channel, uint16_t pitch_bend);
  static void OnSysExByte(uint8_t sysex_byte);
  static void OnClock();
  static void OnStart();
  static void OnContinue();
  static void OnStop();

  static uint8_t CheckChannel(uint8_t channel);

  static void OnRawByte(uint8_t byte);

  static void OnRawMidiData(
    uint8_t status,
    uint8_t* data,
    uint8_t data_size,
    uint8_t accepted_channel);

  static void ClockCallback();

  // Device parameter accessors
  static void name(uint8_t* name) { memcpy(name, name_, kNameLength); }
  static void set_name(const uint8_t* name) { memcpy(name_, name, kNameLength); }

  #define DEFINE_ACCESSORS_(type, name) \
    static type name() { return name##_; } \
    static void set_##name(type name) { name##_ = name; }

  #define DEFINE_ACCESSORS2(type, name, call) \
    static type name() { return name##_; } \
    static void set_##name(type name) { name##_ = name; call(); }

  DEFINE_ACCESSORS_(uint8_t, slot)
  DEFINE_ACCESSORS_(uint8_t, channel)
  DEFINE_ACCESSORS2(uint8_t, tempo, UpdateClock)
  DEFINE_ACCESSORS2(int16_t, swing, UpdateClock)
  DEFINE_ACCESSORS2(uint8_t, step_length, UpdateClockPrescaler)
  DEFINE_ACCESSORS_(uint8_t, clock_mode)
  DEFINE_ACCESSORS_(uint8_t, clock_output)
  DEFINE_ACCESSORS_(uint8_t, pattern)
  DEFINE_ACCESSORS_(uint8_t, map_x)
  DEFINE_ACCESSORS_(uint8_t, map_y)
  DEFINE_ACCESSORS_(uint8_t, map_chaos)
  DEFINE_ACCESSORS_(uint8_t, accent)
  DEFINE_ACCESSORS2(uint8_t, strobe_width, UpdateStrobeWidth)

  #undef DEFINE_ACCESSORS_
  #undef DEFINE_ACCESSORS2

  #define DEFINE_TRK_ACCESSORS(type, name) \
    static type trk_##name(uint8_t trk) { return trk_[trk].name(); } \
    static void set_trk_##name(uint8_t trk, type name) { trk_[trk].set_##name(name); }

  DEFINE_TRK_ACCESSORS(uint8_t, fill)
  DEFINE_TRK_ACCESSORS(uint8_t, size)
  DEFINE_TRK_ACCESSORS(uint8_t, note)
  DEFINE_TRK_ACCESSORS(uint8_t, velocity)
  DEFINE_TRK_ACCESSORS(uint8_t, flags)
  DEFINE_TRK_ACCESSORS(uint8_t, muted)

  #undef DEFINE_TRK_ACCESSORS

  static uint8_t map_xy(uint8_t y) { return y ? map_y_ : map_x_; }
  static void set_map_xy(uint8_t y, uint8_t xy) { (y ? map_y_ : map_x_) = xy; }

  static uint8_t tick() { return tick_; }
  static uint8_t running() { return running_; }
  static uint8_t state() { return prev_state_; }

  static uint8_t InternalClock() { return !ExternalClock(); }
  static uint8_t ExternalClock() { return clock_mode_ == CLOCK_MODE_EXTERNAL; }
  static uint8_t SendOutputClock() { return clock_output_ != CLOCK_OUTPUT_NEVER; }
  static uint8_t ContiguousClock() { return clock_output_ == CLOCK_OUTPUT_ALWAYS; }

  static uint8_t GridsMode() { return pattern_ == PATTERN_GRIDS; }
  static uint8_t EuclideanMode() { return pattern_ == PATTERN_EUCLIDEAN; }

  // Callbacks
  #define DEFINE_CALLBACK(type, name) \
    type set_##name(type callback) { \
      type prev = name##_; \
      name##_ = callback; \
      return prev; \
    }

  DEFINE_CALLBACK(NoteCallback, note_callback)
  DEFINE_CALLBACK(ControlChangeCallback, control_change_callback)
  DEFINE_CALLBACK(RunningCallback, running_callback)
  DEFINE_CALLBACK(TickCallback, tick_callback)

  #undef DEFINE_CALLBACK

  // Note stack accessors
  static uint8_t note_stack_size() { return note_stack_.size(); }
  static const NoteEntry& note_stack_at(uint8_t index) {
    return note_stack_.played_note(index);
  }

  // Output helpers
  static void SendNow(uint8_t byte) {
    midi_out.Send(byte);
  }

  static void Send(uint8_t status, uint8_t* data, uint8_t size);
  static void Send2(uint8_t a, uint8_t b);
  static void Send3(uint8_t a, uint8_t b, uint8_t c);

  static void FlushOutputBuffer(uint8_t size);

  static void SendLater(uint8_t note, uint8_t velocity, uint8_t when, uint8_t channel);
  static void SendScheduledNotes();
  static void FlushQueue();

  // Operations
  static void InitDev();
  static void InitSlots();
  static void InitSlot();
  static void LoadSlot(uint8_t slot);
  static void SaveSlot(uint8_t slot);
  static void SaveSlot() {
    SaveSlot(slot_);
  }

  static void ToggleRun();
  static void Start();
  static void Stop();

  static void SilenceAllNotes();

 private:
  static void Tick();

  static void SendTrkNote(uint8_t trk, uint8_t accent);
  static void SendNote(uint8_t note, uint8_t velocity);

  static void UpdateClock();
  static void UpdateClockPrescaler();
  static void UpdateStrobeWidth();

  static uint8_t HandleCC(uint8_t controller, uint8_t value);

  // Persistent data
  static uint8_t slot_;
  static uint8_t name_[kNameLength];
  static uint8_t channel_;
  static uint8_t tempo_;
  static int16_t swing_;
  static uint8_t step_length_;
  static uint8_t clock_mode_;
  static uint8_t clock_output_;
  static uint8_t pattern_;
  static uint8_t map_x_;
  static uint8_t map_y_;
  static uint8_t map_chaos_;
  static uint8_t accent_;
  static uint8_t strobe_width_;

  static Trk trk_[kNumTrk];

  // Volatile data
  static uint8_t tick_;
  static uint8_t running_;
  static uint8_t clock_prescaler_;
  static uint8_t prev_state_;

  static NoteStack<16> note_stack_;

  static NoteCallback note_callback_;
  static ControlChangeCallback control_change_callback_;
  static RunningCallback running_callback_;
  static TickCallback tick_callback_;

  DISALLOW_COPY_AND_ASSIGN(Dev);
};

extern Dev dev;

}  // namespace midigrids

#endif // MIDIGRIDS_DEV_H_
