////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    stringselectdialog.h
///\ingroup bruo
///\brief   Simple string list selection dialog implementation.
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
#include "stringselectdialog.h"

////////////////////////////////////////////////////////////////////////////////
// StringSelectDialog::StringSelectDialog()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] strings: The strings to choose from.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
StringSelectDialog::StringSelectDialog(const QStringList& strings, QWidget* parent) :
  QDialog(parent),
  m_selectedItem(-1)
{
  // Init the dialog:
  setWindowTitle(tr("Please select"));
  setSizeGripEnabled(true);
  resize(440, 300);

  // Create and fill the list box:
  QListWidget* listBox = new QListWidget(this);
  listBox->addItems(strings);
  connect(listBox, SIGNAL(itemSelectionChanged()), this, SLOT(listBoxSelectionChanged()));
  connect(listBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listBoxItemDoubleClicked(QListWidgetItem*)));

  // Create ok and cancel buttons:
  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  // Create a layout for the mess:
  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(listBox);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);
}

////////////////////////////////////////////////////////////////////////////////
// StringSelectDialog::~StringSelectDialog()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this class.
///\remarks Does final cleanup.
////////////////////////////////////////////////////////////////////////////////
StringSelectDialog::~StringSelectDialog()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// StringSelectDialog::selectedItem()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the currently selected string.
///\return  The index of the currently selected string or -1 if there is no
///         selection.
///\remarks This index is the same as the index of the string in the list that
///         was passed to the constructor.
////////////////////////////////////////////////////////////////////////////////
int StringSelectDialog::selectedItem() const
{
  // Return the requested item:
  return m_selectedItem;
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::listBoxSelectionChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the list widget's selectionChanged signal.
///\remarks Just updates the current index.
////////////////////////////////////////////////////////////////////////////////
void StringSelectDialog::listBoxSelectionChanged()
{
  // Get the list box:
  QListWidget* listBox = qobject_cast<QListWidget*>(sender());
  if (listBox == 0)
    return;

  // Get selection:
  m_selectedItem = listBox->currentRow();
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::listBoxItemDoubleClicked()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the list widget's itemDoubleClicked signal.
///\param   [in] item: The item that was double clicked.
///\remarks Updates the current index and closes the dialog via accept().
////////////////////////////////////////////////////////////////////////////////
void StringSelectDialog::listBoxItemDoubleClicked(QListWidgetItem* item)
{
  // Get the list box:
  QListWidget* listBox = qobject_cast<QListWidget*>(sender());
  if (listBox == 0)
    return;

  // Get the item's index:
  m_selectedItem = listBox->row(item);
  if (m_selectedItem < 0)
    return;

  // Close the dialog:
  accept();
}

///////////////////////////////// End of File //////////////////////////////////
