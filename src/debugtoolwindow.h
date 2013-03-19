////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    debugtoolwindow.h
///\ingroup bruo
///\brief   Debug view tool window definition.
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
#ifndef __DEBUGTOOLWINDOW_H_INCLUDED__
#define __DEBUGTOOLWINDOW_H_INCLUDED__

#include "bruo.h"

////////////////////////////////////////////////////////////////////////////////
///\class DebugToolWindow debugtoolwindow.h
///\brief Tool window for the application's debug output viewer.
////////////////////////////////////////////////////////////////////////////////
class DebugToolWindow :
  public QDockWidget
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // DebugToolWindow::DebugToolWindow()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this window.
  ///\param   [in] parent: Parent window for this window.
  ///\remarks Basically initializes the entire gui.
  //////////////////////////////////////////////////////////////////////////////
  DebugToolWindow(QWidget* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // DebugToolWindow::~DebugToolWindow()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~DebugToolWindow();
};

#endif // #ifndef __DEBUGTOOLWINDOW_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
