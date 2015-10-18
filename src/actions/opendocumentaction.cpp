////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    opendocumentaction.cpp
///\ingroup bruo
///\brief   Open document action implementation.
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
#include "opendocumentaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// OpenDocumentAction::OpenDocumentAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
OpenDocumentAction::OpenDocumentAction(MainFrame* parent) :
  QAction(QIcon(":/images/document-open.png"), tr("&Open..."), parent),
  m_parent(parent)
{
  setShortcuts(QKeySequence::Open);
  setStatusTip(tr("Open an existing document"));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
}

////////////////////////////////////////////////////////////////////////////////
// OpenDocumentAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void OpenDocumentAction::fired()
{
  // Show open dialog:
  QFileDialog dialog;
  dialog.setNameFilters(getOpenFilters());
  dialog.setParent(m_parent);
  if (dialog.exec())
  {
    // Load the file:
    m_parent->loadFile(dialog.selectedFiles()[0]);
  }
}

///////////////////////////////// End of File //////////////////////////////////
