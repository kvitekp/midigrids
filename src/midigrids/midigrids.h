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

#ifndef MIDIGRIDS_MIDIGRIDS_H_
#define MIDIGRIDS_MIDIGRIDS_H_

#include "avrlib/base.h"

#include <string.h>

#ifdef ENABLE_DEBUG_OUTPUT
#include <stdlib.h>
#include <stdio.h>
#endif

#include "midigrids/hardware_config.h"
#include "midigrids/resources.h"
#include "midigrids/settings.h"
#include "midigrids/duration.h"

namespace midigrids {

#define MIDIGRIDS_VERSION "1.00"

// Configuration

static const uint8_t kNumSlots = 100;

static const uint8_t kNumTrk = 4;

static const uint8_t kNameLength = 4;  // 8 - 4 ("NN[name]")
static const uint8_t kMinNameChar = ' ';
static const uint8_t kMaxNameChar = '~';

static const uint8_t kDefChannel = 10 - 1;
static const uint8_t kMinChannel = 0;
static const uint8_t kMaxChannel = 0xf;

static const uint8_t kMinTempo = 25;
static const uint8_t kMaxTempo = 250;
static const uint8_t kDefTempo = 120;

static const int16_t kMinSwing = 1;
static const int16_t kMaxSwing = 999;
static const int16_t kDefSwing = 500;

static const uint8_t kMinStepLength = 0;
static const uint8_t kMaxStepLength = kNoteDurationCount - 1;
static const uint8_t kDefStepLength = k16thNote;

static const uint8_t kMinClockMode = CLOCK_MODE_INTERNAL;
static const uint8_t kMaxClockMode = CLOCK_MODE_EXTERNAL;
static const uint8_t kDefClockMode = CLOCK_MODE_INTERNAL;

static const uint8_t kMinClockOut = CLOCK_OUTPUT_NEVER;
static const uint8_t kMaxClockOut = CLOCK_OUTPUT_ALWAYS;
static const uint8_t kDefClockOut = CLOCK_OUTPUT_NEVER;

static const uint8_t kMinPattern = PATTERN_GRIDS;
static const uint8_t kMaxPattern = PATTERN_EUCLIDEAN;
static const uint8_t kDefPattern = PATTERN_GRIDS;

static const uint8_t kMinMapXY = 0;
static const uint8_t kMaxMapXY = 255;
static const uint8_t kDefMapXY = 160;

static const uint8_t kMinMapChaos = 0;
static const uint8_t kMaxMapChaos = 0x3f;
static const uint8_t kDefMapChaos = 0;

static const uint8_t kMinAccent = 0;
static const uint8_t kMaxAccent = 0x7f;
static const uint8_t kDefAccent = 0;

static const uint8_t kMinTrkFill = 0;
static const uint8_t kMaxTrkFill = 0xff;
static const uint8_t kDefTrkFill = kMaxTrkFill / 2;
static const uint8_t kDefTrk1Fill = 100;
static const uint8_t kDefTrk2Fill = 132;
static const uint8_t kDefTrk3Fill = 100;
static const uint8_t kDefTrk4Fill = 255;

static const uint8_t kMinTrkSize = 0;
static const uint8_t kMaxTrkSize = 32;
static const uint8_t kDefTrkSize = 16;

static const uint8_t kMinTrkNote = 0;
static const uint8_t kMaxTrkNote = 0x7f;
static const uint8_t kDefTrkNote = 60; // C4
static const uint8_t kDefTrk1Note = 36; // C2
static const uint8_t kDefTrk2Note = 38; // D2
static const uint8_t kDefTrk3Note = 41; // F2
static const uint8_t kDefTrk4Note = 42; // F#2

static const uint8_t kMinTrkVelocity = 0;
static const uint8_t kMaxTrkVelocity = 0x7f;
static const uint8_t kDefTrkVelocity = 100;

static const uint8_t kMinStrobeWidth = 0;
static const uint8_t kMaxStrobeWidth = 4;
static const uint8_t kDefStrobeWidth = 0;

// Control Change codes

static const uint8_t kCCTrk1Fill = 14;
static const uint8_t kCCTrk2Fill = 15;
static const uint8_t kCCTrk3Fill = 16;
static const uint8_t kCCTrk4Fill = 17;
static const uint8_t kCCMapX = 18;
static const uint8_t kCCMapY = 19;
static const uint8_t kCCMapChaos = 20;
static const uint8_t kCCAccent = 21;

// Useful declarations

#define numbof(a)  (sizeof(a)/sizeof(a[0]))
#define lengof(s)  (numbof(s) - 1)

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

#define SETFLAG(f,b)  (f)|= (b)
#define CLRFLAG(f,b)  (f)&=~(b)
#define SETFLAGTO(f,b,c)  if (c) SETFLAG(f,b); else CLRFLAG(f,b);

#define PSTRN(str) PSTR(str), lengof(str)

#ifdef _MSC_VER
#define OFFSETOF(s,m) (uint16_t)offsetof(s,m)
#else
#define OFFSETOF(s,m) (uint16_t)&reinterpret_cast<const volatile char&>((((s *)0)->m))
#endif

#ifndef _MSC_VER
#define assert(expr)  (void)0
#endif

// 50ns @20MHz
static inline void nop() {
  __asm__ volatile (
    "nop"
    );
}

// count*160ns @20MHz, 0=38us
static inline void Delay(uint8_t count) {
  __asm__ volatile (
    "1: dec %0" "\n\t"
    "brne 1b"
    : "=r" (count)
    : "0" (count)
  );
}

// Disable interrupts
class DisableInterrupts {
  uint8_t sreg_;
 public:
   DisableInterrupts() { sreg_= SREG; cli(); }
   ~DisableInterrupts() { SREG = sreg_; }
};

// Profiling aids
struct RaiseIOPort {
  typedef IOPort0 DebugPort;
  RaiseIOPort() { DebugPort::High(); }
  RaiseIOPort(uint8_t count) { DebugPort::High(); Delay(count); }
  ~RaiseIOPort() { DebugPort::Low(); }
};

struct RaiseIOPort2 {
  typedef IOPort1 DebugPort;
  RaiseIOPort2() { DebugPort::High(); }
  RaiseIOPort2(uint8_t count) { DebugPort::High(); Delay(count); }
  ~RaiseIOPort2() { DebugPort::Low(); }
};

}  // namespace midigrids

#endif  // MIDIGRIDS_MIDIGRIDS_H_
