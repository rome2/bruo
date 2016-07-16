////////////////////////////////////////////////////////////////////////////////
// (c) 2016 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    noecentfilesaction.cpp
///\ingroup bruo
///\brief   No recent files action implementation.
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
#include "norecentfilesaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// NoRecentFilesAction::NoRecentFilesAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
NoRecentFilesAction::NoRecentFilesAction(MainFrame* parent) :
  QAction(tr("No recent files"), parent),
  m_parent(parent)
{
  setEnabled(false);
  setStatusTip(tr("No recent files"));
  connect(parent->manager(), SIGNAL(recentFilesChanged()), this, SLOT(recentFilesChanged()));
}

//////////////////////////////////////////////////////////////////////////////
// NoRecentFilesAction::recentFilesChanged()
//////////////////////////////////////////////////////////////////////////////
///\brief The recent file list was changed, update state accordingly.
//////////////////////////////////////////////////////////////////////////////
void NoRecentFilesAction::recentFilesChanged()
{
  // Update visible state:
  setVisible(m_parent->manager()->recentFiles().length() <= 0);
}

///////////////////////////////// End of File //////////////////////////////////
