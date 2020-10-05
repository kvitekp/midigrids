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
// Main page class.

#ifndef MIDIGRIDS_UI_PAGES_MAIN_PAGE_H_
#define MIDIGRIDS_UI_PAGES_MAIN_PAGE_H_

#include "midigrids/ui_pages/ui_page.h"

namespace midigrids {

class MainPage : public UiPage {
 public:
  MainPage() { }

  static void OnInit(PageInfo* pageInfo, UiPageIndex prevPage);
  static void OnQuit(UiPageIndex nextPage);

  static uint8_t OnIncrement(uint8_t id, int8_t value);
  static uint8_t OnClick(uint8_t id, uint8_t value);
  static uint8_t OnSwitch(uint8_t id, uint8_t value);
  static uint8_t OnIdle();

  static void UpdateScreen();
  static void UpdateLeds();

  static const prog_EventHandlers event_handlers_;

 protected:
  static uint8_t slot_;
  static uint8_t saving_slot_;
  static uint8_t name_editing_;
  static uint8_t name_[kNameLength];
  static uint8_t offset_;

  static void PeekSlot(uint8_t slot);
  static void LoadSlot();
  static void SaveSlot();
  static void CancelSlotSelection();

  static void StartNameEditing();
  static void StopNameEditing();
  static void CancelNameEditing();

  static uint8_t NoteCallback(uint8_t channel, uint8_t note, uint8_t velocity);

  DISALLOW_COPY_AND_ASSIGN(MainPage);
};

}  // namespace midigrids

#endif  // MIDIGRIDS_UI_PAGES_MAIN_PAGE_H_
