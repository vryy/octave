// Template array classes
/*

Copyright (C) 1996, 1997, 1999, 2000, 2002, 2003, 2004, 2005, 2007,
              2008, 2009 John W. Eaton
Copyright (C) 2010 VZLU Prague

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

#include <cassert>

#include <iostream>

#include <algorithm>

#include "DiagArray2.h"

#include "lo-error.h"

template <class T>
DiagArray2<T>::DiagArray2 (const Array<T>& a, octave_idx_type r, octave_idx_type c)
  : Array<T> (a.as_column ()), d1 (r), d2 (c)
{
  octave_idx_type rcmin = std::min (r, c);
  if (rcmin != a.length ())
      Array<T>::resize (rcmin, 1);
}

template <class T>
Array<T>
DiagArray2<T>::diag (octave_idx_type k) const
{
  Array<T> d;

  if (k == 0)
    // The main diagonal is shallow-copied.
    d = *this;
  else if (k > 0 && k < cols ())
    d = Array<T> (std::min (cols () - k, rows ()), 1, T ());
  else if (k < 0 && -k < rows ())
    d = Array<T> (std::min (rows () + k, cols ()), 1, T ());
  else
    (*current_liboctave_error_handler)
      ("diag: requested diagonal out of range");

  return d;
}

template <class T>
DiagArray2<T>
DiagArray2<T>::transpose (void) const
{
  return DiagArray2<T> (*this, d2, d1);
}

template <class T>
DiagArray2<T>
DiagArray2<T>::hermitian (T (* fcn) (const T&)) const
{
  return DiagArray2<T> (Array<T>::template map<T> (fcn), d2, d1);
}

// A two-dimensional array with diagonal elements only.

template <class T>
T
DiagArray2<T>::checkelem (octave_idx_type r, octave_idx_type c) const
{
  if (r < 0 || r >= dim1 ())
    gripe_index_out_of_range (2, 1, r+1, dim1 ());
  if (c < 0 || c >= dim2 ())
    gripe_index_out_of_range (2, 2, c+1, dim2 ());

  return elem (r, c);
}

template <class T>
void
DiagArray2<T>::resize (octave_idx_type r, octave_idx_type c,
                       const T& rfv)
{
  if (r < 0 || c < 0)
    {
      (*current_liboctave_error_handler) ("can't resize to negative dimensions");
      return;
    }

  if (r != dim1 () || c != dim2 ())
    {
      Array<T>::resize (std::min (r, c), 1, rfv);
      d1 = r; d2 = c;
    }
}

template <class T>
Array<T> DiagArray2<T>::array_value (void) const
{
  Array<T> result (dim1 (), dim2 ());
  for (octave_idx_type i = 0, len = length (); i < len; i++)
    result.xelem (i, i) = dgelem (i);

  return result;
}