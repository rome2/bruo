////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    selectionaction.cpp
///\ingroup bruo
///\brief   Selection action implementation.
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
#include "activedocumentaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// ActiveDocumentAction::ActiveDocumentAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] icon:    Icon of the action.
///\param   [in] caption: Title of the action.
///\param   [in] parent:  Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
ActiveDocumentAction::ActiveDocumentAction(QIcon icon, QString caption, MainFrame* parent) :
  QAction(icon, caption, parent),
  m_parent(parent)
{
  setEnabled(false);
  connect(parent->manager(), SIGNAL(activeDocumentChanged()), this, SLOT(activeDocumentChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// ActiveDocumentAction::activeDocumentChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the document manager's activeDocumentChanged signal.
///\remarks This one is called whenever a new document was made active. The
///         current document may be null if there is no document open.
////////////////////////////////////////////////////////////////////////////////
void ActiveDocumentAction::activeDocumentChanged()
{
  // Uppdate enabled state:
  setEnabled(m_parent->manager()->activeDocument() != 0);
}

///////////////////////////////// End of File //////////////////////////////////
