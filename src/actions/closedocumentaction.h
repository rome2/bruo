////////////////////////////////////////////////////////////////////////////////
// (c) 2016 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    closedocumentaction.h
///\ingroup bruo
///\brief   Close document action definition.
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
#ifndef __CLOSEDOCUMENTACTION_H_INCLUDED__
#define __CLOSEDOCUMENTACTION_H_INCLUDED__

#include "../bruo.h"
#include "activedocumentaction.h"

////////////////////////////////////////////////////////////////////////////////
///\class CloseDocumentAction closedocumentaction.h
///\brief Close document action class.
/// Closes the current document.
////////////////////////////////////////////////////////////////////////////////
class CloseDocumentAction :
  public ActiveDocumentAction
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // CloseDocumentAction::CloseDocumentAction()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this action.
  ///\param   [in] parent: Parent object for this action.
  ///\remarks Initializes the action states, strings, events and icons.
  //////////////////////////////////////////////////////////////////////////////
  CloseDocumentAction(class MainFrame* parent);

private slots:

  //////////////////////////////////////////////////////////////////////////////
  // CloseDocumentAction::fired()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief The function where the action happens.
  //////////////////////////////////////////////////////////////////////////////
  void fired();
};

#endif // __CLOSEDOCUMENTACTION_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
