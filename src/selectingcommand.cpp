////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    selectingcommand.cpp
///\ingroup bruo
///\brief   Continuous selection change command.
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
#include "selectingcommand.h"

////////////////////////////////////////////////////////////////////////////////
// SelectingCommand::SelectingCommand()
////////////////////////////////////////////////////////////////////////////////
///\brief Initialization constructor of this class.
///\param [in] doc:       The target document.
///\param [in] selStart:  New selection start.
///\param [in] selLength: New selection length.
///\param [in] last:      Last command in the chain?
///\param [in] parent:    Parent undo item.
////////////////////////////////////////////////////////////////////////////////
SelectingCommand::SelectingCommand(Document* doc, qint64 selStart, qint64 selLength, bool last, QUndoCommand* parent) :
  AppUndoCommand(doc, parent),
  m_newSelStart(selStart),
  m_newSelLength(selLength),
  m_last(last)
{
  // Set description:
  setText(QApplication::translate("SelectingCommand", "change selection"));

  // Save current selection:
  m_oldSelStart  = document()->selectionStart();
  m_oldSelLength = document()->selectionLength();
}

////////////////////////////////////////////////////////////////////////////////
// SelectingCommand::~SelectingCommand()
////////////////////////////////////////////////////////////////////////////////
///\brief Destructor of this class.
////////////////////////////////////////////////////////////////////////////////
SelectingCommand::~SelectingCommand()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// SelectingCommand::undo()
////////////////////////////////////////////////////////////////////////////////
///\brief Reverts a change to the document.
////////////////////////////////////////////////////////////////////////////////
void SelectingCommand::undo()
{
  // Return to old selection:
  document()->setSelection(m_oldSelStart, m_oldSelLength);

  // Update listeners:
  document()->emitSelectionChanged();
}

////////////////////////////////////////////////////////////////////////////////
// SelectingCommand::redo()
////////////////////////////////////////////////////////////////////////////////
///\brief Applies a change to the document.
////////////////////////////////////////////////////////////////////////////////
void SelectingCommand::redo()
{
  // Set new selection:
  document()->setSelection(m_newSelStart, m_newSelLength);

  // Update listeners:
  if (!m_last)
    document()->emitSelectionChanging();
  else
    document()->emitSelectionChanged();
}

////////////////////////////////////////////////////////////////////////////////
// SelectingCommand::id()
////////////////////////////////////////////////////////////////////////////////
///\brief  Accessor for the id property of this command to the document.
///\return The id of this command.
////////////////////////////////////////////////////////////////////////////////
int SelectingCommand::id() const
{
  // Get new ID:
  static int cmdID = newCommandID();

  // Return our ID:
  return cmdID;
}

////////////////////////////////////////////////////////////////////////////////
// SelectingCommand::mergeWith()
////////////////////////////////////////////////////////////////////////////////
///\brief  Merge a following undo command into this one.
///\param  [in] other: The command to merge.
///\return true if the merge was successful or false otherwise.
////////////////////////////////////////////////////////////////////////////////
bool SelectingCommand::mergeWith(const QUndoCommand *other)
{
  // Make sure other is also an SelectingCommand instance:
  if (other->id() != id())
    return false;

  // If we are the last one in the chain then terminate it:
  if (m_last)
    return false;

  // Store new selection:
  m_newSelStart  = static_cast<const SelectingCommand*>(other)->m_newSelStart;
  m_newSelLength = static_cast<const SelectingCommand*>(other)->m_newSelLength;

  // Store last state:
  m_last = static_cast<const SelectingCommand*>(other)->m_last;

  // Successful merged:
  return true;
}

///////////////////////////////// End of File //////////////////////////////////
