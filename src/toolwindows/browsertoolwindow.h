////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    browsertoolwindow.h
///\ingroup bruo
///\brief   File browser tool window definition.
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
#ifndef __BROWSERTOOLWINDOW_H_INCLUDED__
#define __BROWSERTOOLWINDOW_H_INCLUDED__

#include "bruo.h"

////////////////////////////////////////////////////////////////////////////////
///\class BrowserToolWindow browsertoolwindow.h
///\brief Tool window for the application's file browser.
////////////////////////////////////////////////////////////////////////////////
class BrowserToolWindow :
  public QDockWidget
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // BrowserToolWindow::BrowserToolWindow()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this window.
  ///\param   [in] parent: Parent window for this window.
  ///\remarks Basically initializes the entire gui.
  //////////////////////////////////////////////////////////////////////////////
  BrowserToolWindow(QWidget* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // BrowserToolWindow::~BrowserToolWindow()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~BrowserToolWindow();

private slots:

  //////////////////////////////////////////////////////////////////////////////
  // BrowserToolWindow::bookmarkSelected()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the bookmark window selection changed signal.
  ///\param   [in] path: Path of the selected bookmark.
  ///\remarks Updates the current selection of the tree view.
  //////////////////////////////////////////////////////////////////////////////
  void bookmarkSelected(const QString& path);

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QTreeView* m_fileTree; ///> The file tree view.
};

#endif // #ifndef __BROWSERTOOLWINDOW_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
