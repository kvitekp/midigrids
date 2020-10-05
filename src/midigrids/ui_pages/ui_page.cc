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

#include "midigrids/ui_pages/ui_page.h"

namespace midigrids {

/* static */
uint8_t UiPage::changing_page_;
uint8_t UiPage::reset_counter_;

/* static */
const prog_EventHandlers UiPage::event_handlers_ PROGMEM = {
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
void UiPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
}

/* static */
void UiPage::OnQuit(UiPageIndex nextPage) {
}

/* static */
uint8_t UiPage::OnIncrement(uint8_t id, int8_t value) {
  return 0;
}

/* static */
uint8_t UiPage::OnClick(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t UiPage::OnSwitch(uint8_t id, uint8_t value) {
  return 0;
}

/* static */
uint8_t UiPage::OnIdle() {
  return 0;
}

/* static */
void UiPage::UpdateScreen() {
}

/* static */
void UiPage::UpdateLeds() {
}

///////////////////////////////////////////////////////////////////////////////
// Miscellaneous helpers

/* static */
uint8_t UiPage::SelectTrkOrPage(uint8_t& trk, int8_t value) {
    if (value > 0) {
      if (trk == kNumTrk - 1) {
        return 0;
      } else
        ++trk;
    } else
    if (value < 0) {
      if (trk == 0) {
        return 0;
      } else
        --trk;
    }

    return 1;
}

/* static */
uint8_t UiPage::CheckIdleClickAction() {
  if (ui.GetEncoderLow()) {
    if (++reset_counter_ >= 2) {  // @1.225Hz
      reset_counter_ = 0;
      return 1;
    }
  } else
    reset_counter_ = 0;

  return 0;
}

/* static */
void UiPage::DrawSelStr2(char* buffer, uint8_t sel, const prog_char* strings) {
  memcpy_P(buffer, &strings[sel << 1], 2);
}

/* static */
void UiPage::DrawSelStr4(char* buffer, uint8_t sel, const prog_char* strings) {
  memcpy_P(buffer, &strings[sel << 2], 4);
}

/* static */
void UiPage::DrawSelStrN(char* buffer, uint8_t sel, const prog_char* strings, uint8_t width) {
  memcpy_P(buffer, &strings[sel * width], width);
}

/* static */
void UiPage::DrawBrackets(char* buffer, uint8_t max_width) {
  *buffer++ = '[';
  for (uint8_t n = 0; n < max_width; n++, buffer++) {
    if (buffer[1] == ' ') {
      buffer[1] = ']';
      break;
    }
  }
}

/* static */
uint8_t UiPage::Clamp(int16_t value, uint8_t min, uint8_t max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return static_cast<uint8_t>(value);
}

/* static */
int16_t UiPage::Clamp16(int16_t value, int16_t min, int16_t max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

}  // namespace midigrids
