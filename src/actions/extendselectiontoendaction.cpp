////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    extendselectiontoendaction.cpp
///\ingroup bruo
///\brief   Extend selection to end action implementation.
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
#include "extendselectiontoendaction.h"
#include "../mainframe.h"
#include "../commands/selectcommand.h"

////////////////////////////////////////////////////////////////////////////////
// ExtendSelectionToEndAction::ExtendSelectionToEndAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
ExtendSelectionToEndAction::ExtendSelectionToEndAction(MainFrame* parent) :
  SelectionAction(QIcon(":/images/edit-select-to-end.png"), tr("Ex&tend to end"), parent)
{
  setStatusTip(tr("Extend selection to the end of the document"));
  setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Right));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
}

////////////////////////////////////////////////////////////////////////////////
// ExtendSelectionToEndAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void ExtendSelectionToEndAction::fired()
{
  // Get current document:
  Document* doc = m_parent->manager()->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->selectionLength() == doc->sampleCount())
    return;

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, doc->selectionStart(), doc->sampleCount() - doc->selectionStart(), doc->selectedChannel());
  cmd->setText(tr("Extend selection"));
  doc->undoStack()->push(cmd);
}

///////////////////////////////// End of File //////////////////////////////////
