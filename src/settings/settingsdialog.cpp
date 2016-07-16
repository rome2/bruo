#include "bruo.h"
#include "../mainframe.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(MainFrame* parent) :
  QDialog(parent),
  m_filterLineEdit(0),
  m_categoryList(0),
  m_headerLabel(0),
  m_stackedLayout(0)
{
  // Line edit for searching/filtering:
  m_filterLineEdit = new QLineEdit();

  // Category list view:
  m_categoryList = new QListWidget();
  m_categoryList->setViewMode(QListView::ListMode);
  m_categoryList->setIconSize(QSize(24, 24));
  m_categoryList->setMovement(QListView::Static);
  m_categoryList->setMaximumWidth(128);
  m_categoryList->setSpacing(12);

  // Header label:
  m_headerLabel = new QLabel();
  m_headerLabel->setText(tr("No page"));
  QFont headerLabelFont = m_headerLabel->font();
  headerLabelFont.setBold(true);
  int pointSize = headerLabelFont.pointSize();
  if (pointSize > 0)
    headerLabelFont.setPointSize(pointSize + 2);
  m_headerLabel->setFont(headerLabelFont);

  // Layout item for the header:
  QHBoxLayout* headerHLayout = new QHBoxLayout();
  int leftMargin = qApp->style()->pixelMetric(QStyle::PM_LayoutLeftMargin);
  headerHLayout->addSpacerItem(new QSpacerItem(leftMargin, 0, QSizePolicy::Fixed, QSizePolicy::Ignored));
  headerHLayout->addWidget(m_headerLabel);

  // Stacked layout for the dialog pages:
  m_stackedLayout = new QStackedLayout();
  m_stackedLayout->setMargin(0);

  // Button box:
  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel);
  buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
  connect(buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  // Layout for the dialog:
  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->addWidget(m_filterLineEdit, 0, 0, 1, 1);
  mainGridLayout->addLayout(headerHLayout,    0, 1, 1, 1);
  mainGridLayout->addWidget(m_categoryList,   1, 0, 1, 1);
  mainGridLayout->addLayout(m_stackedLayout,  1, 1, 1, 1);
  mainGridLayout->addWidget(buttonBox,        2, 0, 1, 2);
  mainGridLayout->setColumnStretch(1, 4);
  setLayout(mainGridLayout);
  setMinimumSize(800, 550);

  // Add pages:
}

void SettingsDialog::apply()
{
}
