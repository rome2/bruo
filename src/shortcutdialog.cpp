#include "bruo.h"
#include "shortcutdialog.h"

ShortcutDialog::ShortcutDialog(QHash<QString, QAction*>& actionMap, QWidget* parent) :
  QDialog(parent),
  m_actionMap(actionMap)
{
  // Init the dialog:
  setWindowTitle(tr("Configure shortcuts"));
  setSizeGripEnabled(true);
  resize(640, 480);

  QListWidget* listBox = new QListWidget(this);

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

    // Create item:
    QListWidgetItem* item = new QListWidgetItem(str);

    // Add status text:
    item->setStatusTip(action->statusTip());
    item->setToolTip(action->statusTip());

    // Add icon:
    if (action->icon().isNull())
      item->setIcon(QIcon(":/images/empty.png"));
    else
      item->setIcon(action->icon());

    // Add to the list:
    listBox->addItem(item);
  }

  // Sort the list:
  listBox->sortItems();

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
