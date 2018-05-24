////////////////////////////////////////////////////////////////////////////////
// (c) 2016 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    closealldocumentsaction.cpp
///\ingroup bruo
///\brief   Close all documents action implementation.
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
#include "closealldocumentsaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// CloseAllDocumentsAction::CloseAllDocumentsAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
CloseAllDocumentsAction::CloseAllDocumentsAction(MainFrame* parent) :
  ActiveDocumentAction(QIcon(":images/document-close-all.png"), tr("Close a&ll"), parent)
{
  setShortcut(QKeySequence(Qt::ALT + Qt::CTRL + Qt::Key_F4));
  setStatusTip(tr("Close all open documents"));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
}

////////////////////////////////////////////////////////////////////////////////
// CloseAllDocumentsAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void CloseAllDocumentsAction::fired()
{
  // Close the documents:
  m_parent->manager()->closeAllDocuments();
}

///////////////////////////////// End of File //////////////////////////////////
