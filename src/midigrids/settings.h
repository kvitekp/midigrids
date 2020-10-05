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

#ifndef MIDIGRIDS_SETTINGS_H_
#define MIDIGRIDS_SETTINGS_H_

namespace midigrids {

enum ClockMode {
  CLOCK_MODE_INTERNAL,
  CLOCK_MODE_EXTERNAL,
};

enum ClockOutput {
  CLOCK_OUTPUT_NEVER,
  CLOCK_OUTPUT_WHILE_RUNNING,
  CLOCK_OUTPUT_ALWAYS,
};

enum Pattern {
  PATTERN_GRIDS,
  PATTERN_EUCLIDEAN,
};

}  // namespace midigrids

#endif  // MIDIGRIDS_SETTINGS_H_
