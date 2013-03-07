////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    selectcommand.h
///\ingroup bruo
///\brief   Selection change command.
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
#include "selectcommand.h"

////////////////////////////////////////////////////////////////////////////////
// SelectCommand::SelectCommand()
////////////////////////////////////////////////////////////////////////////////
///\brief Initialization constructor of this class.
///\param [in] doc:       The target document.
///\param [in] selStart:  New selection start.
///\param [in] selLength: New selection length.
///\param [in] parent:    Parent undo item.
////////////////////////////////////////////////////////////////////////////////
SelectCommand::SelectCommand(Document* doc, qint64 selStart, qint64 selLength, QUndoCommand* parent) :
  AppUndoCommand(doc, parent),
  m_newSelStart(selStart),
  m_newSelLength(selLength)
{
  // Set description:
  setText(QApplication::translate("SelectCommand", "change selection"));

  // Save current selection:
  m_oldSelStart  = document()->selectionStart();
  m_oldSelLength = document()->selectionLength();
}

////////////////////////////////////////////////////////////////////////////////
// SelectCommand::~SelectCommand()
////////////////////////////////////////////////////////////////////////////////
///\brief Destructor of this class.
////////////////////////////////////////////////////////////////////////////////
SelectCommand::~SelectCommand()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// SelectCommand::undo()
////////////////////////////////////////////////////////////////////////////////
///\brief Reverts a change to the document.
////////////////////////////////////////////////////////////////////////////////
void SelectCommand::undo()
{
  // Return to old selection:
  document()->setSelection(m_oldSelStart, m_oldSelLength);

  // Update listeners:
  document()->emitSelectionChanged();
}

////////////////////////////////////////////////////////////////////////////////
// SelectCommand::redo()
////////////////////////////////////////////////////////////////////////////////
///\brief Applies a change to the document.
////////////////////////////////////////////////////////////////////////////////
void SelectCommand::redo()
{
  // Set new selection:
  document()->setSelection(m_newSelStart, m_newSelLength);

  // Update listeners:
  document()->emitSelectionChanged();
}

///////////////////////////////// End of File //////////////////////////////////

