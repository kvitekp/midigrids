// Copyright 2017 Peter Kvitek.
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

#include "midigrids/ui.h"

#include "avrlib/op.h"
#include "avrlib/string.h"

#include "midigrids/resources.h"
#include "midigrids/display.h"
#include "midigrids/storage.h"
#include "midigrids/clock.h"
#include "midigrids/eemem.h"
#include "midigrids/slot.h"
#include "midigrids/leds.h"
#include "midigrids/dev.h"

#include "midigrids/ui_pages/main_page.h"
#include "midigrids/ui_pages/tempo_page.h"
#include "midigrids/ui_pages/swing_page.h"
#include "midigrids/ui_pages/step_length_page.h"
#include "midigrids/ui_pages/pattern_page.h"
#include "midigrids/ui_pages/map_xy_page.h"
#include "midigrids/ui_pages/map_chaos_page.h"
#include "midigrids/ui_pages/accent_page.h"
#include "midigrids/ui_pages/trk_fill_page.h"
#include "midigrids/ui_pages/trk_size_page.h"
#include "midigrids/ui_pages/trk_note_page.h"
#include "midigrids/ui_pages/trk_velocity_page.h"
#include "midigrids/ui_pages/midi_channel_page.h"
#include "midigrids/ui_pages/clock_mode_page.h"
#include "midigrids/ui_pages/clock_output_page.h"
#include "midigrids/ui_pages/strobe_width_page.h"
#include "midigrids/ui_pages/dlg_page.h"

namespace midigrids {

using namespace avrlib;

static const uint8_t kDblClickTime = 250;

static const uint8_t kHeartBeatChar = 1;

/* <static> */
Encoder Ui::encoder_;

Switch Ui::switch_;

EventQueue<32> Ui::queue_;

UiPageIndex Ui::active_page_;
UiPageIndex Ui::last_page_;
EventHandlers Ui::event_handlers_;
PageInfo Ui::page_info_;

uint8_t Ui::cycle_;
uint8_t Ui::request_redraw_;
uint8_t Ui::inhibit_shift_raised_;
uint32_t Ui::last_click_time_;

uint8_t Ui::tick_;
uint8_t Ui::beat_;
uint8_t Ui::last_beat_;
uint8_t Ui::last_beat_char_;
/* </static> */

/* extern */
Ui ui;

const prog_PageInfo page_registry[] PROGMEM = {
  { PAGE_MAIN,
    &MainPage::event_handlers_,
  },
  { PAGE_TEMPO,
    &TempoPage::event_handlers_,
  },
  { PAGE_SWING,
    &SwingPage::event_handlers_,
  },
  { PAGE_STEP_LENGTH,
    &StepLengthPage::event_handlers_,
  },
#ifdef ENABLE_EUCLIDEAN_MODE
  { PAGE_PATTERN,
    &PatternPage::event_handlers_,
  },
#endif
  { PAGE_MAP_X,
    &MapXYPage::event_handlers_,
  },
  { PAGE_MAP_Y,
    &MapXYPage::event_handlers_,
  },
  { PAGE_MAP_CHAOS,
    &MapChaosPage::event_handlers_,
  },
  { PAGE_ACCENT,
    &AccentPage::event_handlers_,
  },
  { PAGE_TRK1_FILL,
    &TrkFillPage::event_handlers_,
  },
  { PAGE_TRK2_FILL,
    &TrkFillPage::event_handlers_,
  },
  { PAGE_TRK3_FILL,
    &TrkFillPage::event_handlers_,
  },
  { PAGE_TRK4_FILL,
    &TrkFillPage::event_handlers_,
  },
#ifdef ENABLE_EUCLIDEAN_MODE
  { PAGE_TRK_SIZE,
    &TrkSizePage::event_handlers_,
  },
#endif
  { PAGE_TRK_NOTE,
    &TrkNotePage::event_handlers_,
  },
  { PAGE_TRK_VELOCITY,
    &TrkVelocityPage::event_handlers_,
  },
  { PAGE_MIDI_CHANNEL,
    &MidiChannelPage::event_handlers_,
  },
  { PAGE_CLOCK_MODE,
    &ClockModePage::event_handlers_,
  },
  { PAGE_CLOCK_OUTPUT,
    &ClockOutputPage::event_handlers_,
  },
  { PAGE_STROBE_WIDTH,
    &StrobeWidthPage::event_handlers_,
  },
  { PAGE_DLG,
    &DlgPage::event_handlers_,
  },
};

/* static */
void Ui::Init() {
  encoder_.Init();
  switch_.Init();
  lcd.Init();
  display.Init();

  lcd.SetCustomCharMapRes(chr_res_custom_icons, 8, kHeartBeatChar);

  dev.set_running_callback(RunningCallback);
  dev.set_tick_callback(TickCallback);

  if (OneKeySalute()) {
    InitDev();
  }

  Logo();

  ShowPage(FIRST_PAGE);
}

/* static */
uint8_t Ui::OneKeySalute() {
  for (uint8_t n = 0; n < 8; n++) {
    switch_.Read();
    _delay_ms(1);
  }

  return switch_.low();
}

/* static */
void Ui::InitDev() {
  ClearScreen();
  char* line1 = display.line_buffer(0);
  memcpy_P(&line1[0], PSTRN("Initing!"));
  RedrawScreen();
  dev.InitDev();
}

/* static */
void Ui::Logo() {
  ClearScreen();
  char* line1 = display.line_buffer(0);
  char* line2 = display.line_buffer(1);
  memcpy_P(&line1[0], PSTRN("MIDIGrid"));
#if 1
  memcpy_P(&line2[1], PSTRN("v" MIDIGRIDS_VERSION));
#else
  int x = UnsafeItoaLen(sizeof(SlotData), 2, &line2[0]); line2[x++] = '/';
  x+= UnsafeItoaLen(OFFSETOF(SlotData, mod_), 2, &line2[x]); line2[x++] = '/';
  x+= UnsafeItoaLen(sizeof(ModData), 3, &line2[x]);
#endif
  RedrawScreen();

  for (uint8_t n = 0; n < 3; n++) {
    MidiInLed::On(); MidiOutLed::On(); _delay_ms(150);
    MidiInLed::Off(); MidiOutLed::Off(); _delay_ms(150);
  }
}

/* static */
void Ui::Poll() {

  // 2.45KHz
  cycle_++;

  PollEncoder();

  if ((cycle_ & 3) == 0) {
    // 612Hz
    PollSwitches();
  }

  // Output
  lcd.Tick();
}

/* static */
void Ui::PollEncoder() {
  int8_t increment = encoder_.Read();
  if (increment) {
    AddEvent(CONTROL_ENCODER, ENCODER, increment);
  }
  if (encoder_.lowered()) {
    uint32_t time = milliseconds();
    uint8_t value = ((time - last_click_time_) < kDblClickTime ? DBLCLICK : CLICK) + GetShifted();
    AddEvent(CONTROL_ENCODER_CLICK, ENCODER, value);
    last_click_time_ = time;
  }
}

/* static */
void Ui::PollSwitches() {
  switch_.Read();
  if (switch_.lowered()) {
    AddEvent(CONTROL_SWITCH, SWITCH);
  } else
  if (switch_.raised()) {
    if (inhibit_shift_raised_) {
      inhibit_shift_raised_ = 0;
    } else
      AddEvent(CONTROL_SWITCH, SWITCH_RAISED);
  }
}

/* static */
void Ui::DoEvents() {
  display.Tick();

  uint8_t redraw = request_redraw_; request_redraw_ = 0;
  while (queue_.available()) {
    Event e = queue_.PullEvent();
    queue_.Touch();
    HandleEvent(e);
    redraw = 1;
  }

  if (queue_.idle_time_ms() > 1000) {
    queue_.Touch();
    if ((*event_handlers_.OnIdle)()) {
      redraw = 1;
    }
  }

  if (redraw) {
    ClearScreen();
    (*event_handlers_.UpdateScreen)();
  }

  (*event_handlers_.UpdateLeds)();

  BlinkBeat(redraw);
}

/* static */
void Ui::HandleEvent(const Event& e) {
  switch (e.control_type) {
    case CONTROL_ENCODER:
      if (!(*event_handlers_.OnIncrement)(e.control_id, e.value)) {
        HandleEncoderEvent(e);
      }
      break;
    case CONTROL_ENCODER_CLICK:
      if (!(*event_handlers_.OnClick)(e.control_id, e.value)) {
        HandleEncoderClickEvent(e);
      }
      break;
    case CONTROL_SWITCH:
      if (!(*event_handlers_.OnSwitch)(e.control_id, e.value)) {
        HandleSwitchEvent(e);
      }
      break;
    case CONTROL_REQUEST:
      {
        HandleRequestEvent(e);
      }
      break;
  }
}

/* static */
void Ui::HandleEncoderEvent(const Event& e) {
  // Cycle between primary pages on ENC rotation
  if (e.control_id == ENCODER) {
    uint8_t page = active_page_;
    if ((int8_t)e.value > 0) {
      if (page < LAST_PAGE) {
        page++;
      }
    } else
    if ((int8_t)e.value < 0) {
      if (page > FIRST_PAGE) {
        page--;
      }
    }
    ShowPage(static_cast<UiPageIndex>(page));
  }
}

void Ui::HandleEncoderClickEvent(const Event& e) {
  if (e.control_id == ENCODER) {
    switch (e.value) {
      case CLICK:
        break;
      case CLICK_SHIFTED:
        break;
      case DBLCLICK:
        UiPage::set_changing_page(0);
        SaveSlot();
        break;
      case DBLCLICK_SHIFTED:
        break;
    }
  }
}

void Ui::HandleSwitchEvent(const Event& e) {
  if (e.control_id == SWITCH) {
    dev.ToggleRun();
  } else
  if (e.control_id == SWITCH_SHIFTED) {
    ;
  } else
  if (e.control_id == SWITCH_RAISED) {
    ;
  } else
  if (e.control_id == SWITCH_RAISED_SHIFTED) {
    ;
  }
}

/* static */
void Ui::HandleRequestEvent(const Event& e) {
  switch (e.control_id) {
    case REQUEST_SHOWPAGE:
      if (active_page_ != e.value) {
        ShowPage(static_cast<UiPageIndex>(e.value));
      } else
        UpdateScreen();
      break;
  }
}

/* static */
void Ui::ShowPage(UiPageIndex page) {
  // Flush the event queue.
  queue_.Flush();
  queue_.Touch();

  // Inform current page if any
  if (*event_handlers_.OnQuit) {
    (*event_handlers_.OnQuit)(page);
  }

#ifdef ENABLE_DEBUG_OUTPUT
  printf("ShowPage: %d\n", page);
#endif

  // Only remember primary pages
  if (active_page_ >= FIRST_PAGE && active_page_ <= LAST_PAGE) {
    last_page_ = active_page_;
  }

  // Activate new page
  UiPageIndex prevPage = active_page_;
  active_page_ = page;

  // Load the page info structure in RAM.
  ResourcesManager::Load(page_registry, page, &page_info_);

  // Load the event handlers structure in RAM.
  ResourcesManager::Load(page_info_.event_handlers, 0, &event_handlers_);

  (*event_handlers_.OnInit)(&page_info_, prevPage);

  UpdateScreen();
}

/* static */
void Ui::EnsurePage(UiPageIndex page) {
  if (page != active_page_) {
    AddRequest(REQUEST_SHOWPAGE, static_cast<uint8_t>(page));
  }
}

/* static */
Dialog::Dialog(const prog_char* text, const prog_char* buttons, uint8_t num_buttons) {
  data_.text = text;
  data_.buttons = buttons;
  data_.num_buttons = num_buttons;
  Ui::ShowDialog(data_);
}

/* static */
void Ui::ShowDialog(DlgData& dlg_data) {
  // Flush the event queue.
  queue_.Flush();
  queue_.Touch();

  // Load the Dlg page info structure in RAM.
  ResourcesManager::Load(page_registry, PAGE_DLG, &page_info_);

  // Load the event handlers structure in RAM.
  ResourcesManager::Load(page_info_.event_handlers, 0, &event_handlers_);

  memcpy(&page_info_.dlg_data, &dlg_data, sizeof(DlgData));

  (*event_handlers_.OnInit)(&page_info_, active_page_);

  UpdateScreen();

  // Run local event loop
  page_info_.dlg_data.result = kNoDlgResult;
  while (page_info_.dlg_data.result == kNoDlgResult) {
    ui.DoEvents();
  }

  // Set result for the caller
  dlg_data.result = page_info_.dlg_data.result;

  // Flush the event queue.
  queue_.Flush();
  queue_.Touch();

  // Load the active page info structure in RAM.
  ResourcesManager::Load(page_registry, active_page_, &page_info_);

  // Load the event handlers structure in RAM.
  ResourcesManager::Load(page_info_.event_handlers, 0, &event_handlers_);

  UpdateScreen();
}

/* static */
void Ui::ClearScreen() {
  display.Clear();
  last_beat_char_ = ' ';
}

/* static */
void Ui::UpdateScreen() {
  ClearScreen();
  (*event_handlers_.UpdateScreen)();
}

/* static */
void Ui::RedrawScreen() {
  for (uint8_t n = 0; n < kLcdWidth * kLcdHeight; n++) {
    display.Tick();
  }
}

/* static */
void Ui::SaveSlot() {
  char* line2 = display.line_buffer(1);
  PrintNN(&line2[0], dev.slot());
  memcpy_P(&line2[2], PSTRN(" saved"));
  RedrawScreen();
  dev.SaveSlot();
  _delay_ms(500);
  UpdateScreen();
}

/* static */
void Ui::PrintHex(char* buffer, uint8_t value) {
  *buffer++ = NibbleToAscii(U8ShiftRight4(value));
  *buffer = NibbleToAscii(value & 0xf);
}

/* static */
void Ui::PrintNote(char* buffer, uint8_t note) {
  uint8_t octave = 0;
  while (note >= 12) {
    ++octave;
    note -= 12;
  }

  static const prog_char note_names[] PROGMEM = "CCDDEFFGGAAB";
  static const prog_char octaves[] PROGMEM = "-0123456789";

  *buffer++ = ResourcesManager::Lookup<char, uint8_t>(
      note_names, note);

  switch (note) {
    case 1: case 3: case 6: case 8: case 10:
      *buffer++ = '#';
      break;
  }

  *buffer = ResourcesManager::Lookup<char, uint8_t>(
      octaves, octave);
}

/* static */
void Ui::PrintNN(char* buffer, uint16_t numb, char filler) {
  uint8_t x = 0;
  if (numb < 10) { x = 1; buffer[0] = filler; }
  UnsafeItoa(numb, 2 - x, &buffer[x]);
}

/* static */
void Ui::PrintNNN(char* buffer, uint16_t numb, char filler) {
  uint8_t x = 0;
  if (numb < 10)  { x = 2; buffer[0] = filler; buffer[1] = filler; } else
  if (numb < 100) { x = 1; buffer[0] = filler; }
  UnsafeItoa(numb, 3 - x, &buffer[x]);
}

/* static */
void Ui::BlinkBeat(uint8_t redrawn) {
  if (!redrawn && beat_ == last_beat_)
    return;

  last_beat_ = beat_;

  static const uint8_t x = kLcdWidth - 1;
  char* line2 = display.line_buffer(1);

  if (redrawn) {
    last_beat_char_ = line2[x];
  }

  if (dev.running() && (beat_ & 1) == 0) {
    line2[x] = kHeartBeatChar;
  } else
  if (!redrawn) {
    line2[x] = last_beat_char_;
  }
}

/* static */
void Ui::RunningCallback(uint8_t running) {
  if (running) {
    tick_ = 0;
    beat_ = 0;
  }
  last_beat_ ^= -1;
  ui.RequestRedraw();
}

/* static */
void Ui::TickCallback() {
  if (++tick_ >= kNumTicksPer8thNote) {
    ++beat_; tick_ = 0;
  }
}

/* static */
int16_t Ui::Scale(int16_t x0, int16_t x0min, int16_t x0max,
    int16_t x1min, int16_t x1max) {
  if (x0max == x0min)
    return x1max;

  int16_t x1 = x1min + ((x0 - x0min) * (x1max - x1min)) / (x0max - x0min);

  if (x1min < x1max) {
    if (x1 < x1min) {
      x1 = x1min;
    } else
    if (x1 > x1max) {
      x1 = x1max;
    }
  } else {
    if (x1 < x1max) {
      x1 = x1max;
    } else
    if (x1 > x1min) {
      x1 = x1min;
    }
  }

  return x1;
}

}  // namespace midigrids
