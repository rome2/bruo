////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    bookmarkwidget.h
///\ingroup bruo
///\brief   Bookmark edit control definition.
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
#include "../bruo.h"
#include "bookmarkwidget.h"

////////////////////////////////////////////////////////////////////////////////
///\class BookMarkItem bookmarkwidget.cpp
///\brief Helper class to attach user data to a list item.
////////////////////////////////////////////////////////////////////////////////
class BookMarkItem : public QListWidgetItem
{
public:
  //////////////////////////////////////////////////////////////////////////////
  // BookMarkItem::BookMarkItem()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Initialization constructor of this class.
  ///\param [in] location: The default location of this bookmark.
  ///\param [in] view:     The parent list widget.
  //////////////////////////////////////////////////////////////////////////////
  BookMarkItem(QStandardPaths::StandardLocation location, QListWidget* view = 0) :
    QListWidgetItem(view, QListWidgetItem::UserType + 1),
    m_location(location)
  {
    // Get display text:
    setText(QStandardPaths::displayName(m_location));

    // Get path:
    QStringList locations = QStandardPaths::standardLocations(m_location);
    if (locations.count() > 0)
    {
      m_path = locations[0];

      // Update icon:
      QIcon icon = QFileIconProvider().icon(QFileInfo(m_path));
      if (!icon.isNull())
        setIcon(icon);

      // Set path as tool tip:
      setToolTip(m_path);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // BookMarkItem::BookMarkItem()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Initialization constructor of this class.
  ///\param [in] display: The display string of this bookmark.
  ///\param [in] path:    The path of this bookmark.
  ///\param [in] view:    The parent list widget.
  //////////////////////////////////////////////////////////////////////////////
  BookMarkItem(QString display, QString path, QListWidget* view = 0) :
    QListWidgetItem(view, QListWidgetItem::UserType + 1),
    m_path(path),
    m_location(static_cast<QStandardPaths::StandardLocation>(-1))
  {
    // Set display text:
    setText(display);

    // Update icon:
    QIcon icon = QFileIconProvider().icon(QFileIconProvider::Folder);
    if (!icon.isNull())
      setIcon(icon);

    // Set path as tool tip:
    setToolTip(m_path);

    // Mark this item as editable:
    setFlags(flags() | Qt::ItemIsEditable);
  }

  //////////////////////////////////////////////////////////////////////////////
  // BookMarkItem::path()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Access the path of this item.
  ///\return The currently set path.
  //////////////////////////////////////////////////////////////////////////////
  const QString& path() const
  {
    // Return our path:
    return m_path;
  }

  //////////////////////////////////////////////////////////////////////////////
  // BookMarkItem::location()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Access the location of this item.
  ///\return The currently set location.
  //////////////////////////////////////////////////////////////////////////////
  QStandardPaths::StandardLocation location() const
  {
    // Return our location:
    return m_location;
  }

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QString                          m_path;     ///> The actual path.
  QStandardPaths::StandardLocation m_location; ///> The current location.
};

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::BookmarkWidget()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
BookmarkWidget::BookmarkWidget(QWidget* parent) :
  QListWidget(parent)
{
  // Set properties:
  setSelectionMode(QAbstractItemView::SingleSelection);

  // Enable drag and drop:
  viewport()->setAcceptDrops(true);
  setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::InternalMove);

  // Load entries from file:
  if (!loadBookmarks())
  {
    // Create default entries:
    addItem(new BookMarkItem(QStandardPaths::DesktopLocation));
    addItem(new BookMarkItem(QStandardPaths::HomeLocation));
    addItem(new BookMarkItem(QStandardPaths::DocumentsLocation));
    addItem(new BookMarkItem(QStandardPaths::MusicLocation));
    addItem(new BookMarkItem(QStandardPaths::MoviesLocation));
  }

  // Connect events:
  connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(bookmarkChanged(QListWidgetItem*)));
}

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::~BookmarkWidget()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Cleans up used resources.
////////////////////////////////////////////////////////////////////////////////
BookmarkWidget::~BookmarkWidget()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::keyPressEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Reimplemented keypress event handler.
///\param [in] event: Provides details about the event.
////////////////////////////////////////////////////////////////////////////////
void BookmarkWidget::keyPressEvent(QKeyEvent* event)
{
  // Base class handling:
  QListWidget::keyPressEvent(event);

  // Was the delete key pressed:
  if (event->key() == Qt::Key_Delete)
  {
    // Get the current item:
    BookMarkItem* item = static_cast<BookMarkItem*>(currentItem());
    if (item)
    {
      // Is this a predefined location?
      if (item->location() >= 0)
        return;

      // Remove the item:
      takeItem(row(item));

      // Save list:
      saveBookmarks();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::selectionChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Reimplemented selection changed event handler.
///\param [in] selected:   Currently selected item.
///\param [in] deselected: Previously selected item.
////////////////////////////////////////////////////////////////////////////////
void BookmarkWidget::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
  // Base class handling:
  QListWidget::selectionChanged(selected, deselected);

  // Selected anything?
  if (selected.empty())
    return;

  // Get the selected item:
  BookMarkItem* bm = (BookMarkItem*)item(selected.indexes().at(0).row());
  if (bm)
  {
    // Notify listeners:
    emit bookmarkSelected(bm->path());
  }
}

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::dragEnterEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Reimplemented drag enter event handler.
///\param [in] event: Provides details about the event.
////////////////////////////////////////////////////////////////////////////////
void BookmarkWidget::dragEnterEvent(QDragEnterEvent* event)
{
  // Check mime type:
  if (!event->mimeData()->hasFormat("text/uri-list"))
    return;

  // Get the dropped files:
  QList<QUrl> urls = event->mimeData()->urls();
  if (urls.count() != 1)
    return;

  // Extract file name;
  QString fileName = urls.first().toLocalFile();
  if (fileName.isEmpty())
    return;

  // Check if this is a folder:
  if (!QFileInfo(fileName).isDir())
    return;

  // Acceppt this file:
  event->acceptProposedAction();
}

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::dropEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Reimplemented drop event handler.
///\param [in] event: Provides details about the event.
////////////////////////////////////////////////////////////////////////////////
void BookmarkWidget::dropEvent(QDropEvent* event)
{
  // Get the dropped files:
  QList<QUrl> urls = event->mimeData()->urls();
  if (urls.isEmpty())
    return;

  // Extract file name;
  QString fileName = urls.first().toLocalFile();
  if (fileName.isEmpty())
    return;

  // Check if this is a folder:
  if (!QFileInfo(fileName).isDir())
    return;

  // Check if this is already in the list:
  for (int i = 0; i < count(); i++)
  {
    BookMarkItem* bookmark = static_cast<BookMarkItem*>(item(i));
    if (fileName.compare(bookmark->path(), Qt::CaseInsensitive) == 0)
      return;
  }

  // Get display string:
  QString display = QFileInfo(fileName).baseName();
  if (display.isEmpty())
    display = fileName;
  if (display.isEmpty())
    display = tr("Unnamed folder");

  // Create new bookmark item:
  addItem(new BookMarkItem(display, fileName));

  // Save list:
  saveBookmarks();
}

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::contextMenuEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Reimplemented context menu event handler.
///\param [in] event: Provides details about the event.
////////////////////////////////////////////////////////////////////////////////
void BookmarkWidget::contextMenuEvent(QContextMenuEvent* event)
{
  // Get item under cursor:
  BookMarkItem* item = static_cast<BookMarkItem*>(itemAt(event->pos()));
  if (!item)
    return;

  // Build the menu:
  QMenu myMenu;
  QAction* deleteAction = myMenu.addAction(QIcon(":/images/edit-delete.png"), tr("Remove item"));
  QAction* editAction   = myMenu.addAction(QIcon(":/images/edit-text.png"), tr("Edit title"));

  // Is this a predefined location?
  if (item->location() >= 0)
  {
    deleteAction->setEnabled(false);
    editAction->setEnabled(false);
  }

  // Get screen coordinates for the menu:
  QPoint globalPos = viewport()->mapToGlobal(event->pos());

  // Show the meny:
  QAction* action = myMenu.exec(globalPos);
  if (!action)
    return;

  // Edit the title?
  if (action == editAction)
    editItem(item);

  // Delete the item?
  else if (action == deleteAction)
    takeItem(row(item));
}

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::bookmarkChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Item changed event handler.
///\param [in] item: The changed item.
////////////////////////////////////////////////////////////////////////////////
void BookmarkWidget::bookmarkChanged(QListWidgetItem* /* item */)
{
  // Save list:
  saveBookmarks();
}

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::loadBookmarks()
////////////////////////////////////////////////////////////////////////////////
///\brief   Load the bookmarks from the settings file.
///\returns true if successful or false otherwise.
////////////////////////////////////////////////////////////////////////////////
bool BookmarkWidget::loadBookmarks()
{
  // Find the bookmarks file:
  QFile file(getSettingsPath() + "bookmarks.xml");
  if (!file.exists())
    return false;

  // Open the file:
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Error while reading the key map: File not found: " << file.fileName();
    return false;
  }

  QList<BookMarkItem*> bookmarks;

  // Parse the file:
  QXmlStreamReader xml(&file);
  while (!xml.atEnd() && !xml.hasError())
  {
    // Read next element:
    QXmlStreamReader::TokenType token = xml.readNext();

    // If token is just StartDocument, we'll go to next:
    if (token == QXmlStreamReader::StartDocument)
      continue;

    // If token is StartElement, we'll see if we can read it:
    if (token == QXmlStreamReader::StartElement)
    {
      // If it's a bookmark, we'll dig the information from there:
      if (xml.name() == "bookmark")
      {
        // Get the attributes:
        QXmlStreamAttributes attributes = xml.attributes();
        if (attributes.hasAttribute("location"))
        {
          // Get the location:
          int location = attributes.value("location").toString().toInt();

          // If it's negative then it's a custom value:
          if (location < 0)
          {
            QString path, display;
            if (attributes.hasAttribute("path"))
              path = attributes.value("path").toString();
            if (attributes.hasAttribute("display"))
              display = attributes.value("display").toString();
            bookmarks.append(new BookMarkItem(display, path));
          }
          else
            bookmarks.append(new BookMarkItem(static_cast<QStandardPaths::StandardLocation>(location)));
        }
      }
    }
  }

  // Got errors?
  bool error = xml.hasError();
  if (error)
    qDebug() << "Error while reading the bookmarks from " << file.fileName();
  else
  {
    // Add bookmarks:
    for (int i = 0; i < bookmarks.count(); i++)
      addItem(bookmarks[i]);
  }

  // Close file:
  xml.clear();
  file.close();

  // Return reader result:
  return !error && !bookmarks.empty();
}

////////////////////////////////////////////////////////////////////////////////
// BookmarkWidget::saveBookmarks()
////////////////////////////////////////////////////////////////////////////////
///\brief   Save the bookmarks to the settings file.
///\returns true if successful or false otherwise.
////////////////////////////////////////////////////////////////////////////////
bool BookmarkWidget::saveBookmarks()
{
  // Open the file:
  QFile file(getSettingsPath() + "bookmarks.xml");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    qDebug() << "Error while writing the bookmarks to " << file.fileName();
    return false;
  }

  // Write the file:
  QXmlStreamWriter xml(&file);

  // Start the file:
  xml.setAutoFormatting( true );
  xml.writeStartDocument();
  xml.writeStartElement("bookmarks");

  // Write the bookmarks:
  for (int i = 0; i < count(); i++)
  {
    BookMarkItem* bookmark = static_cast<BookMarkItem*>(item(i));
    xml.writeEmptyElement("bookmark");
    xml.writeAttribute("location", QString::number(static_cast<int>(bookmark->location())));
    xml.writeAttribute("display", bookmark->text());
    xml.writeAttribute("path", bookmark->path());
  }

  // Done:
  xml.writeEndDocument();

  // Got errors?
  bool error = xml.hasError();
  if (error)
    qDebug() << "Error while writing the bookmarks to " << file.fileName();

  // Close file:
  file.close();

  // Return reader result:
  return !error;
}

///////////////////////////////// End of File //////////////////////////////////
