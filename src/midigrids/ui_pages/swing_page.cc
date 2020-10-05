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
// Swing page class.

#include "midigrids/ui_pages/swing_page.h"
#include "midigrids/clock.h"

namespace midigrids {

/* static */
const prog_EventHandlers SwingPage::event_handlers_ PROGMEM = {
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
void SwingPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
}

/* static */
void SwingPage::OnQuit(UiPageIndex nextPage) {
}

/* static */
uint8_t SwingPage::OnIncrement(uint8_t id, int8_t value) {
  if (!changing_page())
    return 0;

  dev.set_swing(Clamp16(static_cast<int16_t>(dev.swing()) + value,
      kMinSwing, kMaxSwing));

  return 1;
}

/* static */
uint8_t SwingPage::OnClick(uint8_t id, uint8_t value) {
  if (value == CLICK) {
    ToggleChangingPage();
    return 1;
  }

  return 0;
}

/* static */
uint8_t SwingPage::OnSwitch(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t SwingPage::OnIdle() {
  return 0;
}

/* static */
void SwingPage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);
  memcpy_P(&line1[0], PSTRN("Swing"));

  uint8_t x = 1;
  int16_t swing_int = dev.swing() / 10;
  x += UnsafeItoaLen(swing_int, 3, &line2[x]); line2[x++] = '.';
  x += UnsafeItoaLen(dev.swing() - swing_int * 10, 1, &line2[x]); line2[x] = '%';

  if (changing_page()) {
    DrawBrackets(&line2[0], 6);
  }
}

/* static */
void SwingPage::UpdateLeds() {
}

}  // namespace midigrids
