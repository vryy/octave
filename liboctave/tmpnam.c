/* Copyright (C) 1991, 1993 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with the GNU C Library; see the file COPYING.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef HAVE_TMPNAM

#include <stddef.h>
#include <stdio.h>
#include <string.h>

extern char *__stdio_gen_tempname (const char *dir, const char *pfx,
				   int dir_search, size_t *lenptr,
				   FILE **streamptr);

/* Generate a unique filename in P_tmpdir.  */
char *
tmpnam (register char *s)
{
  register char *t = __stdio_gen_tempname((const char *) NULL,
					  (const char *) NULL, 0,
					  (size_t *) NULL, (FILE **) NULL);

  if (t == NULL)
    return NULL;

  if (s != NULL)
    (void) strcpy(s, t);
  else
    s = t;

  return s;
}

#endif
