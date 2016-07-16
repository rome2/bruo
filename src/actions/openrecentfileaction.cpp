////////////////////////////////////////////////////////////////////////////////
// (c) 2016 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    openrecentfileaction.cpp
///\ingroup bruo
///\brief   Open recent file action implementation.
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
#include "openrecentfileaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// OpenRecentFileAction::OpenRecentFileAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\param   [in] index:  Index of the recent file that we belong to.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
OpenRecentFileAction::OpenRecentFileAction(MainFrame* parent, int index) :
  QAction(QIcon(":/images/wave-document.png"), "-", parent),
  m_parent(parent),
  m_index(index)
{
  setVisible(false);
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
  connect(parent->manager(), SIGNAL(recentFilesChanged()), this, SLOT(recentFilesChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// OpenRecentFileAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void OpenRecentFileAction::fired()
{
  // Sanity check:
  if (m_index >= m_parent->manager()->recentFiles().length())
    return;

  // Load the file:
  m_parent->loadFile(m_parent->manager()->recentFiles()[m_index]);
}

//////////////////////////////////////////////////////////////////////////////
// OpenRecentFileAction::recentFilesChanged()
//////////////////////////////////////////////////////////////////////////////
///\brief The recent file list was changed, update state accordingly.
//////////////////////////////////////////////////////////////////////////////
void OpenRecentFileAction::recentFilesChanged()
{
  // Update visible state:
  setVisible(m_index < m_parent->manager()->recentFiles().length());
  if (!isVisible())
    return;

  // Show full paths?
  bool fullPaths = QSettings().value("document/recentFilesFullPath", true).toBool();

  // Get short file name:
  QString shortPath = QFileInfo(m_parent->manager()->recentFiles()[m_index]).fileName();

  // Update action text:
  QString text = QString(m_index == 9 ? "1&0: %1" : "&%1: ").arg(m_index + 1);
  if (fullPaths)
    text.append(m_parent->manager()->recentFiles()[m_index]);
  else
    text.append(shortPath);
  setText(text);

  // Update status bar entry:
  setStatusTip(tr("Open %1").arg(shortPath));
}

///////////////////////////////// End of File //////////////////////////////////
