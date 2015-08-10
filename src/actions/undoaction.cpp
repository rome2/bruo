////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    undoaction.cpp
///\ingroup bruo
///\brief   Undo action implementation.
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
#include "undoaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// UndoAction::UndoAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
UndoAction::UndoAction(MainFrame* parent) :
  QAction(QIcon(":/images/edit-undo.png"), tr("Undo"), parent),
  m_parent(parent)
{
  setStatusTip(tr("Undo the last action"));
  setShortcuts(QKeySequence::Undo);
  setEnabled(false);
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
  connect(parent->manager(), SIGNAL(documentCreated(Document*)), this, SLOT(documentCreated(Document*)));
  connect(parent->manager(), SIGNAL(activeDocumentChanged()), this, SLOT(stateChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// UndoAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void UndoAction::fired()
{
  // Get current document:
  Document* doc = m_parent->manager()->activeDocument();
  if (doc == 0)
    return;

  // Undo the action:
  doc->undoStack()->undo();
}

////////////////////////////////////////////////////////////////////////////////
// UndoAction::documentCreated()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the document manager's documentCreated signal.
///\remarks This one is called whenever a new document was created. At this
///         stage the document is still empty but this handler is good place to
///         attach per document signal handlers etc.
////////////////////////////////////////////////////////////////////////////////
void UndoAction::documentCreated(Document* doc)
{
  // Connect to the document's undo stack:
  QUndoStack* stack = doc->undoStack();
  connect(stack, SIGNAL(canUndoChanged(bool)),     this, SLOT(canUndoChanged(bool)));
  connect(stack, SIGNAL(undoTextChanged(QString)), this, SLOT(undoTextChanged(QString)));
  connect(doc, SIGNAL(closed()), this, SLOT(stateChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// UndoAction::stateChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for undo state changes.
///\remarks This one is called whenever the undo state was changed.
////////////////////////////////////////////////////////////////////////////////
void UndoAction::stateChanged()
{
  // Get document:
  Document* doc = m_parent->manager()->activeDocument();

  // Update text of the undo action:
  if (doc != 0 && doc->undoStack()->canUndo())
  {
    setText(tr("&Undo ") + doc->undoStack()->undoText());
    setStatusTip(text());
  }
  else
  {
    setText(tr("&Undo"));
    setStatusTip(tr("Undo the last action"));
  }

  // Update enabled state:
  setEnabled(doc != 0 && doc->undoStack()->canUndo());
}

////////////////////////////////////////////////////////////////////////////////
// UndoAction::canUndoChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the current document's canUndo state changed signal.
///\param   [in] state: New state of the property.
///\remarks Updates the state of the related UI elements.
////////////////////////////////////////////////////////////////////////////////
void UndoAction::canUndoChanged(bool state)
{
  // Get sending stack:
  QUndoStack* stack = qobject_cast<QUndoStack*>(sender());
  if (stack == 0)
    return;

  // Check if the message was sent from the current document:
  if (m_parent->manager()->activeDocument() != 0 && m_parent->manager()->activeDocument()->undoStack() != stack)
    return;

  // Update enabled state of the corresponding action:
  setEnabled(state);
}

////////////////////////////////////////////////////////////////////////////////
// UndoAction::undoTextChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the current document's undo text changed signal.
///\param   [in] newString: New text of the property.
///\remarks Updates the state of the related UI elements.
////////////////////////////////////////////////////////////////////////////////
void UndoAction::undoTextChanged(QString newString)
{
  // Get sending stack:
  QUndoStack* stack = qobject_cast<QUndoStack*>(sender());
  if (stack == 0)
    return;

  // Check if the message was sent from the current document:
  if (m_parent->manager()->activeDocument() != 0 && m_parent->manager()->activeDocument()->undoStack() != stack)
    return;

  // Update text of the corresponding action:
  if (stack->canUndo())
  {
    setText(tr("&Undo ") + newString);
    setStatusTip(tr("Undo ") + newString);
  }
  else
  {
    setText(tr("&Undo"));
    setStatusTip(tr("Undo the last action"));
  }
}

///////////////////////////////// End of File //////////////////////////////////
