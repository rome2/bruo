////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    savedocumentaction.cpp
///\ingroup bruo
///\brief   Save document action implementation.
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
#include "savedocumentaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// SaveDocumentAction::SaveDocumentAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent:  Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
SaveDocumentAction::SaveDocumentAction(MainFrame* parent) :
  QAction(QIcon(":/images/document-save.png"), tr("&Save"), parent),
  m_parent(parent)
{
  setEnabled(false);
  setShortcuts(QKeySequence::Save);
  setStatusTip(tr("Save the current document"));
  connect(parent->manager(), SIGNAL(documentCreated(Document*)), this, SLOT(documentCreated(Document*)));
  connect(parent->manager(), SIGNAL(activeDocumentChanged()), this, SLOT(dirtyChanged()));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
}

////////////////////////////////////////////////////////////////////////////////
// SaveDocumentAction::documentCreated()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the document manager's documentCreated signal.
///\remarks This one is called whenever a new document was created. At this
///         stage the document is still empty but this handler is good place to
///         attach per document signal handlers etc.
////////////////////////////////////////////////////////////////////////////////
void SaveDocumentAction::documentCreated(Document* doc)
{
  // Connect to the document's dirty slot:
  connect(doc, SIGNAL(dirtyChanged()), this, SLOT(dirtyChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// SaveDocumentAction::dirtyChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for dirty state changes.
///\remarks This one is called whenever the dirty state was changed.
////////////////////////////////////////////////////////////////////////////////
void SaveDocumentAction::dirtyChanged()
{
  // Get document:
  Document* doc = m_parent->manager()->activeDocument();

  // Update enabled state:
  setEnabled(doc != 0 && doc->dirty());
}

//////////////////////////////////////////////////////////////////////////////
// SaveDocumentAction::fired()
//////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
//////////////////////////////////////////////////////////////////////////////
void SaveDocumentAction::fired()
{
  // Get document:
  Document* doc = m_parent->manager()->activeDocument();
  if (doc == 0)
    return;

  // Save the document...
}

///////////////////////////////// End of File //////////////////////////////////
