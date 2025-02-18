#! /bin/sh

########################################################################
##
## Copyright (C) 2016-2024 The Octave Project Developers
##
## See the file COPYRIGHT.md in the top-level directory of this
## distribution or <https://octave.org/copyright/>.
##
## This file is part of Octave.
##
## Octave is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <https://www.gnu.org/licenses/>.
##
########################################################################

: ${SED=@SED@}

AWK="@AWK@"
FIND="@FIND@"
SED="@SED@"
ADDRESS_SANITIZER_ENABLED="@ADDRESS_SANITIZER_ENABLED@"
ADDRESS_SANITIZER_OPTIONS="@ADDRESS_SANITIZER_OPTIONS@"
abs_top_srcdir="@abs_top_srcdir@"
abs_top_builddir="@abs_top_builddir@"
host_os="@host_os@"

$SED \
  -e "s|%AWK%|${AWK}|g" \
  -e "s|%FIND%|${FIND}|g" \
  -e "s|%SED%|${SED}|g" \
  -e "s|%ADDRESS_SANITIZER_ENABLED%|${ADDRESS_SANITIZER_ENABLED}|g" \
  -e "s|%ADDRESS_SANITIZER_OPTIONS%|${ADDRESS_SANITIZER_OPTIONS}|g" \
  -e "s|%abs_top_srcdir%|${abs_top_srcdir}|" \
  -e "s|%builddir%|${abs_top_builddir}|" \
  -e "s|%host_os%|${host_os}|"
