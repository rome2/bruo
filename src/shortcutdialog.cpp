////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    shortcutdialog.cpp
///\ingroup bruo
///\brief   Shortcut editor dialog definition.
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
#include "bruo.h"
#include "shortcutdialog.h"
#include "controls/shortcutedit.h"

////////////////////////////////////////////////////////////////////////////////
///\class CustomItem shortcutdialog.cpp
///\brief Helper class to attach user data to a table item.
////////////////////////////////////////////////////////////////////////////////
class CustomItem :
  public QTableWidgetItem
{
public:
  //////////////////////////////////////////////////////////////////////////////
  // CustomItem::CustomItem()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Initialization constructor of this class.
  ///\param [in] text:       The text to show in the table.
  ///\param [in] itemAction: The action of this item.
  //////////////////////////////////////////////////////////////////////////////
  CustomItem(const QString& text, QAction* itemAction) :
    QTableWidgetItem(text, QTableWidgetItem::UserType + 1),
    m_action(itemAction),
    m_modified(false)
  {
    // Get current shortcut:
    m_shortCut = m_action->shortcut();
  }

  //////////////////////////////////////////////////////////////////////////////
  // CustomItem::action()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Access the action of this item.
  ///\return The currently set action.
  //////////////////////////////////////////////////////////////////////////////
  QAction* action()
  {
    // Return our action:
    return m_action;
  }

  //////////////////////////////////////////////////////////////////////////////
  // CustomItem::modified()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Has this item been modified?
  ///\return True if the shortcut has been changedd for the item's action.
  //////////////////////////////////////////////////////////////////////////////
  bool modified() const
  {
    // Return current state:
    return m_modified;
  }

  //////////////////////////////////////////////////////////////////////////////
  // CustomItem::shortcut()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Access the shortcut of this item.
  ///\return The currently set shortcut.
  //////////////////////////////////////////////////////////////////////////////
  const QKeySequence& shortcut() const
  {
    // Return current shortcut:
    return m_shortCut;
  }

  //////////////////////////////////////////////////////////////////////////////
  // CustomItem::setShortcut()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Set the shortcut of this item.
  ///\param [in] sc: The new shortcut.
  //////////////////////////////////////////////////////////////////////////////
  void setShortcut(const QKeySequence& sc)
  {
    // Anything to do?
    if (sc != m_shortCut)
    {
      // Set shortcut:
      m_shortCut = sc;

      // Flag modified:
      m_modified = true;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // CustomItem::restore()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Restore the shortcut to it's initial state.
  //////////////////////////////////////////////////////////////////////////////
  void restore()
  {
    // Get current shortcut:
    m_shortCut = m_action->shortcut();

    // Reset flag:
    m_modified = false;
  }

private:
  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QAction*     m_action;   ///> The target action.
  bool         m_modified; ///> Was this item changed?
  QKeySequence m_shortCut; ///> The current shortcut for the action.
};

////////////////////////////////////////////////////////////////////////////////
// ShortcutDialog::ShortcutDialog()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] actionMap: The action list to edit.
///\param   [in] parent:    Parent window for this window.
///\remarks Basically initializes the gui.
////////////////////////////////////////////////////////////////////////////////
ShortcutDialog::ShortcutDialog(QHash<QString, QAction*>& actionMap, QWidget* parent) :
  QDialog(parent)
{
  // Init the dialog:
  setWindowTitle(tr("Configure shortcuts"));
  setSizeGripEnabled(true);
  resize(640, 480);

  // Create a key map entry for the last known state:
  m_maps.append(Keymap());
  m_maps[0].setName(tr("Current shortcut set"));
  for (QHash<QString, QAction*>::iterator i = actionMap.begin(); i != actionMap.end(); i++)
    m_maps[0].setKey(i.key(), (*i)->shortcut());

  // Load maps from home directory:
  QString mapPath = getSettingsPath();
  QDir mapDir(mapPath);
  QStringList filters;
  filters << "*.keymap";
  mapDir.setNameFilters(filters);
  QStringList keyFiles = mapDir.entryList();
  for (int i = 0; i < keyFiles.count(); i++)
  {
    m_maps.append(Keymap());
    m_maps[m_maps.count() - 1].load(mapPath + keyFiles[i]);
  }

  // Load common presets:
  QStringList libraryDirs = getLibraryPaths();
  for (int i = 0; i < libraryDirs.count(); i++)
  {
    mapPath = libraryDirs[i] + "keymaps" + QDir::separator();
    mapDir.setPath(mapPath);
    keyFiles = mapDir.entryList();
    for (int i = 0; i < keyFiles.count(); i++)
    {
      m_maps.append(Keymap());
      m_maps[m_maps.count() - 1].load(mapPath + keyFiles[i]);
    }
  }

  // Create preset label:
  QLabel* presetLabel = new QLabel(tr("Load preset:"), this);

  // Fill preset combo box:
  QComboBox* presetCombo = new QComboBox(this);
  for (int i = 0; i < m_maps.count(); i++)
    presetCombo->addItem(m_maps[i].name());
  presetCombo->setCurrentIndex(-1);
  connect(presetCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(presetChanged(int)));

  // Create save button:
  QPushButton* saveBtn = new QPushButton(tr("Save"), this);

  // Create search text and icon:
  QLineEdit* searchEdit = new QLineEdit(this);
  connect(searchEdit, SIGNAL(textChanged(QString)), this, SLOT(searchTextChanged(QString)));
  QLabel* searchIcon = new QLabel(this);
  searchIcon->setPixmap(QPixmap(":images/search16.png"));

  // Create table widget and add actions:
  m_tableWidget = new QTableWidget(actionMap.count(), 4, this);
  int row = 0;
  for (QHash<QString, QAction*>::iterator i = actionMap.begin(); i != actionMap.end(); i++)
  {
    QAction* action = *i;

    // Get stripped text:
    QString str = action->text().remove('&').remove('.');
    if (str.isEmpty())
      continue;

    // If it starts with a digit skip it:
    if (str.at(0).isDigit())
      continue;

    // Create text:
    QTableWidgetItem* item = new CustomItem(str, action);
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    m_tableWidget->setItem(row, 1, item);

    // Add icon:
    item = new QTableWidgetItem();
    if (!action->icon().isNull())
      item->setIcon(action->icon());
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    m_tableWidget->setItem(row, 0, item);

    // Add shortcut:
    item = new QTableWidgetItem(action->shortcut().toString());
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    m_tableWidget->setItem(row, 2, item);

    // Add description:
    item = new QTableWidgetItem(action->statusTip());
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    m_tableWidget->setItem(row, 3, item);

    // Next row:
    row++;
  }

  // Shrink and sort the list:
  m_tableWidget->setRowCount(row);
  m_tableWidget->sortItems(1);

  // Adjust cell dimensions:
  m_tableWidget->resizeRowsToContents();
  m_tableWidget->resizeColumnsToContents();
  m_tableWidget->horizontalHeader()->hide();
  m_tableWidget->verticalHeader()->hide();

  // Some visual stylings:
  m_tableWidget->setAlternatingRowColors(true);
  m_tableWidget->setShowGrid(false);
  m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

  // Set events:
  connect(m_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));

  // Create edit area:
  QLabel* keyLabel = new QLabel(tr("Change shortcut to:"), this);
  m_scEdit = new ShortcutEdit(this);
  m_scEdit->setEnabled(false);
  m_assignBtn = new QPushButton(tr("Assign"), this);
  m_restoreBtn = new QPushButton(tr("Restore"), this);
  m_assignBtn->setEnabled(false);
  m_restoreBtn->setEnabled(false);
  connect(m_assignBtn, SIGNAL(clicked()), this, SLOT(assign()));
  connect(m_restoreBtn, SIGNAL(clicked()), this, SLOT(restore()));

  // Create ok and cancel buttons:
  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  // Create a layout for the mess:
  QVBoxLayout* mainLayout = new QVBoxLayout();
  QHBoxLayout* presetBox = new QHBoxLayout();
  QHBoxLayout* searchBox = new QHBoxLayout();
  QHBoxLayout* editBox = new QHBoxLayout();
  mainLayout->addLayout(presetBox);
  presetBox->addWidget(presetLabel, Qt::AlignLeft);
  presetBox->addWidget(presetCombo, Qt::AlignCenter);
  presetBox->addWidget(saveBtn, Qt::AlignRight);
  mainLayout->addLayout(searchBox);
  searchBox->addWidget(searchIcon);
  searchBox->addWidget(searchEdit);
  mainLayout->addWidget(m_tableWidget);
  mainLayout->addLayout(editBox);
  editBox->addWidget(keyLabel);
  editBox->addWidget(m_scEdit);
  editBox->addWidget(m_assignBtn);
  editBox->addWidget(m_restoreBtn);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);
}

////////////////////////////////////////////////////////////////////////////////
// ShortcutDialog::itemSelectionChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the shortcut table's selection changed event.
////////////////////////////////////////////////////////////////////////////////
void ShortcutDialog::itemSelectionChanged()
{
  // Get selection:
  QList<QTableWidgetItem*> sel = m_tableWidget->selectedItems();

  // Deselect?
  if (sel.count() == 0)
  {
    // Clear selection and disable the control:
    m_scEdit->clear();
    m_scEdit->setEnabled(false);
    m_assignBtn->setEnabled(false);
    m_restoreBtn->setEnabled(false);
  }

  else
  {
    // Find the action:
    CustomItem* item = static_cast<CustomItem*>(sel[1]);
    if (item != 0)
    {
      // Set contents:
      m_scEdit->setShortcut(item->shortcut());

      // Enable the controls:
      m_scEdit->setEnabled(true);
      m_assignBtn->setEnabled(true);
      m_restoreBtn->setEnabled(item->modified());
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// ShortcutDialog::searchTextChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the search text box changed event.
///\param [in] text: The search text.
////////////////////////////////////////////////////////////////////////////////
void ShortcutDialog::searchTextChanged(QString text)
{
  // Clear selection:
  m_tableWidget->clearSelection();

  // Stop updates while we change the list:
  m_tableWidget->setUpdatesEnabled(false);

  // Got anything?
  if (text.isEmpty())
  {
    // Show all rows:
    for (int i = 0; i < m_tableWidget->rowCount(); i++)
      m_tableWidget->showRow(i);
  }

  else
  {
    // Search items:
    for (int i = 0; i < m_tableWidget->rowCount(); i++)
    {
      // Get stripped text:
      QString str = m_tableWidget->item(i, 1)->text();
      if (str.contains(text, Qt::CaseInsensitive))
        m_tableWidget->showRow(i);
      else
        m_tableWidget->hideRow(i);
    }
  }

  // Update the table:
  m_tableWidget->setUpdatesEnabled(true);
}

////////////////////////////////////////////////////////////////////////////////
// ShortcutDialog::assign()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the assign button clicked event.
////////////////////////////////////////////////////////////////////////////////
void ShortcutDialog::assign()
{
  // Get selection:
  QList<QTableWidgetItem*> sel = m_tableWidget->selectedItems();
  if (sel.count() == 0)
    return;

  // Find the action:
  CustomItem* item = static_cast<CustomItem*>(sel[1]);
  if (item != 0)
  {
    // Set contents:
    item->setShortcut(m_scEdit->shortcut());
    if (sel[2] != 0)
      sel[2]->setText(item->shortcut().toString());

    // Clear selection and disable the controls:
    m_tableWidget->clearSelection();
    m_scEdit->clear();
    m_scEdit->setEnabled(false);
    m_assignBtn->setEnabled(false);
    m_restoreBtn->setEnabled(false);
  }
}

////////////////////////////////////////////////////////////////////////////////
// ShortcutDialog::restore()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the restore button clicked event.
////////////////////////////////////////////////////////////////////////////////
void ShortcutDialog::restore()
{
  // Get selection:
  QList<QTableWidgetItem*> sel = m_tableWidget->selectedItems();
  if (sel.count() == 0)
    return;

  // Find the action:
  CustomItem* item = static_cast<CustomItem*>(sel[1]);
  if (item != 0)
  {
    // Set contents:
    item->setShortcut(item->action()->shortcut());
    if (sel[2] != 0)
      sel[2]->setText(item->shortcut().toString());

    // Clear selection and disable the controls:
    m_scEdit->setShortcut(item->shortcut());
    item->restore();
    m_restoreBtn->setEnabled(false);
  }
}

////////////////////////////////////////////////////////////////////////////////
// ShortcutDialog::accept()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the ok button clicked event.
////////////////////////////////////////////////////////////////////////////////
void ShortcutDialog::accept()
{
  // Apply changes:
  for (int i = 0; i < m_tableWidget->rowCount(); i++)
  {
    CustomItem* item = static_cast<CustomItem*>(m_tableWidget->item(i, 1));
    if (item->modified())
      item->action()->setShortcut(item->shortcut());
  }

  // Base class handling:
  QDialog::accept();
}

////////////////////////////////////////////////////////////////////////////////
// ShortcutDialog::presetChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the preset combo box selection changed event.
///\param [in] newID: The index of the selected preset.
////////////////////////////////////////////////////////////////////////////////
void ShortcutDialog::presetChanged(int /* newID */)
{
}

///////////////////////////////// End of File //////////////////////////////////
