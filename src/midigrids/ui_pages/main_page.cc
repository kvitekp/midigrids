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

#include "midigrids/ui_pages/main_page.h"
#include "midigrids/eemem.h"
#include "midigrids/slot.h"

namespace midigrids {

/* static */
uint8_t MainPage::slot_;
uint8_t MainPage::saving_slot_;
uint8_t MainPage::name_editing_;
uint8_t MainPage::name_[kNameLength];
uint8_t MainPage::offset_;

/* static */
const prog_EventHandlers MainPage::event_handlers_ PROGMEM = {
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
void MainPage::OnInit(PageInfo* pageInfo, UiPageIndex prevPage) {
  saving_slot_ = 0;
  slot_ = dev.slot();
  dev.name(name_);
  dev.set_note_callback(NoteCallback);
}

/* static */
void MainPage::OnQuit(UiPageIndex nextPage) {
  LoadSlot();
  StopNameEditing();
  dev.set_note_callback(0);
}

/* static */
uint8_t MainPage::OnIncrement(uint8_t id, int8_t value) {
  if (!changing_page())
    return 0;

  if (name_editing_) {
    if (ui.GetShifted()) {
      offset_ = Clamp(static_cast<int16_t>(offset_) + value, 0, kNameLength - 1);
      ui.set_inhibit_shift_raised(1);
    } else {
      int16_t ch = (int16_t)name_[offset_] + value;
      if (value > 0) {
        name_[offset_] = (ch > kMaxNameChar) ? kMaxNameChar : ch;
      } else
      if (value < 0) {
        name_[offset_] = (ch < kMinNameChar) ? kMinNameChar : ch;
      }
    }
  } else {
    PeekSlot(Clamp(static_cast<int16_t>(slot_) + value, 0, kNumSlots - 1));
  }

  return 1;
}

/* static */
uint8_t MainPage::OnClick(uint8_t id, uint8_t value) {
  if (value == CLICK) {
    if (saving_slot_) {
      SaveSlot();
    } else
    if (name_editing_) {
      StopNameEditing();
    } else {
      ToggleChangingPage();
      if (!changing_page()) {
        LoadSlot();
      }
    }
    return 1;
  }

  if (value == DBLCLICK) {
    LoadSlot();
    StopNameEditing();
    set_changing_page(1);
    saving_slot_ = 1;
    return 1;
  }

  return 0;
}

/* static */
uint8_t MainPage::OnSwitch(uint8_t id, uint8_t value) {
  if (!changing_page())
    return 0;

  if (id == SWITCH) {
    if (saving_slot_) {
      saving_slot_ = 0;
      CancelSlotSelection();
      ui.set_inhibit_shift_raised();
      return 1;
    }
    if (slot_ != dev.slot()) {
      CancelSlotSelection();
      ui.set_inhibit_shift_raised();
      return 1;
    }
  }

  if (id == SWITCH_RAISED) {
    if (!name_editing_) {
      // If not name editing, start it
      StartNameEditing();
    } else {
      // Otherwise char right
      if (offset_ < kNameLength - 1) {
        ++offset_;
      } else
        offset_ = 0;
    }
  }

  return 1;
}

/* static */
uint8_t MainPage::OnIdle() {
  if (CheckIdleClickAction()) {
    StartNameEditing();
    return 1;
  }
  return 0;
}

/* static */
void MainPage::UpdateScreen() {
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);

  Ui::PrintNN(&line1[0], slot_);

  memcpy((uint8_t*)&line1[3], name_, kNameLength);

  if (changing_page()) {
    line1[2] = '['; line1[2 + kNameLength + 1] = ']';
  }

  if (name_editing_) {
    display.set_cursor_position(3 + offset_);
  }

  if (saving_slot_) {
    memcpy_P(&line2[0], PSTRN("clk2save"));
  } else
  if (slot_ != dev.slot()) {
    memcpy_P(&line2[0], PSTRN("clk2load"));
  } else {
    // Draw pattern state bar graph.
    uint8_t mask = 1;
    for (uint8_t n = 0; n < kLcdWidth - 1 && n < kNumTrk; n++, mask<<= 1) {
      uint8_t state = dev.state();
      if (state & mask) {
        line2[n] = 2 + (state & (mask << 4) ? 6 : 4);
      }
    }
  }
}

/* static */
void MainPage::UpdateLeds() {
}

/* static */
void MainPage::PeekSlot(uint8_t slot) {
  Slot::ReadName(slot, name_);
  slot_ = slot;
}

/* static */
void MainPage::LoadSlot() {
  if (slot_ != dev.slot()) {
    dev.LoadSlot(slot_);
    dev.name(name_);
    eemem.set_slot(slot_);
  }
}

/* static */
void MainPage::SaveSlot() {
  StopNameEditing();
  dev.SaveSlot(slot_);
  set_changing_page(0);
  saving_slot_ = 0;
  dev.name(name_);
}

/* static */
void MainPage::CancelSlotSelection() {
  dev.name(name_);
  slot_ = dev.slot();
  set_changing_page(0);
}

/* static */
void MainPage::StartNameEditing() {
  LoadSlot();
  set_changing_page(1);
  name_editing_ = 1;
  offset_ = 0;
}

/* static */
void MainPage::StopNameEditing() {
  if (!name_editing_)
    return;

  name_editing_ = 0;
  set_changing_page(0);
  display.set_cursor_position(kLcdNoCursor);
  dev.set_name(name_);
}

/* static */
void MainPage::CancelNameEditing() {
  if (!name_editing_)
    return;

  name_editing_ = 0;
  set_changing_page(0);
  display.set_cursor_position(kLcdNoCursor);
}

/* static */
uint8_t MainPage::NoteCallback(uint8_t channel, uint8_t note, uint8_t velocity) {
  if (channel == dev.channel()) {
    ui.RequestRedraw();
  }
  return 0;
}

}  // namespace midigrids
