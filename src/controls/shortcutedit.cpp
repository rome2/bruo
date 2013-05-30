////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    shortcutedit.cpp
///\ingroup bruo
///\brief   Shortcut edit control implementation.
///\author  Rolf Meyerhoff (badlantic@gmail.com)
///\version 1.0
/// This file is part of the bruo audio editor.
////////////////////////////////////////////////////////////////////////////////
///\par License:
/// This program is free software: you can redistribute it and/or modify it
/// under the terms of the GNU General Public License as published by the Free
/// Software Foundation, either version 2 of the License, or (at your option)
/// any later version.
///\par
/// This program is distributed in the hope that it will be useful, but WITHOUT
/// ANY WARRANTY; without even  the implied warranty of MERCHANTABILITY or
/// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
/// more details.
///\par
/// You should have received a copy of the GNU General Public License along with
/// this program; see the file COPYING. If not, see http://www.gnu.org/licenses/
/// or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
/// Floor, Boston, MA 02110-1301, USA.
////////////////////////////////////////////////////////////////////////////////
#include "../bruo.h"
#include "shortcutedit.h"

////////////////////////////////////////////////////////////////////////////////
// ShortcutEdit::ShortcutEdit()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the gui.
////////////////////////////////////////////////////////////////////////////////
ShortcutEdit::ShortcutEdit(QWidget* parent) :
  QLineEdit(parent)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// ShortcutEdit::shortcut()
////////////////////////////////////////////////////////////////////////////////
///\brief  Access the current shortcut of this control.
///\return The currently set key sequence (may be empty).
////////////////////////////////////////////////////////////////////////////////
QKeySequence ShortcutEdit::shortcut() const
{
  // Return our shortcut:
  return m_shortcut;
}

////////////////////////////////////////////////////////////////////////////////
// ShortcutEdit::setShortcut()
////////////////////////////////////////////////////////////////////////////////
///\brief Set the current shortcut of this control.
///\param [in] sc: The new key sequence (may be empty).
////////////////////////////////////////////////////////////////////////////////
void ShortcutEdit::setShortcut(QKeySequence sc)
{
  // Set new shortcut:
  m_shortcut = sc;

  // Update display:
  setText(sc.toString());
}

////////////////////////////////////////////////////////////////////////////////
// ShortcutEdit::keyPressEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Reimplemented keypress event handler.
///\param [in] event: Provides details about the event.
////////////////////////////////////////////////////////////////////////////////
void ShortcutEdit::keyPressEvent(QKeyEvent* event)
{
  // Get the key code:
  int val = event->key();

  // Filter out modifier keys and unknowns:
  if (val == Qt::Key_unknown || val == Qt::Key_Shift || val == Qt::Key_Control || val == Qt::Key_Alt)
    val = 0;

  // Clear display?
  if (val == Qt::Key_Backspace)
    val = 0;

  // Add modifiers:
  int modifiers = event->modifiers();
  if(modifiers & Qt::ControlModifier)
    val += Qt::CTRL;
  if(modifiers & Qt::ShiftModifier)
    val += Qt::SHIFT;
  if(modifiers & Qt::AltModifier)
    val += Qt::ALT;

  // Create new key sequence:
  m_shortcut = QKeySequence(val);

  // Update display:
  setText(m_shortcut.toString());
}

///////////////////////////////// End of File //////////////////////////////////
