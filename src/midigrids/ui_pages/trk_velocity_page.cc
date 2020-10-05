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
// Track velocity page class.

#include "midigrids/ui_pages/trk_velocity_page.h"

namespace midigrids {

/* <static> */
uint8_t TrkVelocityPage::trk_;
/* </static> */

/* static */
const prog_EventHandlers TrkVelocityPage::event_handlers_ PROGMEM = {
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
void TrkVelocityPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
  if (prevPage <= LAST_PAGE) {
    trk_ = prevPage < PAGE_TRK_VELOCITY ? 0 : kNumTrk - 1;
  }
  dev.set_note_callback(NoteCallback);
}

/* static */
void TrkVelocityPage::OnQuit(UiPageIndex nextPage) {
  dev.set_note_callback(0);
}

/* static */
uint8_t TrkVelocityPage::OnIncrement(uint8_t id, int8_t value) {
  if (!changing_page()) {
    return SelectTrkOrPage(trk_, value);
  }

  dev.set_trk_velocity(trk_,
    Clamp(static_cast<int16_t>(dev.trk_velocity(trk_)) + value,
        kMinTrkVelocity, kMaxTrkVelocity));

  return 1;
}

/* static */
uint8_t TrkVelocityPage::OnClick(uint8_t id, uint8_t value) {
  if (value == CLICK) {
    ToggleChangingPage();
    return 1;
  }

  return 0;
}

/* static */
uint8_t TrkVelocityPage::OnSwitch(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t TrkVelocityPage::OnIdle() {
  return 0;
}

/* static */
void TrkVelocityPage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);

  memcpy_P(&line1[0], PSTRN("TrkXVelo"));
  UnsafeItoa(1 + trk_, 3, &line1[3]);

  UnsafeItoa(dev.trk_velocity(trk_), 3, &line2[1]);

  if (changing_page()) {
      DrawBrackets(&line2[0], 8);
  }
}

/* static */
void TrkVelocityPage::UpdateLeds() {
}

/* static */
uint8_t TrkVelocityPage::NoteCallback(uint8_t channel, uint8_t note, uint8_t velocity) {
  if (changing_page() && channel == dev.channel()) {
    dev.set_trk_velocity(trk_, velocity);
    ui.RequestRedraw();
  }
  return 0;
}

}  // namespace midigrids
