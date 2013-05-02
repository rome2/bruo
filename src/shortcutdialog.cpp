#include "bruo.h"
#include "shortcutdialog.h"
#include "controls/shortcutedit.h"

ShortcutDialog::ShortcutDialog(QHash<QString, QAction*>& actionMap, QWidget* parent) :
  QDialog(parent)
{
  // Init the dialog:
  setWindowTitle(tr("Configure shortcuts"));
  setSizeGripEnabled(true);
  resize(640, 480);

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
    if (!action->icon().isNull())
    {
      item = new QTableWidgetItem();
      item->setIcon(action->icon());
      item->setFlags(item->flags() & (~Qt::ItemIsEditable));
      m_tableWidget->setItem(row, 0, item);
    }

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

  // Create ok and cancel buttons:
  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  // Create a layout for the mess:
  QVBoxLayout* mainLayout = new QVBoxLayout();
  QHBoxLayout* searchBox = new QHBoxLayout();
  QHBoxLayout* editBox = new QHBoxLayout();
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
    // Enable the controls:
    m_scEdit->setEnabled(true);
    m_assignBtn->setEnabled(true);
    m_restoreBtn->setEnabled(true);
  }
}

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
