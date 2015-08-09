////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    browsertoolwindow.cpp
///\ingroup bruo
///\brief   File browser tool window implementation.
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
#include "browsertoolwindow.h"
#include "controls/bookmarkwidget.h"

////////////////////////////////////////////////////////////////////////////////
// BrowserToolWindow::BrowserToolWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
BrowserToolWindow::BrowserToolWindow(QWidget* parent) :
  QDockWidget(tr("File browser"), parent)
{
  // Set constraints:
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  // Name for position serialization:
  setObjectName("BrowserToolWindow");

  // List view for the bookmarks:
  BookmarkWidget* bookmarkView = new BookmarkWidget();
  connect(bookmarkView, SIGNAL(bookmarkSelected(const QString&)), this, SLOT(bookmarkSelected(const QString&)));

  // File model:
  QFileSystemModel* model = new QFileSystemModel;
  model->setRootPath(QDir::currentPath());

  // File tree view:
  m_fileTree = new QTreeView();
  m_fileTree->setModel(model);
  m_fileTree->setColumnWidth(0, m_fileTree->columnWidth(0) * 3);
  m_fileTree->setDragEnabled(true);

  // Preview progress bar:
  QProgressBar* progress = new QProgressBar();
  progress->setOrientation(Qt::Horizontal);
  progress->setTextVisible(false);
  progress->setMaximumHeight(10);

  // Player button:
  QPushButton* playButton = new QPushButton();
  playButton->setFixedSize(30, 30);
  playButton->setIcon(QIcon(":/images/media-playback-start.png"));
  playButton->setIconSize(QSize(20, 20));
  playButton->setFlat(true);
  playButton->setToolTip(tr("Start playback"));

  // Stop button:
  QPushButton* stopButton = new QPushButton();
  stopButton->setFixedSize(30, 30);
  stopButton->setIcon(QIcon(":/images/media-playback-stop.png"));
  stopButton->setIconSize(QSize(20, 20));
  stopButton->setFlat(true);
  stopButton->setToolTip(tr("Stop playback"));

  // Loop button:
  QPushButton* loopButton = new QPushButton();
  loopButton->setFixedSize(30, 30);
  loopButton->setIcon(QIcon(":/images/media-loop.png"));
  loopButton->setIconSize(QSize(20, 20));
  loopButton->setCheckable(true);
  loopButton->setFlat(true);
  loopButton->setToolTip(tr("Loop on/off"));

  // No volume label:
  QLabel* image1 = new QLabel();
  image1->setPixmap(QPixmap(":/images/volume-low.png"));
  image1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  image1->setFixedSize(30, 30);

  // Volume slider:
  QSlider* volumeSlider = new QSlider();
  volumeSlider->setOrientation(Qt::Horizontal);

  // Full volume label:
  QLabel* image2 = new QLabel();
  image2->setPixmap(QPixmap(":/images/volume-full.png"));
  image2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  image2->setFixedSize(30, 30);

  // Grid for the preview:
  QGridLayout* grid = new QGridLayout();
  grid->setMargin(0);
  grid->addWidget(progress, 0, 0, 1, 6);
  grid->addWidget(playButton, 1, 0, 1, 1);
  grid->addWidget(stopButton, 1, 1, 1, 1);
  grid->addWidget(loopButton, 1, 2, 1, 1);
  grid->addWidget(image1, 1, 3, 1, 1);
  grid->addWidget(volumeSlider, 1, 4, 1, 1);
  grid->addWidget(image2, 1, 5, 1, 1);
  grid->setColumnStretch(4, 1);

  // Panel for the preview controls:
  QWidget* preview = new QWidget();
  preview->setMinimumHeight(40);
  preview->setLayout(grid);

  // Group box for the file tree:
  QGroupBox* fileBox = new QGroupBox();
  fileBox->setContentsMargins(0, 0, 0, 0);
  fileBox->setGeometry(0, 0, 100, 600);

  // Layout for the file tree:
  QVBoxLayout* fileLayout = new QVBoxLayout();
  fileLayout->setMargin(0);
  fileLayout->setSpacing(0);
  fileLayout->setContentsMargins(0, 0, 0, 0);
  fileLayout->addWidget(m_fileTree);
  fileLayout->addWidget(preview);
  fileBox->setLayout(fileLayout);

  // Splitter for the bookmark box and the file tree:
  QSplitter* splitter = new QSplitter();
  splitter->setOrientation(Qt::Vertical);
  splitter->addWidget(bookmarkView);
  splitter->addWidget(fileBox);

  // Set the splitter as dock child:
  setWidget(splitter);
}

////////////////////////////////////////////////////////////////////////////////
// BrowserToolWindow::~BrowserToolWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Cleans up used resources.
////////////////////////////////////////////////////////////////////////////////
BrowserToolWindow::~BrowserToolWindow()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// BrowserToolWindow::bookmarkSelected()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the bookmark window selection changed signal.
///\param   [in] path: Path of the selected bookmark.
///\remarks Updates the current selection of the tree view.
////////////////////////////////////////////////////////////////////////////////
void BrowserToolWindow::bookmarkSelected(const QString& path)
{
  // Parameter check:
  if (path.isEmpty())
    return;

  // Update file tree:
  QModelIndex index = static_cast<QFileSystemModel*>(m_fileTree->model())->index(path);
  m_fileTree->setCurrentIndex(index);
  m_fileTree->expand(index);
}

///////////////////////////////// End of File //////////////////////////////////
