////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    selectingcommand.h
///\ingroup bruo
///\brief   Continuous selection change command.
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
#ifndef __SELECTINGCOMMAND_H_INCLUDED__
#define __SELECTINGCOMMAND_H_INCLUDED__

#include "appundocommand.h"

////////////////////////////////////////////////////////////////////////////////
///\class   SelectingCommand selectingcommand.h
///\brief   Undo/redo command for the document's continous selection changes.
///\remarks Use this if you want to do continuos selections (like moving
///         selection borders, marquee selection etc). Create a command for
///         each move. The commands will create a document.selectionChanging()
///         event for every move and merge itself into the prevoius undo
///         entry.
///         If you set the 'last' parameter of the constructor to 'true' then
///         this command will fire a document.selectionChanged() event that
///         signals the end of the selection operation. This will terminate
///         the undo merging as well.
////////////////////////////////////////////////////////////////////////////////
class SelectingCommand :
  public AppUndoCommand
{
public:

  //////////////////////////////////////////////////////////////////////////////
  // SelectingCommand::SelectingCommand()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Initialization constructor of this class.
  ///\param [in] doc:       The target document.
  ///\param [in] selStart:  New selection start.
  ///\param [in] selLength: New selection length.
  ///\param [in] selChannel: New selection channel.
  ///\param [in] last:      Last command in the chain?
  ///\param [in] parent:    Parent undo item.
  //////////////////////////////////////////////////////////////////////////////
  SelectingCommand(Document* doc, qint64 selStart, qint64 selLength, int selChannel = -1, bool last = false, QUndoCommand* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // SelectingCommand::~SelectingCommand()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Destructor of this class.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~SelectingCommand();

  //////////////////////////////////////////////////////////////////////////////
  // SelectingCommand::undo()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Reverts a change to the document.
  //////////////////////////////////////////////////////////////////////////////
  virtual void undo();

  //////////////////////////////////////////////////////////////////////////////
  // SelectingCommand::redo()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Applies a change to the document.
  //////////////////////////////////////////////////////////////////////////////
  virtual void redo();

  //////////////////////////////////////////////////////////////////////////////
  // SelectingCommand::id()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Accessor for the id property of this command to the document.
  ///\return The id of this command.
  //////////////////////////////////////////////////////////////////////////////
  virtual int id() const;

  //////////////////////////////////////////////////////////////////////////////
  // SelectingCommand::mergeWith()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Merge a following undo command into this one.
  ///\param  [in] other: The command to merge.
  ///\return true if the merge was successful or false otherwise.
  //////////////////////////////////////////////////////////////////////////////
  virtual bool mergeWith(const QUndoCommand* other);

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  qint64 m_newSelStart;  ///> New selection start.
  qint64 m_newSelLength; ///> New selection length.
  int    m_newSelChan;   ///> New selection channel.
  qint64 m_oldSelStart;  ///> Old selection start.
  qint64 m_oldSelLength; ///> Old selection length.
  int    m_oldSelChan;   ///> Old selection channel.
  bool   m_last;         ///> Is this the last one in the chain?
};

#endif // #ifndef __SELECTINGCOMMAND_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
