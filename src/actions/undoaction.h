////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    undoaction.h
///\ingroup bruo
///\brief   Undo action definition.
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
#ifndef __UNDOACTION_H_INCLUDED__
#define __UNDOACTION_H_INCLUDED__

#include "../bruo.h"

////////////////////////////////////////////////////////////////////////////////
///\class UndoAction undoaction.h
///\brief Undo action class.
/// Calls the undo command of the current document's undo manager and this will
/// in turn update the UI (history window, enabled states etc).
////////////////////////////////////////////////////////////////////////////////
class UndoAction :
  public QAction
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // UndoAction::UndoAction()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this action.
  ///\param   [in] parent: Parent object for this action.
  ///\remarks Initializes the action states, strings, events and icons.
  //////////////////////////////////////////////////////////////////////////////
  UndoAction(class MainFrame* parent);

private slots:

  //////////////////////////////////////////////////////////////////////////////
  // UndoAction::fired()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief The function where the action happens.
  //////////////////////////////////////////////////////////////////////////////
  void fired();

  //////////////////////////////////////////////////////////////////////////////
  // UndoAction::documentCreated()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the document manager's documentCreated signal.
  ///\remarks This one is called whenever a new document was created. At this
  ///         stage the document is still empty but this handler is good place
  ///         to attach per document signal handlers etc.
  //////////////////////////////////////////////////////////////////////////////
  void documentCreated(class Document* doc);

  //////////////////////////////////////////////////////////////////////////////
  // UndoAction::activeDocumentChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the document manager's activeDocumentChanged signal.
  ///\remarks This one is called whenever a new document was made active. The
  ///         current document may be null if there is no document open.
  //////////////////////////////////////////////////////////////////////////////
  void stateChanged();

  //////////////////////////////////////////////////////////////////////////////
  // UndoAction::canUndoChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the current document's canUndo state changed signal.
  ///\param   [in] state: New state of the property.
  ///\remarks Updates the state of the related UI elements.
  //////////////////////////////////////////////////////////////////////////////
  void canUndoChanged(bool state);

  //////////////////////////////////////////////////////////////////////////////
  // UndoAction::undoTextChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the current document's undo text changed signal.
  ///\param   [in] newString: New text of the property.
  ///\remarks Updates the state of the related UI elements.
  //////////////////////////////////////////////////////////////////////////////
  void undoTextChanged(QString newString);

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  class MainFrame* m_parent; ///> The window that we are acting on.
};

#endif // __UNDOACTION_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
