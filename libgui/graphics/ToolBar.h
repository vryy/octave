////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2011-2024 The Octave Project Developers
//
// See the file COPYRIGHT.md in the top-level directory of this
// distribution or <https://octave.org/copyright/>.
//
// This file is part of Octave.
//
// Octave is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Octave is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Octave; see the file COPYING.  If not, see
// <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////

#if ! defined (octave_ToolBar_h)
#define octave_ToolBar_h 1

#include "Object.h"

class QAction;
class QToolBar;

OCTAVE_BEGIN_NAMESPACE(octave)

class interpreter;

class Figure;

class ToolBar : public Object
{
  Q_OBJECT

public:
  ToolBar (octave::interpreter& interp,
           const graphics_object& go, QToolBar *bar);
  ~ToolBar ();

  static ToolBar *
  create (octave::interpreter& interp,
          const graphics_object& go);

  Container * innerContainer () { return nullptr; }

  bool eventFilter (QObject *watched, QEvent *event);

protected:
  void update (int pId);
  void beingDeleted ();

private Q_SLOTS:
  void hideEmpty ();

private:
  QAction *m_empty;
  Figure *m_figure;
};

OCTAVE_END_NAMESPACE(octave)

#endif
