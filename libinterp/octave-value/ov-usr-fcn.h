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

#if ! defined (octave_ov_usr_fcn_h)
#define octave_ov_usr_fcn_h 1

#include "octave-config.h"

#include <string>

#include "comment-list.h"
#include "filepos.h"
#include "ovl.h"
#include "ov-fcn.h"
#include "ov-typeinfo.h"
#include "symscope.h"
#include "token.h"
#include "unwind-prot.h"

class string_vector;

class octave_value;

OCTAVE_BEGIN_NAMESPACE(octave)

class filepos;
class file_info;
class stack_frame;
class tree_identifier;
class tree_parameter_list;
class tree_statement_list;
class tree_evaluator;
class tree_expression;
class tree_walker;

OCTAVE_END_NAMESPACE(octave)

class octave_user_code : public octave_function
{
protected:

  octave_user_code (const std::string& fnm = "", const std::string& nm = "",
                    const octave::symbol_scope& scope = octave::symbol_scope::anonymous (),
                    octave::tree_statement_list *cmds = nullptr,
                    const std::string& ds = "")
    : octave_function (nm, ds), m_scope (scope), m_file_name (fnm),
      m_t_parsed (static_cast<OCTAVE_TIME_T> (0)),
      m_t_checked (static_cast<OCTAVE_TIME_T> (0)),
      m_file_info (nullptr), m_cmd_list (cmds)
  {
    if (m_scope)
      m_scope.set_user_code (this);
  }

public:

  OCTAVE_DISABLE_COPY_MOVE (octave_user_code)

  ~octave_user_code ();

  bool is_user_code () const { return true; }

  octave::filepos beg_pos () const;
  octave::filepos end_pos () const;

  std::string get_code_line (std::size_t line);

  std::deque<std::string> get_code_lines (std::size_t line,
                                          std::size_t num_lines);

  void cache_function_text (const std::string& text,
                            const octave::sys::time& timestamp);

  octave::symbol_scope scope () { return m_scope; }

  std::size_t scope_num_symbols () { return m_scope.num_symbols (); }

  void stash_fcn_file_name (const std::string& nm) { m_file_name = nm; }

  void mark_fcn_file_up_to_date (const octave::sys::time& t)
  { m_t_checked = t; }

  void stash_fcn_file_time (const octave::sys::time& t)
  {
    m_t_parsed = t;
    mark_fcn_file_up_to_date (t);
  }

  std::string fcn_file_name () const { return m_file_name; }

  octave::sys::time time_parsed () const { return m_t_parsed; }

  octave::sys::time time_checked () const { return m_t_checked; }

  virtual octave_value find_subfunction (const std::string&) const
  {
    return octave_value ();
  }

  virtual std::map<std::string, octave_value> subfunctions () const;

  octave::tree_statement_list * body () { return m_cmd_list; }

  octave_value dump () const;

protected:

  void get_file_info ();

  // Our symbol table scope.
  octave::symbol_scope m_scope;

  // The name of the file we parsed.
  std::string m_file_name;

  // The time the file was parsed.
  octave::sys::time m_t_parsed;

  // The time the file was last checked to see if it needs to be
  // parsed again.
  octave::sys::time m_t_checked;

  // Cached text of function or script code with line offsets
  // calculated.
  octave::file_info *m_file_info;

  // The list of commands that make up the body of this function.
  octave::tree_statement_list *m_cmd_list;
};

// Scripts.

class octave_user_script : public octave_user_code
{
public:

  octave_user_script ();

  octave_user_script (const std::string& fnm, const std::string& nm,
                      const octave::symbol_scope& scope = octave::symbol_scope::anonymous (),
                      octave::tree_statement_list *cmds = nullptr,
                      const std::string& ds = "");

  octave_user_script (const std::string& fnm, const std::string& nm,
                      const octave::symbol_scope& scope = octave::symbol_scope::anonymous (),
                      const std::string& ds = "");

  OCTAVE_DISABLE_COPY_MOVE (octave_user_script)

  ~octave_user_script () = default;

  octave_function * function_value (bool = false) { return this; }

  octave_user_script * user_script_value (bool = false) { return this; }

  octave_user_code * user_code_value (bool = false) { return this; }

  // Scripts and user functions are both considered "scripts" because
  // they are written in Octave's scripting language.

  bool is_user_script () const { return true; }

  // We must overload the call method so that we call the proper
  // push_stack_frame method, which is overloaded for pointers to
  // octave_function, octave_user_function, and octave_user_script
  // objects.

  octave_value_list
  call (octave::tree_evaluator& tw, int nargout = 0,
        const octave_value_list& args = octave_value_list ());

  octave_value_list
  execute (octave::tree_evaluator& tw, int nargout = 0,
           const octave_value_list& args = octave_value_list ());

  void accept (octave::tree_walker& tw);

private:

  DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA_API (OCTINTERP_API)
};

// User-defined functions.

class octave_user_function : public octave_user_code
{
public:

  octave_user_function (const octave::symbol_scope& scope = octave::symbol_scope::anonymous (), octave::tree_identifier *id = nullptr,
                        octave::tree_parameter_list *pl = nullptr, octave::tree_parameter_list *rl = nullptr, octave::tree_statement_list *cl = nullptr);

  OCTAVE_DISABLE_COPY_MOVE (octave_user_function)

  ~octave_user_function ();

  // Declared calling form, generated from the parse tree.
  std::string signature () const;

  octave_function * function_value (bool = false) { return this; }

  octave_user_function * user_function_value (bool = false) { return this; }

  octave_user_code * user_code_value (bool = false) { return this; }

  octave_user_function * define_param_list (octave::tree_parameter_list *t);

  octave_user_function * define_ret_list (octave::tree_parameter_list *t);

  void set_fcn_tok (const octave::token& fcn_tok) { m_fcn_tok = fcn_tok; }
  void set_eq_tok (const octave::token& eq_tok) { m_eq_tok = eq_tok; }

  void attach_trailing_comments (const octave::comment_list& lst);

  octave::filepos beg_pos () const { return m_fcn_tok.beg_pos(); }
  // The end_pos function is defined in the octave_user_code class.

  void maybe_relocate_end ();

  void stash_parent_fcn_scope (const octave::symbol_scope& ps);

  std::string profiler_name () const;

  std::string parent_fcn_name () const
  {
    octave::symbol_scope pscope = parent_fcn_scope ();

    return pscope.fcn_name ();
  }

  octave::symbol_scope parent_fcn_scope () const
  {
    return m_scope.parent_scope ();
  }

  std::list<std::string> parent_fcn_names () const
  {
    return m_scope.parent_fcn_names ();
  }

  void mark_as_system_fcn_file ();

  bool is_system_fcn_file () const { return m_system_fcn_file; }

  bool is_user_function () const { return true; }

  void erase_subfunctions ();

  bool takes_varargs () const;

  bool takes_var_return () const;

  void mark_as_private_function (const std::string& cname = "");

  void lock_subfunctions ();

  void unlock_subfunctions ();

  std::map<std::string, octave_value> subfunctions () const;

  octave_value find_subfunction (const std::string& subfuns) const;

  bool has_subfunctions () const;

  void stash_subfunction_names (const std::list<std::string>& names);

  std::list<std::string> subfunction_names () const;

  octave_value_list all_va_args (const octave_value_list& args);

  void stash_function_name (const std::string& s) { m_name = s; }

  void mark_as_subfunction () { m_subfunction = true; }

  bool is_subfunction () const { return m_subfunction; }

  void mark_as_inline_function () { m_inline_function = true; }

  bool is_inline_function () const { return m_inline_function; }

  void mark_as_anonymous_function () { m_anonymous_function = true; }

  bool is_anonymous_function () const { return m_anonymous_function; }

  bool is_anonymous_function_of_class
  (const std::string& cname = "") const
  {
    return m_anonymous_function
           ? (cname.empty ()
              ? (! dispatch_class ().empty ())
              : cname == dispatch_class ())
           : false;
  }

  // If we are a special expression, then the function body consists of exactly
  // one expression.  The expression's result is the return value of the
  // function.
  bool is_special_expr () const
  {
    return is_inline_function () || is_anonymous_function ();
  }

  void mark_as_nested_function () { m_nested_function = true; }

  bool is_nested_function () const { return m_nested_function; }

  bool is_parent_function () const { return m_scope.is_parent (); }

  void mark_as_legacy_constructor () { m_class_constructor = legacy; }

  bool is_legacy_constructor (const std::string& cname = "") const
  {
    return (m_class_constructor == legacy
            ? (cname.empty () ? true : cname == dispatch_class ()) : false);
  }

  void mark_as_classdef_constructor () { m_class_constructor = classdef; }

  bool is_classdef_constructor (const std::string& cname = "") const
  {
    return (m_class_constructor == classdef
            ? (cname.empty () ? true : cname == dispatch_class ()) : false);
  }

  void mark_as_legacy_method () { m_class_method = legacy; }

  bool is_legacy_method (const std::string& cname = "") const
  {
    return (m_class_method == legacy
            ? (cname.empty () ? true : cname == dispatch_class ()) : false);
  }

  void mark_as_classdef_method () { m_class_method = classdef; }

  bool is_classdef_method (const std::string& cname = "") const
  {
    return (m_class_method == classdef
            ? (cname.empty () ? true : cname == dispatch_class ()) : false);
  }

  // We must overload the call method so that we call the proper
  // push_stack_frame method, which is overloaded for pointers to
  // octave_function, octave_user_function, and octave_user_script
  // objects.

  octave_value_list
  call (octave::tree_evaluator& tw, int nargout = 0,
        const octave_value_list& args = octave_value_list ());

  octave_value_list
  execute (octave::tree_evaluator& tw, int nargout = 0,
           const octave_value_list& args = octave_value_list ());

  octave::tree_parameter_list * parameter_list () { return m_param_list; }

  octave::tree_parameter_list * return_list () { return m_ret_list; }

  octave::comment_list leading_comments () const { return m_fcn_tok.leading_comments (); }
  octave::comment_list trailing_comments () const;

  // If is_special_expr is true, retrieve the sigular expression that forms the
  // body.  May be null (even if is_special_expr is true).
  octave::tree_expression * special_expr ();

  bool subsasgn_optimization_ok ();

  void accept (octave::tree_walker& tw);

  octave_value dump () const;

private:

  enum class_method_type
  {
    none,
    legacy,
    classdef
  };

  std::string ctor_type_str () const;
  std::string method_type_str () const;

  // Name of this function.
  octave::tree_identifier *m_id;

  // List of arguments for this function.  These are local variables.
  octave::tree_parameter_list *m_param_list;

  // List of parameters we return.  These are also local variables in
  // this function.
  octave::tree_parameter_list *m_ret_list;

  // We don't keep track of an end token separately because functions
  // may still be defined without an explicit END.  If there is an
  // explicit END, the final statement will contain it.

  octave::token m_fcn_tok;
  octave::token m_eq_tok;

  // True if this function came from a file that is considered to be a
  // system function.  This affects whether we check the time stamp
  // on the file to see if it has changed.
  bool m_system_fcn_file {false};

  // The number of arguments that have names.
  int m_num_named_args;

  // TRUE means this is a m_subfunction of a primary function.
  bool m_subfunction {false};

  // TRUE means this is an inline function.
  bool m_inline_function {false};

  // TRUE means this is an anonymous function.
  bool m_anonymous_function {false};

  // TRUE means this is a nested function.
  bool m_nested_function {false};

  // Enum describing whether this function is the constructor for class object.
  class_method_type m_class_constructor {none};

  // Enum describing whether this function is a method for a class.
  class_method_type m_class_method {none};

  void maybe_relocate_end_internal ();

  void print_code_function_header (const std::string& prefix);

  void print_code_function_trailer (const std::string& prefix);

  // XXX FIXME (public)
public:

  void restore_warning_states ();

  DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA_API (OCTINTERP_API)
};

#endif
