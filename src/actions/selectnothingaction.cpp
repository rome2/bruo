////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    selectnothingaction.cpp
///\ingroup bruo
///\brief   Select nothing action implementation.
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
#include "selectnothingaction.h"
#include "../mainframe.h"
#include "../commands/clearselectioncommand.h"

////////////////////////////////////////////////////////////////////////////////
// SelectNothingAction::SelectNothingAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
SelectNothingAction::SelectNothingAction(MainFrame* parent) :
  SelectionAction(QIcon(":/images/edit-select-nothing.png"), tr("Select &nothing"), parent)
{
  setStatusTip(tr("Deselect all"));
  setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_A));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
}

////////////////////////////////////////////////////////////////////////////////
// SelectNothingAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void SelectNothingAction::fired()
{
  // Get current document:
  Document* doc = m_parent->manager()->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->selectionStart() == 0 && doc->selectionLength() == 0)
    return;

  // Create selection command:
  ClearSelectionCommand* cmd = new ClearSelectionCommand(doc);
  doc->undoStack()->push(cmd);
}

///////////////////////////////// End of File //////////////////////////////////
