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
//
// -----------------------------------------------------------------------------
//
// Pattern page class.

#include "midigrids/ui_pages/pattern_page.h"

namespace midigrids {

#ifdef ENABLE_EUCLIDEAN_MODE

/* static */
const prog_EventHandlers PatternPage::event_handlers_ PROGMEM = {
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
void PatternPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
}

/* static */
void PatternPage::OnQuit(UiPageIndex nextPage) {
}

/* static */
uint8_t PatternPage::OnIncrement(uint8_t id, int8_t value) {
  if (!changing_page())
    return 0;

  dev.set_pattern(Clamp(static_cast<int16_t>(dev.pattern()) + value,
      kMinPattern, kMaxPattern));

  return 1;
}

/* static */
uint8_t PatternPage::OnClick(uint8_t id, uint8_t value) {
  if (value == CLICK) {
    ToggleChangingPage();
    return 1;
  }

  return 0;
}

/* static */
uint8_t PatternPage::OnSwitch(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t PatternPage::OnIdle() {
  return 0;
}

/* static */
void PatternPage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);
  memcpy_P(&line1[0], PSTRN("Pattern"));

  DrawSelStrN(&line2[1], dev.pattern(), PSTR("grids euclid"), 6);

  if (changing_page()) {
      DrawBrackets(&line2[0], 6);
  }
}

/* static */
void PatternPage::UpdateLeds() {
}

#endif // #ifdef ENABLE_EUCLIDEAN_MODE

}  // namespace midigrids
