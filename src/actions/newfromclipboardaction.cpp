////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    newfromclipboardaction.cpp
///\ingroup bruo
///\brief   New document from clipboard action implementation.
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
#include "newfromclipboardaction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// NewFromClipboardAction::NewFromClipboardAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent:  Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
NewFromClipboardAction::NewFromClipboardAction(MainFrame* parent) :
  QAction(QIcon(":/images/new_clip.png"), tr("Ne&w from clipboard"), parent),
  m_parent(parent)
{
  setShortcut(QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_N));
  setStatusTip(tr("Create a new document from clipboard"));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
  connect(QApplication::clipboard(), SIGNAL(changed(QClipboard::Mode)), SLOT(clipboardChanged(QClipboard::Mode)));
  clipboardChanged(QClipboard::Selection);
}

////////////////////////////////////////////////////////////////////////////////
// NewFromClipboardAction::clipboardChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for clipboard state changes.
///\param   [in] mode: What changed?
///\remarks This one is called whenever the clipboard was changed.
////////////////////////////////////////////////////////////////////////////////
void NewFromClipboardAction::clipboardChanged(QClipboard::Mode /* mode */)
{
  // Check if it's the right type:
  setEnabled(m_parent->manager()->canPaste(false));
}

//////////////////////////////////////////////////////////////////////////////
// NewFromClipboardAction::fired()
//////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
//////////////////////////////////////////////////////////////////////////////
void NewFromClipboardAction::fired()
{
  // Create doc...
}

///////////////////////////////// End of File //////////////////////////////////
