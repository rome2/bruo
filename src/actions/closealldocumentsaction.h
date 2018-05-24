////////////////////////////////////////////////////////////////////////////////
// (c) 2016 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    closealldocumentsaction.h
///\ingroup bruo
///\brief   Close all documents action definition.
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
#ifndef __CLOSEALLDOCUMENTSACTION_H_INCLUDED__
#define __CLOSEALLDOCUMENTSACTION_H_INCLUDED__

#include "../bruo.h"
#include "activedocumentaction.h"

////////////////////////////////////////////////////////////////////////////////
///\class CloseAllDocumentsAction closealldocumentsaction.h
///\brief Close all documents action class.
/// Closes all open documents.
////////////////////////////////////////////////////////////////////////////////
class CloseAllDocumentsAction :
  public ActiveDocumentAction
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // CloseAllDocumentsAction::CloseAllDocumentsAction()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this action.
  ///\param   [in] parent: Parent object for this action.
  ///\remarks Initializes the action states, strings, events and icons.
  //////////////////////////////////////////////////////////////////////////////
  CloseAllDocumentsAction(class MainFrame* parent);

private slots:

  //////////////////////////////////////////////////////////////////////////////
  // CloseAllDocumentsAction::fired()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief The function where the action happens.
  //////////////////////////////////////////////////////////////////////////////
  void fired();
};

#endif // __CLOSEALLDOCUMENTSACTION_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
