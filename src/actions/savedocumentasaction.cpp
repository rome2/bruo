////////////////////////////////////////////////////////////////////////////////
// (c) 2016 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    savedocumentasaction.cpp
///\ingroup bruo
///\brief   Save document as action implementation.
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
#include "savedocumentasaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// SaveDocumentAsAction::SaveDocumentAsAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
SaveDocumentAsAction::SaveDocumentAsAction(MainFrame* parent) :
  ActiveDocumentAction(QIcon(":/images/document-save-as.png"), tr("Save &as..."), parent)
{
  setShortcut(QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_S));
  setStatusTip(tr("Save the current document under a new name"));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
}

////////////////////////////////////////////////////////////////////////////////
// SaveDocumentAsAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void SaveDocumentAsAction::fired()
{
  // Get current document:
  Document* doc = m_parent->manager()->activeDocument();
  if (doc == 0)
    return;

  //TODO the action...
}

///////////////////////////////// End of File //////////////////////////////////
