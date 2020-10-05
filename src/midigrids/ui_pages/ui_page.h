// Copyright 2012 Peter Kvitek.
//
// Author: Peter Kvitek (pete@kvitek.com)
//
// Based on Ambika code by Olivier Gillet (ol.gillet@gmail.com)
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
// Base UI page class.

#ifndef MIDIGRIDS_UI_PAGES_UI_PAGE_H_
#define MIDIGRIDS_UI_PAGES_UI_PAGE_H_

#include "midigrids/midigrids.h"
#include "midigrids/display.h"
#include "midigrids/eemem.h"
#include "midigrids/leds.h"
#include "midigrids/dev.h"
#include "midigrids/ui.h"

#include "avrlib/string.h"

namespace midigrids {

class UiPage {
 public:
  UiPage() { }

  static void OnInit(PageInfo* pageInfo, UiPageIndex prevPage);
  static void OnQuit(UiPageIndex nextPage);

  static uint8_t OnIncrement(uint8_t id, int8_t value);
  static uint8_t OnClick(uint8_t id, uint8_t value);
  static uint8_t OnSwitch(uint8_t id, uint8_t value);
  static uint8_t OnIdle();

  static void UpdateScreen();
  static void UpdateLeds();

  static const prog_EventHandlers event_handlers_;

  static uint8_t changing_page() { return changing_page_; }
  static void set_changing_page(uint8_t changing_page) { changing_page_ = changing_page; }

 protected:
  static uint8_t changing_page_;
  static uint8_t reset_counter_;

  static void ToggleChangingPage() {
    changing_page_ = !changing_page_;
  }

  static uint8_t SelectTrkOrPage(uint8_t& trk, int8_t value);

  static uint8_t CheckIdleClickAction();

  static void DrawSelStr2(char* buffer, uint8_t sel, const prog_char* strings);
  static void DrawSelStr4(char* buffer, uint8_t sel, const prog_char* strings);
  static void DrawSelStrN(char* buffer, uint8_t sel, const prog_char* strings, uint8_t width);

  static void DrawBrackets(char* buffer, uint8_t max_width);

  static uint8_t Clamp(int16_t value, uint8_t min, uint8_t max);
  static uint8_t Clamp7F(int16_t value) { return Clamp(value, 0, 0x7f); }
  static int16_t Clamp16(int16_t value, int16_t min, int16_t max);

  DISALLOW_COPY_AND_ASSIGN(UiPage);
};

}  // namespace midigrids

#endif  // MIDIGRIDS_UI_PAGES_UI_PAGE_H_
