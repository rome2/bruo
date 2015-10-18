////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    pasteaction.cpp
///\ingroup bruo
///\brief   Paste from clipboard action implementation.
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
#include "pasteaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// PasteAction::PasteAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent:  Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
PasteAction::PasteAction(MainFrame* parent) :
  QAction(QIcon(":/images/edit-paste.png"), tr("&Paste"), parent),
  m_parent(parent)
{
  setShortcuts(QKeySequence::Paste);
  setStatusTip(tr("Insert the clipboard contents at the current position."));
  connect(parent->manager(), SIGNAL(activeDocumentChanged()), this, SLOT(activeDocumentChanged()));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
  connect(QApplication::clipboard(), SIGNAL(changed(QClipboard::Mode)), SLOT(clipboardChanged(QClipboard::Mode)));
  clipboardChanged(QClipboard::Selection);
}

////////////////////////////////////////////////////////////////////////////////
// PasteAction::activeDocumentChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the document manager's activeDocumentChanged signal.
///\remarks This one is called whenever a new document was made active. The
///         current document may be null if there is no document open.
////////////////////////////////////////////////////////////////////////////////
void PasteAction::activeDocumentChanged()
{
  // Uppdate enabled state:
  setEnabled(m_parent->manager()->canPaste(true));
}

////////////////////////////////////////////////////////////////////////////////
// PasteAction::clipboardChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for clipboard state changes.
///\param   [in] mode: What changed?
///\remarks This one is called whenever the clipboard was changed.
////////////////////////////////////////////////////////////////////////////////
void PasteAction::clipboardChanged(QClipboard::Mode /* mode */)
{
  // Check if it's the right type:
  setEnabled(m_parent->manager()->canPaste(true));
}

//////////////////////////////////////////////////////////////////////////////
// PasteAction::fired()
//////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
//////////////////////////////////////////////////////////////////////////////
void PasteAction::fired()
{
  // Get document:
  Document* doc = m_parent->manager()->activeDocument();
  if (doc == 0)
    return;

  // Do the paste...
}

///////////////////////////////// End of File //////////////////////////////////
