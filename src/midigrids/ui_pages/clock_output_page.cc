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
// Clock out page class.

#include "midigrids/ui_pages/clock_output_page.h"

namespace midigrids {

/* static */
const prog_EventHandlers ClockOutputPage::event_handlers_ PROGMEM = {
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
void ClockOutputPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
}

/* static */
void ClockOutputPage::OnQuit(UiPageIndex nextPage) {
}

/* static */
uint8_t ClockOutputPage::OnIncrement(uint8_t id, int8_t value) {
  if (!changing_page())
    return 0;

  dev.set_clock_output(Clamp(static_cast<int16_t>(dev.clock_output()) + value,
      kMinClockOut, kMaxClockOut));

  return 1;
}

/* static */
uint8_t ClockOutputPage::OnClick(uint8_t id, uint8_t value) {
  if (value == CLICK) {
    ToggleChangingPage();
    return 1;
  }

  return 0;
}

/* static */
uint8_t ClockOutputPage::OnSwitch(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t ClockOutputPage::OnIdle() {
  return 0;
}

/* static */
void ClockOutputPage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);
  memcpy_P(&line1[0], PSTRN("ClockOut"));

  DrawSelStrN(&line2[1], dev.clock_output(), PSTR("never  runningalways "), 7);

  if (changing_page()) {
      DrawBrackets(&line2[0], 6);
  }
}

/* static */
void ClockOutputPage::UpdateLeds() {
}

}  // namespace midigrids
