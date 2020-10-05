// Copyright 2018 Peter Kvitek.
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
// Track model definitions.

#include "midigrids/midigrids.h"
#include "midigrids/trk.h"

using namespace avrlib;

namespace midigrids {

void Trk::Init() {
  fill_ = kDefTrkFill;
  size_ = kDefTrkSize;
  note_ = kDefTrkNote;
  velocity_ = kDefTrkVelocity;
  flags_ = 0;
}

#define FIX_DEF(name, Name) \
  if (name < kMin##Name || name > kMax##Name) { \
    name = kDef##Name; \
  }

void Trk::Validate() {
  FIX_DEF(fill_, TrkFill);
  FIX_DEF(size_, TrkSize);
  FIX_DEF(note_, TrkNote);
  FIX_DEF(velocity_, TrkVelocity);
}

#undef FIX_DEF

}  // midigrids
