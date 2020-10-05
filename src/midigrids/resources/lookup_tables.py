#!/usr/bin/python2.5
#
# Copyright 2009 Olivier Gillet.
#
# Author: Olivier Gillet (ol.gillet@gmail.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Hardware Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# -----------------------------------------------------------------------------
#
# Lookup table definitions.

import numpy

lookup_tables = []
lookup_tables_8 = []
drum_map_nodes = []

"""----------------------------------------------------------------------------
Drum map nodes
----------------------------------------------------------------------------"""

nodes = [
[
  255, 0, 0, 0, 0, 0, 145, 0,
  0, 0, 0, 0, 218, 0, 0, 0,
  72, 0, 36, 0, 182, 0, 0, 0,
  109, 0, 0, 0, 72, 0, 0, 0,
  36, 0, 109, 0, 0, 0, 8, 0,
  255, 0, 0, 0, 0, 0, 72, 0,
  0, 0, 182, 0, 0, 0, 36, 0,
  218, 0, 0, 0, 145, 0, 0, 0,
  170, 0, 113, 0, 255, 0, 56, 0,
  170, 0, 141, 0, 198, 0, 56, 0,
  170, 0, 113, 0, 226, 0, 28, 0,
  170, 0, 113, 0, 198, 0, 85, 0
],
[
  229, 0, 25, 0, 102, 0, 25, 0,
  204, 0, 25, 0, 76, 0, 8, 0,
  255, 0, 8, 0, 51, 0, 25, 0,
  178, 0, 25, 0, 153, 0, 127, 0,
  28, 0, 198, 0, 56, 0, 56, 0,
  226, 0, 28, 0, 141, 0, 28, 0,
  28, 0, 170, 0, 28, 0, 28, 0,
  255, 0, 113, 0, 85, 0, 85, 0,
  159, 0, 159, 0, 255, 0, 63, 0,
  159, 0, 159, 0, 191, 0, 31, 0,
  159, 0, 127, 0, 255, 0, 31, 0,
  159, 0, 127, 0, 223, 0, 95, 0
],
[
  255, 0, 0, 0, 127, 0, 0, 0,
  0, 0, 102, 0, 0, 0, 229, 0,
  0, 0, 178, 0, 204, 0, 0, 0,
  76, 0, 51, 0, 153, 0, 25, 0,
  0, 0, 127, 0, 0, 0, 0, 0,
  255, 0, 191, 0, 31, 0, 63, 0,
  0, 0, 95, 0, 0, 0, 0, 0,
  223, 0, 0, 0, 31, 0, 159, 0,
  255, 0, 85, 0, 148, 0, 85, 0,
  127, 0, 85, 0, 106, 0, 63, 0,
  212, 0, 170, 0, 191, 0, 170, 0,
  85, 0, 42, 0, 233, 0, 21, 0
],
[
  255, 0, 212, 0, 63, 0, 0, 0,
  106, 0, 148, 0, 85, 0, 127, 0,
  191, 0, 21, 0, 233, 0, 0, 0,
  21, 0, 170, 0, 0, 0, 42, 0,
  0, 0, 0, 0, 141, 0, 113, 0,
  255, 0, 198, 0, 0, 0, 56, 0,
  0, 0, 85, 0, 56, 0, 28, 0,
  226, 0, 28, 0, 170, 0, 56, 0,
  255, 0, 231, 0, 255, 0, 208, 0,
  139, 0, 92, 0, 115, 0, 92, 0,
  185, 0, 69, 0, 46, 0, 46, 0,
  162, 0, 23, 0, 208, 0, 46, 0
],
[
  255, 0, 31, 0, 63, 0, 63, 0,
  127, 0, 95, 0, 191, 0, 63, 0,
  223, 0, 31, 0, 159, 0, 63, 0,
  31, 0, 63, 0, 95, 0, 31, 0,
  8, 0, 0, 0, 95, 0, 63, 0,
  255, 0, 0, 0, 127, 0, 0, 0,
  8, 0, 0, 0, 159, 0, 63, 0,
  255, 0, 223, 0, 191, 0, 31, 0,
  76, 0, 25, 0, 255, 0, 127, 0,
  153, 0, 51, 0, 204, 0, 102, 0,
  76, 0, 51, 0, 229, 0, 127, 0,
  153, 0, 51, 0, 178, 0, 102, 0
],
[
  255, 0, 51, 0, 25, 0, 76, 0,
  0, 0, 0, 0, 102, 0, 0, 0,
  204, 0, 229, 0, 0, 0, 178, 0,
  0, 0, 153, 0, 127, 0, 8, 0,
  178, 0, 127, 0, 153, 0, 204, 0,
  255, 0, 0, 0, 25, 0, 76, 0,
  102, 0, 51, 0, 0, 0, 0, 0,
  229, 0, 25, 0, 25, 0, 204, 0,
  178, 0, 102, 0, 255, 0, 76, 0,
  127, 0, 76, 0, 229, 0, 76, 0,
  153, 0, 102, 0, 255, 0, 25, 0,
  127, 0, 51, 0, 204, 0, 51, 0
],
[
  255, 0, 0, 0, 223, 0, 0, 0,
  31, 0, 8, 0, 127, 0, 0, 0,
  95, 0, 0, 0, 159, 0, 0, 0,
  95, 0, 63, 0, 191, 0, 0, 0,
  51, 0, 204, 0, 0, 0, 102, 0,
  255, 0, 127, 0, 8, 0, 178, 0,
  25, 0, 229, 0, 0, 0, 76, 0,
  204, 0, 153, 0, 51, 0, 25, 0,
  255, 0, 226, 0, 255, 0, 255, 0,
  198, 0, 28, 0, 141, 0, 56, 0,
  170, 0, 56, 0, 85, 0, 28, 0,
  170, 0, 28, 0, 113, 0, 56, 0
],
[
  223, 0, 0, 0, 63, 0, 0, 0,
  95, 0, 0, 0, 223, 0, 31, 0,
  255, 0, 0, 0, 159, 0, 0, 0,
  127, 0, 31, 0, 191, 0, 31, 0,
  0, 0, 0, 0, 109, 0, 0, 0,
  218, 0, 0, 0, 182, 0, 72, 0,
  8, 0, 36, 0, 145, 0, 36, 0,
  255, 0, 8, 0, 182, 0, 72, 0,
  255, 0, 72, 0, 218, 0, 36, 0,
  218, 0, 0, 0, 145, 0, 0, 0,
  255, 0, 36, 0, 182, 0, 36, 0,
  182, 0, 0, 0, 109, 0, 0, 0
],
[
  255, 0, 0, 0, 218, 0, 0, 0,
  36, 0, 0, 0, 218, 0, 0, 0,
  182, 0, 109, 0, 255, 0, 0, 0,
  0, 0, 0, 0, 145, 0, 72, 0,
  159, 0, 0, 0, 31, 0, 127, 0,
  255, 0, 31, 0, 0, 0, 95, 0,
  8, 0, 0, 0, 191, 0, 31, 0,
  255, 0, 31, 0, 223, 0, 63, 0,
  255, 0, 31, 0, 63, 0, 31, 0,
  95, 0, 31, 0, 63, 0, 127, 0,
  159, 0, 31, 0, 63, 0, 31, 0,
  223, 0, 223, 0, 191, 0, 191, 0
],
[
  226, 0, 28, 0, 28, 0, 141, 0,
  8, 0, 8, 0, 255, 0, 8, 0,
  113, 0, 28, 0, 198, 0, 85, 0,
  56, 0, 198, 0, 170, 0, 28, 0,
  8, 0, 95, 0, 8, 0, 8, 0,
  255, 0, 63, 0, 31, 0, 223, 0,
  8, 0, 31, 0, 191, 0, 8, 0,
  255, 0, 127, 0, 127, 0, 159, 0,
  115, 0, 46, 0, 255, 0, 185, 0,
  139, 0, 23, 0, 208, 0, 115, 0,
  231, 0, 69, 0, 255, 0, 162, 0,
  139, 0, 115, 0, 231, 0, 92, 0
],
[
  145, 0, 0, 0, 0, 0, 109, 0,
  0, 0, 0, 0, 255, 0, 109, 0,
  72, 0, 218, 0, 0, 0, 0, 0,
  36, 0, 0, 0, 182, 0, 0, 0,
  0, 0, 127, 0, 159, 0, 127, 0,
  159, 0, 191, 0, 223, 0, 63, 0,
  255, 0, 95, 0, 31, 0, 95, 0,
  31, 0, 8, 0, 63, 0, 8, 0,
  255, 0, 0, 0, 145, 0, 0, 0,
  182, 0, 109, 0, 109, 0, 109, 0,
  218, 0, 0, 0, 72, 0, 0, 0,
  182, 0, 72, 0, 182, 0, 36, 0
],
[
  255, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  255, 0, 0, 0, 218, 0, 72, 36,
  0, 0, 182, 0, 0, 0, 145, 109,
  0, 0, 127, 0, 0, 0, 42, 0,
  212, 0, 0, 212, 0, 0, 212, 0,
  0, 0, 0, 0, 42, 0, 0, 0,
  255, 0, 0, 0, 170, 170, 127, 85,
  145, 0, 109, 109, 218, 109, 72, 0,
  145, 0, 72, 0, 218, 0, 109, 0,
  182, 0, 109, 0, 255, 0, 72, 0,
  182, 109, 36, 109, 255, 109, 109, 0
],
[
  255, 0, 0, 0, 255, 0, 191, 0,
  0, 0, 0, 0, 95, 0, 63, 0,
  31, 0, 0, 0, 223, 0, 223, 0,
  0, 0, 8, 0, 159, 0, 127, 0,
  0, 0, 85, 0, 56, 0, 28, 0,
  255, 0, 28, 0, 0, 0, 226, 0,
  0, 0, 170, 0, 56, 0, 113, 0,
  198, 0, 0, 0, 113, 0, 141, 0,
  255, 0, 42, 0, 233, 0, 63, 0,
  212, 0, 85, 0, 191, 0, 106, 0,
  191, 0, 21, 0, 170, 0, 8, 0,
  170, 0, 127, 0, 148, 0, 148, 0
],
[
  255, 0, 0, 0, 0, 0, 63, 0,
  191, 0, 95, 0, 31, 0, 223, 0,
  255, 0, 63, 0, 95, 0, 63, 0,
  159, 0, 0, 0, 0, 0, 127, 0,
  72, 0, 0, 0, 0, 0, 0, 0,
  255, 0, 0, 0, 0, 0, 0, 0,
  72, 0, 72, 0, 36, 0, 8, 0,
  218, 0, 182, 0, 145, 0, 109, 0,
  255, 0, 162, 0, 231, 0, 162, 0,
  231, 0, 115, 0, 208, 0, 139, 0,
  185, 0, 92, 0, 185, 0, 46, 0,
  162, 0, 69, 0, 162, 0, 23, 0
],
[
  255, 0, 0, 0, 51, 0, 0, 0,
  0, 0, 0, 0, 102, 0, 0, 0,
  204, 0, 0, 0, 153, 0, 0, 0,
  0, 0, 0, 0, 51, 0, 0, 0,
  0, 0, 0, 0, 8, 0, 36, 0,
  255, 0, 0, 0, 182, 0, 8, 0,
  0, 0, 0, 0, 72, 0, 109, 0,
  145, 0, 0, 0, 255, 0, 218, 0,
  212, 0, 8, 0, 170, 0, 0, 0,
  127, 0, 0, 0, 85, 0, 8, 0,
  255, 0, 8, 0, 170, 0, 0, 0,
  127, 0, 0, 0, 42, 0, 8, 0
],
[
  255, 0, 0, 0, 0, 0, 0, 0,
  36, 0, 0, 0, 182, 0, 0, 0,
  218, 0, 0, 0, 0, 0, 0, 0,
  72, 0, 0, 0, 145, 0, 109, 0,
  36, 0, 36, 0, 0, 0, 0, 0,
  255, 0, 0, 0, 182, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 109,
  218, 0, 0, 0, 145, 0, 72, 72,
  255, 0, 28, 0, 226, 0, 56, 0,
  198, 0, 0, 0, 0, 0, 28, 28,
  170, 0, 0, 0, 141, 0, 0, 0,
  113, 0, 0, 0, 85, 85, 85, 85
],
[
  255, 0, 0, 0, 0, 0, 95, 0,
  0, 0, 127, 0, 0, 0, 0, 0,
  223, 0, 95, 0, 63, 0, 31, 0,
  191, 0, 0, 0, 159, 0, 0, 0,
  0, 0, 31, 0, 255, 0, 0, 0,
  0, 0, 95, 0, 223, 0, 0, 0,
  0, 0, 63, 0, 191, 0, 0, 0,
  0, 0, 0, 0, 159, 0, 127, 0,
  141, 0, 28, 0, 28, 0, 28, 0,
  113, 0, 8, 0, 8, 0, 8, 0,
  255, 0, 0, 0, 226, 0, 0, 0,
  198, 0, 56, 0, 170, 0, 85, 0
],
[
  255, 0, 0, 0, 8, 0, 0, 0,
  182, 0, 0, 0, 72, 0, 0, 0,
  218, 0, 0, 0, 36, 0, 0, 0,
  145, 0, 0, 0, 109, 0, 0, 0,
  0, 0, 51, 25, 76, 25, 25, 0,
  153, 0, 0, 0, 127, 102, 178, 0,
  204, 0, 0, 0, 0, 0, 255, 0,
  0, 0, 102, 0, 229, 0, 76, 0,
  113, 0, 0, 0, 141, 0, 85, 0,
  0, 0, 0, 0, 170, 0, 0, 0,
  56, 28, 255, 0, 0, 0, 0, 0,
  198, 0, 0, 0, 226, 0, 0, 0
],
[
  255, 0, 8, 0, 28, 0, 28, 0,
  198, 0, 56, 0, 56, 0, 85, 0,
  255, 0, 85, 0, 113, 0, 113, 0,
  226, 0, 141, 0, 170, 0, 141, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  255, 0, 0, 0, 127, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  63, 0, 0, 0, 191, 0, 0, 0,
  255, 0, 0, 0, 255, 0, 127, 0,
  0, 0, 85, 0, 0, 0, 212, 0,
  0, 0, 212, 0, 42, 0, 170, 0,
  0, 0, 127, 0, 0, 0, 0, 0
],
[
  255, 0, 0, 0, 0, 0, 218, 0,
  182, 0, 0, 0, 0, 0, 145, 0,
  145, 0, 36, 0, 0, 0, 109, 0,
  109, 0, 0, 0, 72, 0, 36, 0,
  0, 0, 0, 0, 109, 0, 8, 0,
  72, 0, 0, 0, 255, 0, 182, 0,
  0, 0, 0, 0, 145, 0, 8, 0,
  36, 0, 8, 0, 218, 0, 182, 0,
  255, 0, 0, 0, 0, 0, 226, 0,
  85, 0, 0, 0, 141, 0, 0, 0,
  0, 0, 0, 0, 170, 0, 56, 0,
  198, 0, 0, 0, 113, 0, 28, 0
],
[
  255, 0, 0, 0, 113, 0, 0, 0,
  198, 0, 56, 0, 85, 0, 28, 0,
  255, 0, 0, 0, 226, 0, 0, 0,
  170, 0, 0, 0, 141, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  255, 0, 145, 0, 109, 0, 218, 0,
  36, 0, 182, 0, 72, 0, 72, 0,
  255, 0, 0, 0, 0, 0, 109, 0,
  36, 0, 36, 0, 145, 0, 0, 0,
  72, 0, 72, 0, 182, 0, 0, 0,
  72, 0, 72, 0, 218, 0, 0, 0,
  109, 0, 109, 0, 255, 0, 0, 0
],
[
  255, 0, 0, 0, 218, 0, 0, 0,
  145, 0, 0, 0, 36, 0, 0, 0,
  218, 0, 0, 0, 36, 0, 0, 0,
  182, 0, 72, 0, 0, 0, 109, 0,
  0, 0, 0, 0, 8, 0, 0, 0,
  255, 0, 85, 0, 212, 0, 42, 0,
  0, 0, 0, 0, 8, 0, 0, 0,
  85, 0, 170, 0, 127, 0, 42, 0,
  109, 0, 109, 0, 255, 0, 0, 0,
  72, 0, 72, 0, 218, 0, 0, 0,
  145, 0, 182, 0, 255, 0, 0, 0,
  36, 0, 36, 0, 218, 0, 8, 0
],
[
  255, 0, 0, 0, 42, 0, 0, 0,
  212, 0, 0, 0, 8, 0, 212, 0,
  170, 0, 0, 0, 85, 0, 0, 0,
  212, 0, 8, 0, 127, 0, 8, 0,
  255, 0, 85, 0, 0, 0, 0, 0,
  226, 0, 85, 0, 0, 0, 198, 0,
  0, 0, 141, 0, 56, 0, 0, 0,
  170, 0, 28, 0, 0, 0, 113, 0,
  113, 0, 56, 0, 255, 0, 0, 0,
  85, 0, 56, 0, 226, 0, 0, 0,
  0, 0, 170, 0, 0, 0, 141, 0,
  28, 0, 28, 0, 198, 0, 28, 0
],
[
  255, 0, 0, 0, 229, 0, 0, 0,
  204, 0, 204, 0, 0, 0, 76, 0,
  178, 0, 153, 0, 51, 0, 178, 0,
  178, 0, 127, 0, 102, 51, 51, 25,
  0, 0, 0, 0, 0, 0, 0, 31,
  0, 0, 0, 0, 255, 0, 0, 31,
  0, 0, 8, 0, 0, 0, 191, 159,
  127, 95, 95, 0, 223, 0, 63, 0,
  255, 0, 255, 0, 204, 204, 204, 204,
  0, 0, 51, 51, 51, 51, 0, 0,
  204, 0, 204, 0, 153, 153, 153, 153,
  153, 0, 0, 0, 102, 102, 102, 102
],
[
  170, 0, 0, 0, 0, 255, 0, 0,
  198, 0, 0, 0, 0, 28, 0, 0,
  141, 0, 0, 0, 0, 226, 0, 0,
  56, 0, 0, 113, 0, 85, 0, 0,
  255, 0, 0, 0, 0, 113, 0, 0,
  85, 0, 0, 0, 0, 226, 0, 0,
  141, 0, 0, 8, 0, 170, 56, 56,
  198, 0, 0, 56, 0, 141, 28, 0,
  255, 0, 0, 0, 0, 191, 0, 0,
  159, 0, 0, 0, 0, 223, 0, 0,
  95, 0, 0, 0, 0, 63, 0, 0,
  127, 0, 0, 0, 0, 31, 0, 0
]
]

for i, p in enumerate(nodes):
  drum_map_nodes.append(('%d' % i, p))


"""----------------------------------------------------------------------------
Euclidean patterns
----------------------------------------------------------------------------"""

def Flatten(l):
  if hasattr(l, 'pop'):
    for item in l:
      for j in Flatten(item):
        yield j
  else:
    yield l


def EuclidianPattern(k, n):
  pattern = [[1]] * k + [[0]] * (n - k)
  while k:
    cut = min(k, len(pattern) - k)
    k, pattern = cut, [pattern[i] + pattern[k + i] for i in xrange(cut)] + \
      pattern[cut:k] + pattern[k + cut:]
  return pattern


table = []
for num_steps in xrange(1, 33):
  for num_notes in xrange(32):
    num_notes = int(round(float(num_notes) / 31 * num_steps))
    bitmask = 0
    for i, bit in enumerate(Flatten(EuclidianPattern(num_notes, num_steps))):
      if bit:
        bitmask |= (1 << i)
    table.append(bitmask)

lookup_tables32 = [('euclidean', table)]