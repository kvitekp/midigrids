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
// Track fill page class.

#include "midigrids/ui_pages/trk_fill_page.h"

namespace midigrids {

/* <static> */
uint8_t TrkFillPage::trk_;
/* </static> */

/* static */
const prog_EventHandlers TrkFillPage::event_handlers_ PROGMEM = {
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
void TrkFillPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
  trk_ = pageInfo->index - PAGE_TRK1_FILL;
}

/* static */
void TrkFillPage::OnQuit(UiPageIndex nextPage) {
}

/* static */
uint8_t TrkFillPage::OnIncrement(uint8_t id, int8_t value) {
  if (!changing_page())
    return 0;

  dev.set_trk_fill(trk_,
    Clamp(static_cast<int16_t>(dev.trk_fill(trk_)) + value,
        kMinTrkFill, kMaxTrkFill));

  return 1;
}

/* static */
uint8_t TrkFillPage::OnClick(uint8_t id, uint8_t value) {
  if (value == CLICK) {
    ToggleChangingPage();
    return 1;
  }

  return 0;
}

/* static */
uint8_t TrkFillPage::OnSwitch(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t TrkFillPage::OnIdle() {
  return 0;
}

/* static */
void TrkFillPage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);

  memcpy_P(&line1[0], PSTRN("TrkXFill"));
  UnsafeItoa(1 + trk_, 3, &line1[3]);

  UnsafeItoa(dev.trk_fill(trk_), 3, &line2[1]);

  if (changing_page()) {
      DrawBrackets(&line2[0], 8);
  }
}

/* static */
void TrkFillPage::UpdateLeds() {
}

}  // namespace midigrids
