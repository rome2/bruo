////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    aboutaction.cpp
///\ingroup bruo
///\brief   About action implementation.
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
#include "aboutaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// AboutAction::AboutAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent: Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
AboutAction::AboutAction(MainFrame* parent) :
  QAction(QIcon(":/images/help-about.png"), tr("&About..."), parent),
  m_parent(parent)
{
  setStatusTip(tr("Show the application's About box"));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
}

////////////////////////////////////////////////////////////////////////////////
// AboutAction::fired()
////////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
////////////////////////////////////////////////////////////////////////////////
void AboutAction::fired()
{
  // Show about box:
  QMessageBox::about(m_parent, tr("About bruo"), tr("About box text."));
}

///////////////////////////////// End of File //////////////////////////////////
