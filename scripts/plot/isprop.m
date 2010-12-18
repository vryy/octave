## Copyright (C) 2010 Ben Abbott
##
## This file is part of Octave.
##
## Octave is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or (at
## your option) any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn  {Function File} {@var{res} =} isprop (@var{h}, @var{prop})
## Determines if @var{prop} is a property of the object with handle, @var{h}.
## @seealso{get, set}
## @end deftypefn

## Author: Ben Abbott  <bpabbott@mac.com>

function res = isprop (h, prop)
  ## Check input
  if (nargin < 1 || nargin > 2)
    print_usage ();
  endif
 
  if (! ishandle (h))
    error ("isprop: first input argument must be a handle");
  elseif (! ischar (prop))
    error ("isprop: second input argument must be string");
  endif

  res = true;
  try
    v = get (h, prop);
  catch
    res = false;
  end_try_catch
endfunction

%!assert (isprop (0, "foobar"), false)

%!assert (isprop (0, "screenpixelsperinch"), true)
