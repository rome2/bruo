////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    stringselectdialog.h
///\ingroup bruo
///\brief   Simple string list selection dialog definition.
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
#ifndef __STRINGSELECTDIALOG_H_INCLUDED__
#define __STRINGSELECTDIALOG_H_INCLUDED__

#include "bruo.h"

////////////////////////////////////////////////////////////////////////////////
///\class StringSelectDialog stringselectdialog.h
///\brief Simple string list selection dialog class.
/// This dialog is a simple way to choose from a given set of strings.
////////////////////////////////////////////////////////////////////////////////
class StringSelectDialog : public QDialog
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // StringSelectDialog::StringSelectDialog()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this window.
  ///\param   [in] strings: The strings to choose from.
  ///\param   [in] parent: Parent window for this window.
  ///\remarks Basically initializes the entire gui.
  //////////////////////////////////////////////////////////////////////////////
  StringSelectDialog(const QStringList& strings, QWidget* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // StringSelectDialog::~StringSelectDialog()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this class.
  ///\remarks Does final cleanup.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~StringSelectDialog();

  //////////////////////////////////////////////////////////////////////////////
  // StringSelectDialog::selectedItem()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the currently selected string.
  ///\return  The index of the currently selected string or -1 if there is no
  ///         selection.
  ///\remarks This index is the same as the index of the string in the list that
  ///         was passed to the constructor.
  //////////////////////////////////////////////////////////////////////////////
  int selectedItem() const;

public slots:

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::listBoxSelectionChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the list widget's selectionChanged signal.
  ///\remarks Just updates the current index.
  ///////////////////////////////////////////////////////////////////////////////
  void listBoxSelectionChanged();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::listBoxItemDoubleClicked()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the list widget's itemDoubleClicked signal.
  ///\param   [in] item: The item that was double clicked.
  ///\remarks Updates the current index and closes the dialog via accept().
  //////////////////////////////////////////////////////////////////////////////
  void listBoxItemDoubleClicked(QListWidgetItem* item);

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  int m_selectedItem; ///> Index of the currently selected item.
};

#endif // #ifndef __STRINGSELECTDIALOG_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////

