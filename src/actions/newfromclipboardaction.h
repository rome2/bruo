////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    newfromclipboardaction.h
///\ingroup bruo
///\brief   New document from clipboard action definition.
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
#ifndef __NEWFROMCLIPBOARDACTION_H_INCLUDED__
#define __NEWFROMCLIPBOARDACTION_H_INCLUDED__

#include "../bruo.h"

////////////////////////////////////////////////////////////////////////////////
///\class NewFromClipboardAction newfromclipboardaction.h
///\brief New document from clipboard action class.
////////////////////////////////////////////////////////////////////////////////
class NewFromClipboardAction :
  public QAction
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // NewFromClipboardAction::NewFromClipboardAction()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this action.
  ///\param   [in] parent: Parent object for this action.
  ///\remarks Initializes the action states, strings, events and icons.
  //////////////////////////////////////////////////////////////////////////////
  NewFromClipboardAction(class MainFrame* parent);

private slots:

  //////////////////////////////////////////////////////////////////////////////
  // NewFromClipboardAction::clipboardChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for clipboard state changes.
  ///\param   [in] mode: What changed?
  ///\remarks This one is called whenever the clipboard was changed.
  //////////////////////////////////////////////////////////////////////////////
  void clipboardChanged(QClipboard::Mode mode);

  //////////////////////////////////////////////////////////////////////////////
  // NewFromClipboardAction::fired()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief The function where the action happens.
  //////////////////////////////////////////////////////////////////////////////
  void fired();

protected:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  class MainFrame* m_parent; ///> The window that we are acting on.
};

#endif // __NEWFROMCLIPBOARDACTION_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
