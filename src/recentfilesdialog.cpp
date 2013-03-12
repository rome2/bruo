#include "recentfilesdialog.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QStringListModel>

RecentFilesDialog::RecentFilesDialog(const QStringList& files, QWidget* parent) :
  QDialog(parent),
  m_selectedItem(-1)
{
  // Init the dialog:
  setWindowTitle(tr("Select file"));
  setSizeGripEnabled(true);
  resize(400, 300);

  // Create and fill the list box:
  QListWidget* listBox = new QListWidget(this);
  listBox->addItems(files);
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

int RecentFilesDialog::selectedItem() const
{
  // Return the requested item:
  return m_selectedItem;
}

void RecentFilesDialog::listBoxSelectionChanged()
{
  // Get the list box:
  QListWidget* listBox = qobject_cast<QListWidget*>(sender());
  if (listBox == 0)
    return;

  // Get selection:
  m_selectedItem = listBox->currentRow();
}

void RecentFilesDialog::listBoxItemDoubleClicked(QListWidgetItem* item)
{
  // Get the list box:
  QListWidget* listBox = qobject_cast<QListWidget*>(sender());
  if (listBox == 0)
    return;

  // Get the item's index:
  m_selectedItem = listBox->row(item);

  // Close the dialog:
  accept();
}
