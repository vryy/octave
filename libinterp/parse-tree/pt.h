////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 1996-2024 The Octave Project Developers
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

#if ! defined (octave_pt_h)
#define octave_pt_h 1

#include "octave-config.h"

#include <string>

#include <iosfwd>

class octave_function;

OCTAVE_BEGIN_NAMESPACE(octave)

class comment_list;
class filepos;
class tree_evaluator;
class tree_walker;

// Base class for the parse tree.

class tree
{
public:

  tree () : m_bp_cond (nullptr) { }

  OCTAVE_DISABLE_COPY_MOVE (tree)

  virtual ~tree () = default;

  virtual int line () const;
  virtual int column () const;

  virtual filepos beg_pos () const = 0;
  virtual filepos end_pos () const = 0;

  // FIXME: maybe make this a pure virtual function?
  virtual comment_list leading_comments () const;

  virtual void set_breakpoint (const std::string& condition)
  {
    if (m_bp_cond)
      *m_bp_cond = condition;
    else
      m_bp_cond = new std::string (condition);
  }

  virtual void delete_breakpoint ()
  {
    delete m_bp_cond;

    m_bp_cond = nullptr;
  }

  bool meets_bp_condition (tree_evaluator& tw) const;

  bool is_breakpoint () const
  {
    return m_bp_cond;
  }

  bool is_active_breakpoint (tree_evaluator& tw) const
  {
    return m_bp_cond && meets_bp_condition (tw);
  }

  // breakpoint condition, or "0" (i.e., "false") if no breakpoint.
  // To distinguish "0" from a disabled breakpoint, test "is_breakpoint" too.
  const std::string bp_cond () const
  {
    return m_bp_cond ? *m_bp_cond : "0";
  }

  std::string str_print_code ();

  virtual void accept (tree_walker& tw) = 0;

private:

  // NULL if no breakpoint, or a breakpoint condition if there is one.
  std::string *m_bp_cond;
};

OCTAVE_END_NAMESPACE(octave)

#endif
