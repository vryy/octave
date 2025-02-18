#!/usr/bin/perl -w

########################################################################
##
## Copyright (C) 2010-2024 The Octave Project Developers
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

################################################################################
# File   : add-to-aspell-dict.pl
# Purpose: Merges a file of new words into an existing dictionary file.
#          The resulting file is uniquified and sorted before being written out.
# Usage  : add-to-aspell-dict.pl <filename-of-new-words>
# Documentation: see README in doccheck directory
################################################################################
# Initialize variables
# Private Octave dictionary for aspell
$octdict_fname = './doccheck/aspell-octave.en.pws';

################################################################################
# Parse command line arguments
unless (@ARGV == 1)
{
  die "USAGE: add-to-aspell-dict.pl <filename-of-new-words>\n";
}

$new_words_fname = shift(@ARGV);
if (! -r $new_words_fname)
{
  die "Unable to read input file: $new_words_fname\n";
}

################################################################################
# Add new words to a dictionary database
open (FH, "<$new_words_fname")
  or die "Unable to open file: $new_words_fname\n";
while (<FH>) { $dict_db{$_} = 1; }
close (FH);

# Add words from existing dictionary to dictionary database
open (FH, "<$octdict_fname")
  or die "Unable to open Octave dictionary: $octdict_fname\n";
$header = <FH>;
while (<FH>) { $dict_db{$_} = 1; }
close (FH);

# Remove old dictionary file and write out new one
unlink ($octdict_fname)
  or die "Unable to delete Octave dictionary: $octdict_fname\n";
open (FH, ">$octdict_fname")
  or die "Unable to open file for writing: $octdict_fname\n";
print FH $header;
print FH sort { uc($a) cmp uc ($b) } keys(%dict_db);
close (FH);
