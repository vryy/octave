/*

Copyright (C) 1995, 1996, 1997, 2000, 2004, 2005, 2007 John W. Eaton

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.

*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "oct-inttypes.h"

// Instantiate MArrays of int values.

#include "MArray.h"
#include "MArray.cc"

template class OCTAVE_API MArray<int>;
template class OCTAVE_API MArray<long>;

INSTANTIATE_MARRAY_FRIENDS (int, OCTAVE_API)
INSTANTIATE_MARRAY_FRIENDS (long, OCTAVE_API)

template class OCTAVE_API MArray<octave_int8>;
template class OCTAVE_API MArray<octave_int16>;
template class OCTAVE_API MArray<octave_int32>;
template class OCTAVE_API MArray<octave_int64>;

INSTANTIATE_MARRAY_FRIENDS (octave_int8, OCTAVE_API)
INSTANTIATE_MARRAY_FRIENDS (octave_int16, OCTAVE_API)
INSTANTIATE_MARRAY_FRIENDS (octave_int32, OCTAVE_API)
INSTANTIATE_MARRAY_FRIENDS (octave_int64, OCTAVE_API)

template class OCTAVE_API MArray<octave_uint8>;
template class OCTAVE_API MArray<octave_uint16>;
template class OCTAVE_API MArray<octave_uint32>;
template class OCTAVE_API MArray<octave_uint64>;

INSTANTIATE_MARRAY_FRIENDS (octave_uint8, OCTAVE_API)
INSTANTIATE_MARRAY_FRIENDS (octave_uint16, OCTAVE_API)
INSTANTIATE_MARRAY_FRIENDS (octave_uint32, OCTAVE_API)
INSTANTIATE_MARRAY_FRIENDS (octave_uint64, OCTAVE_API)

#include "MDiagArray2.h"
#include "MDiagArray2.cc"

template class OCTAVE_API MDiagArray2<int>;

INSTANTIATE_MDIAGARRAY2_FRIENDS (int, OCTAVE_API)