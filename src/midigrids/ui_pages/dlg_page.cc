// Copyright 2012 Peter Kvitek.
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
// Dialog page class.

#include "midigrids/ui_pages/dlg_page.h"

namespace midigrids {

/* static */
PageInfo* DlgPage::info_;

/* static */
const prog_EventHandlers DlgPage::event_handlers_ PROGMEM = {
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
void DlgPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
  info_ = pageInfo;
}

/* static */
void DlgPage::OnQuit(UiPageIndex nextPage) {
}

/* static */
uint8_t DlgPage::OnIncrement(uint8_t id, int8_t value) {
  return 1;
}

/* static */
uint8_t DlgPage::OnClick(uint8_t id, uint8_t value) {
  info_->dlg_data.result = kNoDlgResult - 1;
  return 1;
}

/* static */
uint8_t DlgPage::OnSwitch(uint8_t id, uint8_t value) {
  if (id == SWITCH) {
    info_->dlg_data.result = value;
  }

  return 1;
}

/* static */
uint8_t DlgPage::OnIdle() {
  return 0;
}

/* static */
void DlgPage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);
  size_t cch;

  const prog_char* newline = strchr_P(info_->dlg_data.text, '\n');
  if (newline) {
    cch = static_cast<size_t>(newline - info_->dlg_data.text);
    memcpy_P(&line1[0], info_->dlg_data.text, cch);
    memcpy_P(&line2[0], info_->dlg_data.text + cch + 1, strlen_P(info_->dlg_data.text) - cch - 1);
  } else
    memcpy_P(&line1[0], info_->dlg_data.text, strlen_P(info_->dlg_data.text));

  cch = strlen_P(info_->dlg_data.buttons);
  memcpy_P(&line2[kLcdWidth - cch], info_->dlg_data.buttons, cch);
}

/* static */
void DlgPage::UpdateLeds() {
}

}  // namespace midigrids
