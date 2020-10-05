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
// Onboard EEPROM interface.

#ifndef MIDIGRIDS_EEMEM_H_
#define MIDIGRIDS_EEMEM_H_

#include "midigrids/midigrids.h"

namespace midigrids {

class Eemem {
 public:
  Eemem() {}
  static void Init();
  static void Reset();

  static uint8_t slot();
  static void set_slot(uint8_t slot);

 private:

  DISALLOW_COPY_AND_ASSIGN(Eemem);
};

extern Eemem eemem;

}  // namespace midigrids

#endif // MIDIGRIDS_EEMEM_H_
