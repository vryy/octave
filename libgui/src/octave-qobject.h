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

#if ! defined (octave_octave_qobject_h)
#define octave_octave_qobject_h 1

#include <memory>

#include <QApplication>
#include <QList>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>

#include "interpreter-qobject.h"

OCTAVE_BEGIN_NAMESPACE(octave)

class community_news;
class main_window;
class qt_application;
class qt_interpreter_events;
class release_notes;

//! This class is a simple wrapper around QApplication so that we can
//! reimplement QApplication::notify.  The octave_qapplication object
//! should behave identically to a QApplication object except that it
//! overrides the notify method so we can handle forward Octave
//! execution_exception exceptions from the GUI thread to the
//! interpreter thread.

class octave_qapplication : public QApplication
{
  Q_OBJECT

public:

  octave_qapplication (int& argc, char **argv)
    : QApplication (argc, argv)
  { }

  virtual bool notify (QObject *receiver, QEvent *e) override;

  ~octave_qapplication () { };

Q_SIGNALS:

  void interpreter_event (const fcn_callback& fcn);
  void interpreter_event (const meth_callback& meth);
};

//! Container for windows that may be created from the command line or
//! docked with the main GUI window.  Any of these windows that are
//! created in command line mode will be adopted by the main window if
//! it is opened from the command line.  Any that are undocked from
//! the main window will remain open if control returns to the command
//! line.

class base_qobject;
class documentation_dock_widget;
class file_editor_interface;
class files_dock_widget;
class history_dock_widget;
class terminal_dock_widget;
class variable_editor;
class workspace_model;
class workspace_view;

//! Base class for Octave interfaces that use Qt.  There are two
//! classes derived from this one.  One provides a command-line
//! interface that may use Qt graphics and another provides the
//! full GUI experience.

class base_qobject : public QObject
{
  Q_OBJECT

public:

  // Note: the GUI_APP argument is not needed with the new
  // experimental terminal widget.
  base_qobject (qt_application& app_context, bool gui_app = false);

  ~base_qobject ();

  void config_translators ();

  void start_main_thread ();

  int exec ();

  // The Octave application context.
  qt_application& app_context () { return m_app_context; }

  // The Qt QApplication.
  QApplication * qapplication () { return m_qapplication; };

  // Provided for convenience.  Will be removed once we eliminate the
  // old terminal widget.
  bool experimental_terminal_widget () const;

  // Provided for convenience.
  bool gui_running () const;

  bool have_terminal_window () const
  {
    return ! m_terminal_widget.isNull ();
  }

  main_window * get_main_window ()
  {
    return m_main_window;
  }

  std::shared_ptr<qt_interpreter_events> get_qt_interpreter_events ()
  {
    return m_qt_interpreter_events;
  }

  qt_interpreter_events * qt_link ()
  {
    return m_qt_interpreter_events.get ();
  }

  interpreter_qobject * interpreter_qobj ()
  {
    return m_interpreter_qobj;
  }

  workspace_model * get_workspace_model ()
  {
    return m_workspace_model;
  }

  QPointer<terminal_dock_widget>
  terminal_widget (main_window *mw = nullptr);

  QPointer<documentation_dock_widget>
  documentation_widget (main_window *mw = nullptr);

  QPointer<files_dock_widget>
  file_browser_widget (main_window *mw = nullptr);

  QPointer<history_dock_widget>
  history_widget (main_window *mw = nullptr);

  QPointer<workspace_view>
  workspace_widget (main_window *mw = nullptr);

  // FIXME: The file_editor_interface needs to be a proper generic
  // interface for all editors (internal and external) for this to
  // work properly.
  QPointer<file_editor_interface>
  editor_widget (main_window *mw = nullptr);

  QPointer<variable_editor>
  variable_editor_widget (main_window *mw = nullptr);

  QPointer<community_news> community_news_widget (int serial = -1);

  QPointer<release_notes> release_notes_widget ();

  QThread * main_thread () { return m_main_thread; }

  // Declared virtual so that a derived class may redefine this
  // method.

  virtual bool confirm_shutdown ();

  bool is_gui_app () const { return m_gui_app; }

  template <typename T> void connect_interpreter_events (T *widget)
  {
    connect (widget, qOverload<const fcn_callback&> (&T::interpreter_event),
             this, qOverload<const fcn_callback&> (&base_qobject::interpreter_event));

    connect (widget, qOverload<const meth_callback&> (&T::interpreter_event),
             this, qOverload<const meth_callback&> (&base_qobject::interpreter_event));
  }

public Q_SLOTS:

  void execute_command (const QString& command);

  // Note: START_GUI and CLOSE_GUI don't currently perform any work
  // with the old terminal widget.
  void start_gui (bool gui_app);
  void close_gui ();

  void show_terminal_window ();

  void show_documentation_window (const QString& file);

  void show_file_browser_window ();

  void show_command_history_window ();

  void show_workspace_window ();

  void show_variable_editor_window (const QString& name,
                                    const octave_value& value);

  void handle_variable_editor_update ();

  void show_community_news (int serial);

  void show_release_notes ();

  void interpreter_ready ();

  void interpreter_event (const fcn_callback& fcn);

  void interpreter_event (const meth_callback& meth);

  void interpreter_interrupt ();

  // Note: these currently only work with the new experimental
  // terminal widget.
  void interpreter_pause ();
  void interpreter_stop ();
  void interpreter_resume ();

  void copy_image_to_clipboard (const QString& file, bool remove_file);

protected:

  qt_application& m_app_context;

  // Use these to ensure that argc and argv exist for as long as the
  // QApplication object.

  int m_argc;
  char **m_argv;

  octave_qapplication *m_qapplication;

  QTranslator *m_qt_tr;
  QTranslator *m_gui_tr;
  QTranslator *m_qsci_tr;

  bool m_translators_installed;

  std::shared_ptr<qt_interpreter_events> m_qt_interpreter_events;

  interpreter_qobject *m_interpreter_qobj;

  QThread *m_main_thread;

  bool m_gui_app;

  bool m_interpreter_ready;

  workspace_model *m_workspace_model;

  // Dock widgets that may be used from the command line.  They are
  // adopted by the desktop (main window) if it is also started from
  // the command line.

  QPointer<terminal_dock_widget> m_terminal_widget;

  QPointer<documentation_dock_widget> m_documentation_widget;

  QPointer<files_dock_widget> m_file_browser_widget;

  QPointer<history_dock_widget> m_history_widget;

  QPointer<workspace_view> m_workspace_widget;

  QPointer<file_editor_interface> m_editor_widget;

  QPointer<variable_editor> m_variable_editor_widget;

  QPointer<community_news> m_community_news;

  QPointer<release_notes> m_release_notes;

  main_window *m_main_window;
};

OCTAVE_END_NAMESPACE(octave)

#endif
