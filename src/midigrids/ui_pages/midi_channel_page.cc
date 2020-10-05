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
//
// -----------------------------------------------------------------------------
//
// Midi channel page class.

#include "midigrids/ui_pages/midi_channel_page.h"

namespace midigrids {

/* static */
const prog_EventHandlers MidiChannelPage::event_handlers_ PROGMEM = {
  OnInit,
  OnQuit,
  OnIncrement,
  OnClick,
  OnSwitch,
  OnIdle,
  UpdateScreen,
  UpdateLeds,
};

/* static */
void MidiChannelPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
  dev.set_note_callback(NoteCallback);
}

/* static */
void MidiChannelPage::OnQuit(UiPageIndex nextPage) {
  dev.set_note_callback(0);
}

/* static */
uint8_t MidiChannelPage::OnIncrement(uint8_t id, int8_t value) {
  if (!changing_page())
    return 0;

  dev.set_channel(Clamp(static_cast<int16_t>(dev.channel()) + value,
      kMinChannel, kMaxChannel));

  return 1;
}

/* static */
uint8_t MidiChannelPage::OnClick(uint8_t id, uint8_t value) {
  if (value == CLICK) {
    ToggleChangingPage();
    return 1;
  }

  return 0;
}

/* static */
uint8_t MidiChannelPage::OnSwitch(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t MidiChannelPage::OnIdle() {
  return 0;
}

/* static */
void MidiChannelPage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);
  memcpy_P(&line1[0], PSTRN("Channel"));

  UnsafeItoa(1 + dev.channel(), 2, &line2[1]);

  if (changing_page()) {
    DrawBrackets(&line2[0], 4);
  }
}

/* static */
void MidiChannelPage::UpdateLeds() {
}

/* static */
uint8_t MidiChannelPage::NoteCallback(uint8_t channel, uint8_t note, uint8_t velocity) {
  if (changing_page() && channel != dev.channel()) {
    dev.set_channel(channel);
    ui.RequestRedraw();
  }
  return 0;
}

}  // namespace midigrids
