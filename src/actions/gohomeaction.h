////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    gohomeaction.h
///\ingroup bruo
///\brief   Go home action definition.
///\author  Rolf Meyerhoff (badlantic@gmail.com)
///\version 1.0
/// This file is part of the bruo audio editor.
////////////////////////////////////////////////////////////////////////////////
///\par License:
/// This program is free software: you can redistribute it and/or modify it
/// under the terms of the GNU General Public License as published by the Free
/// Software Foundation, either version 2 of the License, or (at your option)
/// any later version.
///\paction
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
#ifndef __GOHOMEACTION_H_INCLUDED__
#define __GOHOMEACTION_H_INCLUDED__

#include "../bruo.h"

////////////////////////////////////////////////////////////////////////////////
///\class GoHomeAction gohomeaction.h
///\brief Go home action class.
/// Opens the system's default browser and navigates to this project's home
/// page.
////////////////////////////////////////////////////////////////////////////////
class GoHomeAction :
  public QAction
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // GoHomeAction::GoHomeAction()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this action.
  ///\param   [in] parent: Parent object for this action.
  ///\remarks Initializes the action states, strings, events and icons.
  //////////////////////////////////////////////////////////////////////////////
  GoHomeAction(class MainFrame* parent);

private slots:

  //////////////////////////////////////////////////////////////////////////////
  // GoHomeAction::fired()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief The function where the action happens.
  //////////////////////////////////////////////////////////////////////////////
  void fired();

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  class MainFrame* m_parent; ///> The window that we are acting on.
};

#endif // __GOHOMEACTION_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
