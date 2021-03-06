////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    selectcommand.h
///\ingroup bruo
///\brief   Selection change command.
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
#ifndef __SELECTCOMMAND_H_INCLUDED__
#define __SELECTCOMMAND_H_INCLUDED__

#include "appundocommand.h"

////////////////////////////////////////////////////////////////////////////////
///\class   SelectCommand selectcommand.h
///\brief   Undo/redo command for the document's selection changes.
///\remarks If you want to do continuous selections use SelectingCommand
///         instead.
////////////////////////////////////////////////////////////////////////////////
class SelectCommand :
  public AppUndoCommand
{
public:

  //////////////////////////////////////////////////////////////////////////////
  // SelectCommand::SelectCommand()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Initialization constructor of this class.
  ///\param [in] doc:        The target document.
  ///\param [in] selStart:   New selection start.
  ///\param [in] selLength:  New selection length.
  ///\param [in] selChannel: New selection channel.
  ///\param [in] parent:     Parent undo item.
  //////////////////////////////////////////////////////////////////////////////
  SelectCommand(Document* doc, qint64 selStart, qint64 selLength, int selChannel = -1, QUndoCommand* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // SelectCommand::~SelectCommand()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Destructor of this class.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~SelectCommand();

  //////////////////////////////////////////////////////////////////////////////
  // SelectCommand::undo()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Reverts a change to the document.
  //////////////////////////////////////////////////////////////////////////////
  virtual void undo();

  //////////////////////////////////////////////////////////////////////////////
  // SelectCommand::redo()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Applies a change to the document.
  //////////////////////////////////////////////////////////////////////////////
  virtual void redo();

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  qint64 m_newSelStart;  ///> New selection start.
  qint64 m_newSelLength; ///> New selection length.
  int    m_newSelChan;   ///> New selection channel.
  qint64 m_oldSelStart;  ///> Old selection start.
  qint64 m_oldSelLength; ///> Old selection length.
  int    m_oldSelChan;   ///> Old selection channel.
};

#endif // #ifndef __SELECTCOMMAND_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
