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
// Track note page class.

#include "midigrids/ui_pages/trk_note_page.h"

namespace midigrids {

/* <static> */
uint8_t TrkNotePage::trk_;
/* </static> */

/* static */
const prog_EventHandlers TrkNotePage::event_handlers_ PROGMEM = {
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
void TrkNotePage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
  if (prevPage <= LAST_PAGE) {
    trk_ = prevPage < PAGE_TRK_NOTE ? 0 : kNumTrk - 1;
  }
  dev.set_note_callback(NoteCallback);
}

/* static */
void TrkNotePage::OnQuit(UiPageIndex nextPage) {
  dev.set_note_callback(0);
}

/* static */
uint8_t TrkNotePage::OnIncrement(uint8_t id, int8_t value) {
  if (!changing_page()) {
    return SelectTrkOrPage(trk_, value);
  }

  dev.set_trk_note(trk_,
    Clamp(static_cast<int16_t>(dev.trk_note(trk_)) + value,
        kMinTrkNote, kMaxTrkNote));

  return 1;
}

/* static */
uint8_t TrkNotePage::OnClick(uint8_t id, uint8_t value) {
  if (value == CLICK) {
    ToggleChangingPage();
    return 1;
  }

  return 0;
}

/* static */
uint8_t TrkNotePage::OnSwitch(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t TrkNotePage::OnIdle() {
  return 0;
}

/* static */
void TrkNotePage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);

  memcpy_P(&line1[0], PSTRN("TrkXNote"));
  UnsafeItoa(1 + trk_, 3, &line1[3]);

  ui.PrintNote(&line2[1], dev.trk_note(trk_));

  if (changing_page()) {
      DrawBrackets(&line2[0], 8);
  }
}

/* static */
void TrkNotePage::UpdateLeds() {
}

/* static */
uint8_t TrkNotePage::NoteCallback(uint8_t channel, uint8_t note, uint8_t velocity) {
  if (changing_page() && channel == dev.channel()) {
    dev.set_trk_note(trk_, note);
    ui.RequestRedraw();
  }
  return 0;
}

}  // namespace midigrids
