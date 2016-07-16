////////////////////////////////////////////////////////////////////////////////
// (c) 2016 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    clearrecentfilesaction.cpp
///\ingroup bruo
///\brief   Clear recent files action implementation.
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
#include "clearrecentfilesaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// ClearRecentFilesAction::ClearRecentFilesAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
ClearRecentFilesAction::ClearRecentFilesAction(MainFrame* parent) :
  QAction(QIcon(":/images/edit-clear-list.png"), tr("&Clear list"), parent),
  m_parent(parent)
{
  setStatusTip(tr("Clears the list of recent files"));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
  connect(parent->manager(), SIGNAL(recentFilesChanged()), this, SLOT(recentFilesChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// ClearRecentFilesAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void ClearRecentFilesAction::fired()
{
  // Clear list:
  m_parent->manager()->clearRecentFiles();
}

//////////////////////////////////////////////////////////////////////////////
// ClearRecentFilesAction::recentFilesChanged()
//////////////////////////////////////////////////////////////////////////////
///\brief The recent file list was changed, update state accordingly.
//////////////////////////////////////////////////////////////////////////////
void ClearRecentFilesAction::recentFilesChanged()
{
  // Update enabled state:
  setEnabled(m_parent->manager()->recentFiles().length() > 0);
}

///////////////////////////////// End of File //////////////////////////////////
