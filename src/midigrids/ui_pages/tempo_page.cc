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
// Tempo page class.

#include "midigrids/ui_pages/tempo_page.h"
#include "midigrids/clock.h"

namespace midigrids {

/* static */
const prog_EventHandlers TempoPage::event_handlers_ PROGMEM = {
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
void TempoPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
}

/* static */
void TempoPage::OnQuit(UiPageIndex nextPage) {
}

/* static */
uint8_t TempoPage::OnIncrement(uint8_t id, int8_t value) {
  if (!changing_page())
    return 0;

  dev.set_tempo(Clamp(static_cast<int16_t>(dev.tempo()) + value,
      kMinTempo, kMaxTempo));

  return 1;
}

/* static */
uint8_t TempoPage::OnClick(uint8_t id, uint8_t value) {
  if (value == CLICK) {
    ToggleChangingPage();
    return 1;
  }

  return 0;
}

/* static */
uint8_t TempoPage::OnSwitch(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t TempoPage::OnIdle() {
  return 0;
}

/* static */
void TempoPage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);
  memcpy_P(&line1[0], PSTRN("Tempo"));

  UnsafeItoa(dev.tempo(), 3, &line2[1]);

  if (changing_page()) {
    DrawBrackets(&line2[0], 3);
  }
}

/* static */
void TempoPage::UpdateLeds() {
}

}  // namespace midigrids
