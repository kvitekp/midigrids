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
// Device model implementation.

#include "avrlib/serial.h"
#include "avrlib/string.h"
#include "avrlib/random.h"
#include "avrlib/op.h"

#include "midigrids/midigrids.h"
#include "midigrids/event_scheduler.h"
#include "midigrids/pattern_generator.h"
#include "midigrids/midi_handler.h"
#include "midigrids/output_ports.h"
#include "midigrids/clock.h"
#include "midigrids/eemem.h"
#include "midigrids/slot.h"
#include "midigrids/leds.h"
#include "midigrids/dev.h"
#include "midigrids/ui.h"

namespace midigrids {

using namespace avrlib;

/* extern */
Dev dev;

/* <static> */
uint8_t Dev::slot_;
uint8_t Dev::name_[kNameLength];
uint8_t Dev::channel_ = kDefChannel;
uint8_t Dev::tempo_ = kDefTempo;
int16_t Dev::swing_ = kDefSwing;
uint8_t Dev::step_length_ = kDefStepLength;
uint8_t Dev::clock_mode_ = kDefClockMode;
uint8_t Dev::clock_output_ = kDefClockOut;
uint8_t Dev::pattern_ = kDefPattern;
uint8_t Dev::map_x_ = kDefMapXY;
uint8_t Dev::map_y_ = kDefMapXY;
uint8_t Dev::map_chaos_ = kDefMapChaos;
uint8_t Dev::accent_ = kDefAccent;
uint8_t Dev::strobe_width_ = kDefStrobeWidth;

Trk Dev::trk_[kNumTrk];

uint8_t Dev::tick_;
uint8_t Dev::running_;
uint8_t Dev::clock_prescaler_;
uint8_t Dev::prev_state_;

NoteStack<16> Dev::note_stack_;

NoteCallback Dev::note_callback_;
ControlChangeCallback Dev::control_change_callback_;
RunningCallback Dev::running_callback_;
TickCallback Dev::tick_callback_;
/* </static> */

///////////////////////////////////////////////////////////////////////////////
// Device initialization

/* static */
void Dev::Init() {
  clock.set_callback(ClockCallback);
  clock.Start();

  LoadSlot(eemem.slot());
}

///////////////////////////////////////////////////////////////////////////////
// MIDI event handlers

/* static */
void Dev::OnNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
  if (note_callback_ && note_callback_(channel, note, velocity))
    return;

  if (channel != channel_)
    return;

  note_stack_.NoteOn(note, velocity);

  // Forward note on  event
  Send3(0x90 | channel_, note, velocity);
}

/* static */
void Dev::OnNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
  if (note_callback_ && note_callback_(channel, note, 0))
    return;

  if (channel != channel_)
    return;

  note_stack_.NoteOff(note);

  // Forward note off event
  Send3(0x80 | channel_, note, velocity);
}

/* static */
void Dev::OnAftertouch(uint8_t channel, uint8_t note, uint8_t velocity) {
  if (channel != channel_)
    return;

  // Forward after touch event
  Send3(0xa0 | channel, note, velocity);
}

/* static */
void Dev::OnAftertouch(uint8_t channel, uint8_t velocity) {
  if (channel != channel_)
    return;

  // Forward after touch event
  Send2(0xd0 | channel, velocity);
}

/* static */
void Dev::OnControlChange(uint8_t channel, uint8_t controller, uint8_t value) {
  if (control_change_callback_ && control_change_callback_(channel, controller, value))
    return;

  if (channel != channel_)
    return;

  if (HandleCC(controller, value))
    return;

  // Forward cc event
  Send3(0xb0 | channel, controller, value);
}

/* static */
void Dev::OnProgramChange(uint8_t channel, uint8_t program) {
  if (channel != channel_)
    return;

  // Forward program change event
  Send2(0xc0 | channel, program);
}

/* static */
void Dev::OnPitchBend(uint8_t channel, uint16_t pitch_bend) {
  if (channel != channel_)
    return;

  // Forward pitch bend event
  Send3(0xe0 | channel, static_cast<uint8_t>(pitch_bend & 0x7f), static_cast<uint8_t>(pitch_bend >> 7));
}

/* static */
void Dev::OnSysExByte(uint8_t sysex_byte) {
  SendNow(sysex_byte);
}

/* static */
void Dev::OnClock() {
  if (ExternalClock() && running_) {
    Tick();
  }
}

/* static */
void Dev::OnStart() {
  if (ExternalClock()) {
    Start();
  }
}

/* static */
void Dev::OnContinue() {
  if (ExternalClock()) {
    running_ = 1;
  }
}

/* static */
void Dev::OnStop() {
  if (ExternalClock()) {
    Stop();
  }
}

/* static */
uint8_t Dev::CheckChannel(uint8_t channel) {
  return 1;
}

/* static */
void Dev::OnRawByte(uint8_t byte) {
}

/* static */
void Dev::OnRawMidiData(
  uint8_t status,
  uint8_t* data,
  uint8_t data_size,
  uint8_t accepted_channel) {
  // Filter MIDI Clock related events
  switch (status) {
    case 0xf2: // SPP
    case 0xf8: // Clock
    case 0xfa: // Start
    case 0xfb: // Continue
    case 0xfc: // Stop
      // Don't forward clock related events unless configured for external clock and
      // clock output is enabled.
      if (!(ExternalClock() && SendOutputClock()))
        return;
      // Don't forward clock events unless configured appropriately
      if (status == 0xf8 && !(ContiguousClock() || running_))
        return;
  }

  // Forward all other channels
  if ((status & 0xf) != channel_) {
    Send(status, data, data_size);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Device actions

/* static */
void Dev::InitDev() {
  eemem.Reset();
  InitSlots();
  LoadSlot(0);
}

/* static */
void Dev::InitSlots() {
  Slot slot; slot.Init();
  for (uint8_t n = 0; n < kNumSlots; n++) {
    slot.Write(n);
  }
}

/* static */
void Dev::InitSlot() {
  Slot slot; slot.Init();
  slot.Write(slot_);
  LoadSlot(slot_);
}

/* static */
void Dev::LoadSlot(uint8_t slot_numb) {
  set_slot(slot_numb);

  Slot slot; slot.Read(slot_numb);

  set_name(slot.name_);
  set_channel(slot.channel_);
  set_tempo(slot.tempo_);
  set_swing(slot.swing_);
  set_step_length(slot.step_length_);
  set_clock_mode(slot.clock_mode_);
  set_clock_output(slot.clock_output_);
  set_pattern(slot.pattern_);
  set_map_x(slot.map_x_);
  set_map_y(slot.map_y_);
  set_map_chaos(slot.map_chaos_);
  set_accent(slot.accent_);
  set_strobe_width(slot.strobe_width_);

  memcpy(trk_, slot.trk_, sizeof(trk_));
}

/* static */
void Dev::SaveSlot(uint8_t slot_numb) {
  set_slot(slot_numb);

  Slot slot;

  memcpy(slot.name_, name_, kNameLength);
  slot.channel_ = channel();
  slot.tempo_ = tempo();
  slot.swing_ = swing();
  slot.step_length_ = step_length();
  slot.clock_mode_ = clock_mode();
  slot.clock_output_ = clock_output();
  slot.pattern_ = pattern();
  slot.map_x_ = map_x();
  slot.map_y_ = map_y();
  slot.map_chaos_ = map_chaos();
  slot.accent_ = accent();
  slot.strobe_width_ = strobe_width();

  memcpy(slot.trk_, trk_, sizeof(trk_));

  slot.Write(slot_numb);
}

///////////////////////////////////////////////////////////////////////////////
// Clock related routines

/* static */
void Dev::ToggleRun() {
  if (!running_) {
    Start();
  } else {
    Stop();
  }
}

/* static */
void Dev::Start() {
  if (running_)
    return;

  if (InternalClock()) {
    clock.Start();
    if (SendOutputClock()) {
      SendNow(0xfa);
    }
  }

  UpdateClockPrescaler();

  pattern_generator.Reset();
  tick_ = clock_prescaler_ - 1;
  running_ = 1;
  prev_state_ = 0;

  if (running_callback_) {
    running_callback_(1);
  }
}

/* static */
void Dev::Stop() {
  if (!running_)
    return;

  FlushQueue();

  if (InternalClock() && SendOutputClock()) {
    SendNow(0xfc);
  }

  running_ = 0;
  prev_state_ = 0;

  if (running_callback_) {
    running_callback_(0);
  }
}

/* static */
void Dev::ClockCallback() {
  if (InternalClock()) {
    if (SendOutputClock()) {
      if (running_ || ContiguousClock()) {
        SendNow(0xf8);
      }
    }
    if (running_) {
      Tick();
    } else
      SendScheduledNotes();
  } else {
    // Make all notes get their note offs served
    if (!running_) {
      SendScheduledNotes();
    }
  }
}

/* static */
void Dev::Tick() {
  SendScheduledNotes();

  if (!running_)
    return;

  if (tick_callback_) {
    tick_callback_();
  }

  pattern_generator.Tick();

  if (++tick_ < clock_prescaler_)
    return;

  tick_ = 0;

  uint8_t state = pattern_generator.state();
  if (state != prev_state_) {
    uint8_t mask = 1;
    for (uint8_t n = 0; n < kNumTrk; n++, mask<<= 1) {
      if ((state & mask) && (prev_state_ & mask) == 0 && !trk_muted(n)) {
        uint8_t accent = state & (mask << 4);
        SendTrkNote(n, accent);
      }
    }
    prev_state_ = state;
  }
}

/* static */
void Dev::UpdateClock() {
  clock.Update(tempo_, swing_);
}

/* static */
void Dev::UpdateClockPrescaler() {
  clock_prescaler_ = ResourcesManager::Lookup<uint8_t, uint8_t>(
      midi_clock_ticks_per_note, step_length_);
  clock.set_tick_count_limit(clock_prescaler_);
  pattern_generator.set_ticks_per_step(clock_prescaler_);
}

/* static */
void Dev::UpdateStrobeWidth() {
  output_ports.SetStrobeWidth(strobe_width_);
}

///////////////////////////////////////////////////////////////////////////////
// Output helpers

/* static */
void Dev::SendTrkNote(uint8_t trk, uint8_t accent) {
  output_ports.SetStrobe(trk);

  uint8_t note = trk_note(trk);
  uint8_t velocity = trk_velocity(trk);
  if (accent && GridsMode()) {
    velocity = Clip(velocity + accent_, 0, 127);
  }

  SendNote(note, velocity);
}

/* static */
void Dev::SendNote(uint8_t note, uint8_t velocity) {
  if (!velocity) velocity = 1;

  // Check if we have note off scheduled for this note and if so,
  // remove it from the queue and send note off event now
  if (event_scheduler.Remove(note, 0, channel_)) {
    Send3(0x80 | channel_, note, 0);
  }

  // Send note on
  Send3(0x90 | channel_, note, velocity);

  if (note_callback_) {
    note_callback_(channel_, note, velocity);
  }

  // Calculate step notes length
  uint8_t length = clock_prescaler_ >> 1;
  if (!length) length = 1;

  // Schedule note off
  SendLater(note, 0, length - 1, channel_);
}

/* static */
void Dev::Send2(uint8_t a, uint8_t b) {
  FlushOutputBuffer(2);
  MidiHandler::OutputBuffer::Write(a);
  MidiHandler::OutputBuffer::Write(b);
}

/* static */
void Dev::Send3(uint8_t a, uint8_t b, uint8_t c) {
  FlushOutputBuffer(3);
  MidiHandler::OutputBuffer::Write(a);
  MidiHandler::OutputBuffer::Write(b);
  MidiHandler::OutputBuffer::Write(c);
}

/* static */
void Dev::Send(uint8_t status, uint8_t* data, uint8_t size) {
  FlushOutputBuffer(1 + size);
  MidiHandler::OutputBuffer::Write(status);
  while (size--) {
    MidiHandler::OutputBuffer::Write(*data++);
  }
}

/* static */
void Dev::FlushOutputBuffer(uint8_t requested_size) {
  while (MidiHandler::OutputBuffer::writable() < requested_size) {
    SendNow(MidiHandler::OutputBuffer::Read());
  }
}

/* static */
void Dev::SendLater(uint8_t note, uint8_t velocity, uint8_t when, uint8_t channel) {
  event_scheduler.Schedule(note, velocity, when, channel);
}

/* static */
void Dev::SendScheduledNotes() {
  if (event_scheduler.size() == 0)
    return;

  uint8_t current = event_scheduler.root();
  while (current) {
    const SchedulerEntry& entry = event_scheduler.entry(current);
    if (entry.when) {
      break;
    }
    if (entry.note != kZombieSlot) {
      if (entry.velocity) {
        Send3(0x90 | entry.channel, entry.note, entry.velocity);
      } else {
        Send3(0x80 | entry.channel, entry.note, 0);
      }
    }
    current = entry.next;
  }
  event_scheduler.Tick();
}

/* static */
void Dev::FlushQueue() {
  while (event_scheduler.size()) {
    SendScheduledNotes();
  }
}

/* static */
void Dev::SilenceAllNotes() {
  Send3(0xb0 | channel_, 120, 0);
  Send3(0xb0 | channel_, 123, 0);
}

/* static */
uint8_t Dev::HandleCC(uint8_t controller, uint8_t value) {
  switch (controller) {
    case kCCTrk1Fill:
    case kCCTrk2Fill:
    case kCCTrk3Fill:
    case kCCTrk4Fill:
      ui.EnsurePage(static_cast<UiPageIndex>
          (static_cast<uint8_t>(PAGE_TRK1_FILL) + controller - kCCTrk1Fill));
      set_trk_fill(controller - kCCTrk1Fill, static_cast<uint8_t>(
          ui.Scale(value, 0, 0x7f, kMinTrkFill, kMaxTrkFill)));
      break;

    case kCCMapX:
      ui.EnsurePage(PAGE_MAP_X);
      set_map_x(static_cast<uint8_t>(
          ui.Scale(value, 0, 0x7f, kMinMapXY, kMaxMapXY)));
      break;
    case kCCMapY:
      ui.EnsurePage(PAGE_MAP_Y);
      set_map_y(static_cast<uint8_t>(
          ui.Scale(value, 0, 0x7f, kMinMapXY, kMaxMapXY)));
      break;

    case kCCMapChaos:
      ui.EnsurePage(PAGE_MAP_CHAOS);
      set_map_chaos(static_cast<uint8_t>(
          ui.Scale(value, 0, 0x7f, kMinMapChaos, kMaxMapChaos)));
      break;

    case kCCAccent:
      ui.EnsurePage(PAGE_ACCENT);
      set_accent(static_cast<uint8_t>(
          ui.Scale(value, 0, 0x7f, kMinAccent, kMaxAccent)));
      break;

    default:
      return 0;
  }

  ui.RequestRedraw();

  return 1;
}

}  // namespace midigrids
