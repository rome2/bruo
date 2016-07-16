////////////////////////////////////////////////////////////////////////////////
// (c) 2016 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    showmorerecentfilesaction.cpp
///\ingroup bruo
///\brief   Open recent file selection dialog action implementation.
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
#include "showmorerecentfilesaction.h"
#include "../controls/stringselectdialog.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// ShowMoreRecentFilesAction::ShowMoreRecentFilesAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
ShowMoreRecentFilesAction::ShowMoreRecentFilesAction(MainFrame* parent) :
  QAction(QIcon(":/images/document-open-recent.png"), tr("&More..."), parent),
  m_parent(parent)
{
  setShortcut(QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_O));
  setStatusTip(tr("Show full list of recent files"));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
  connect(parent->manager(), SIGNAL(recentFilesChanged()), this, SLOT(recentFilesChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// ShowMoreRecentFilesAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void ShowMoreRecentFilesAction::fired()
{
  // Create and init the dialog:
  StringSelectDialog dialog(m_parent->manager()->recentFiles(), m_parent);
  dialog.setWindowTitle(tr("Select File"));

  // Show it:
  if (dialog.exec() == QDialog::Accepted)
  {
    // Load the seected file:
    int index = dialog.selectedItem();
    if (index >= 0 && index < m_parent->manager()->recentFiles().length())
      m_parent->loadFile(m_parent->manager()->recentFiles()[index]);
  }
}

//////////////////////////////////////////////////////////////////////////////
// ShowMoreRecentFilesAction::recentFilesChanged()
//////////////////////////////////////////////////////////////////////////////
///\brief The recent file list was changed, update state accordingly.
//////////////////////////////////////////////////////////////////////////////
void ShowMoreRecentFilesAction::recentFilesChanged()
{
  // Update enabled state:
  setEnabled(m_parent->manager()->recentFiles().length() > 10);
}

///////////////////////////////// End of File //////////////////////////////////
