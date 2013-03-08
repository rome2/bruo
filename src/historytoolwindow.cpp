////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    historytoolwindow.cpp
///\ingroup bruo
///\brief   History tool window implementation.
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
#include "historytoolwindow.h"

////////////////////////////////////////////////////////////////////////////////
// HistoryToolWindow::HistoryToolWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
HistoryToolWindow::HistoryToolWindow(DocumentManager* docMan, QWidget* parent) :
  QDockWidget(tr("History"), parent),
  m_docMan(docMan)
{
  // Set constraints:
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  // Name for position serialization:
  setObjectName("historyToolWindow");

  // Create undo view:
  m_undoView = new QUndoView(this);
  setWidget(m_undoView);

  // Connect to the document manager:
  connect(m_docMan, SIGNAL(activeDocumentChanged()), this, SLOT(activeDocumentChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// HistoryToolWindow::~MainFrame()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Cleans up used resources.
////////////////////////////////////////////////////////////////////////////////
HistoryToolWindow::~HistoryToolWindow()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// HistoryToolWindow::activeDocumentChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief This signal is emitted when another document was made active.
////////////////////////////////////////////////////////////////////////////////
void HistoryToolWindow::activeDocumentChanged()
{
  // Update current undo target:
  if (m_docMan->activeDocument() != 0)
    m_undoView->setStack(m_docMan->activeDocument()->undoStack());
  else
    m_undoView->setStack(0);
}

///////////////////////////////// End of File //////////////////////////////////
