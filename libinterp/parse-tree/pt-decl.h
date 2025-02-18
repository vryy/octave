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

#if ! defined (octave_pt_decl_h)
#define octave_pt_decl_h 1

#include "octave-config.h"

#include <list>
#include <string>

#include "oct-lvalue.h"
#include "pt-cmd.h"
#include "pt-id.h"
#include "pt-walk.h"

OCTAVE_BEGIN_NAMESPACE(octave)

class symbol_scope;
class tree_evaluator;
class tree_expression;
class tree_identifier;

// List of expressions that make up a declaration statement.

class tree_decl_elt
{
public:

  enum decl_type
  {
    unknown,
    global,
    persistent
  };

  tree_decl_elt (tree_identifier *i, tree_expression *e = nullptr);

  OCTAVE_DISABLE_CONSTRUCT_COPY_MOVE (tree_decl_elt)

  ~tree_decl_elt ();

  filepos beg_pos () const { return m_id->beg_pos (); }
  filepos end_pos () const { return m_expr ? m_expr->end_pos () : m_id->end_pos (); }

  void mark_as_formal_parameter ()
  {
    m_id->mark_as_formal_parameter ();
  }

  bool lvalue_ok () { return m_id->lvalue_ok (); }

  octave_lvalue lvalue (tree_evaluator& tw)
  {
    return m_id->lvalue (tw);
  }

  void mark_global () { m_type = global; }
  bool is_global () const { return m_type == global; }

  void mark_persistent () { m_type = persistent; }
  bool is_persistent () const { return m_type == persistent; }

  tree_identifier * ident () { return m_id; }

  std::string name () const { return m_id->name (); }

  tree_expression * expression () { return m_expr; }

  tree_decl_elt * dup (symbol_scope& scope) const;

  void accept (tree_walker& tw)
  {
    tw.visit_decl_elt (*this);
  }

private:

  decl_type m_type;

  // An identifier to tag with the declared property.
  tree_identifier *m_id;

  // An initializer expression (may be zero);
  tree_expression *m_expr;
};

class tree_decl_init_list : public std::list<tree_decl_elt *>
{
public:

  tree_decl_init_list () { }

  tree_decl_init_list (tree_decl_elt *t) { push_back (t); }

  OCTAVE_DISABLE_COPY_MOVE (tree_decl_init_list)

  ~tree_decl_init_list ()
  {
    while (! empty ())
      {
        auto p = begin ();
        delete *p;
        erase (p);
      }
  }

  filepos beg_pos () const
  {
    if (empty ())
      return filepos ();

    tree_decl_elt *elt = front ();
    return elt->beg_pos ();
  }

  filepos end_pos () const
  {
    if (empty ())
      return filepos ();

    tree_decl_elt *elt = back ();
    return elt->end_pos ();
  }

  void mark_global ()
  {
    for (tree_decl_elt *elt : *this)
      elt->mark_global ();
  }

  void mark_persistent ()
  {
    for (tree_decl_elt *elt : *this)
      elt->mark_persistent ();
  }

  std::list<std::string> variable_names () const
  {
    std::list<std::string> retval;

    for (const tree_decl_elt *elt : *this)
      {
        std::string nm = elt->name ();

        if (! nm.empty ())
          retval.push_back (nm);
      }

    return retval;
  }

  void accept (tree_walker& tw)
  {
    tw.visit_decl_init_list (*this);
  }
};

// Base class for declaration commands -- global, static, etc.

class tree_decl_command : public tree_command
{
public:

  tree_decl_command (const std::string& n, const token& tok, tree_decl_init_list *t);

  OCTAVE_DISABLE_CONSTRUCT_COPY_MOVE (tree_decl_command)

  ~tree_decl_command ();

  filepos beg_pos () const { return m_token.beg_pos (); }
  filepos end_pos () const { return m_init_list->end_pos (); }

  void mark_global ()
  {
    if (m_init_list)
      m_init_list->mark_global ();
  }

  void mark_persistent ()
  {
    if (m_init_list)
      m_init_list->mark_persistent ();
  }

  tree_decl_init_list * initializer_list () { return m_init_list; }

  std::string name () const { return m_cmd_name; }

  void accept (tree_walker& tw)
  {
    tw.visit_decl_command (*this);
  }

private:

  // The name of this command -- global, static, etc.
  std::string m_cmd_name;

  token m_token;

  // The list of variables or initializers in this declaration command.
  tree_decl_init_list *m_init_list;
};

OCTAVE_END_NAMESPACE(octave)

#endif
