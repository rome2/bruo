////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    bruo.cpp
///\ingroup bruo
///\brief   Global functions.
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

#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#endif

////////////////////////////////////////////////////////////////////////////////
// getSettingsPath()
////////////////////////////////////////////////////////////////////////////////
///\brief   Find the path where the settings and other user data are stored.
///\return  The settings path or an empty string if the path cannot be found.
///\remarks The path includes the trailing slash. The settings path differs on
///         every paltform. Please look at the Qt docs for the details.
////////////////////////////////////////////////////////////////////////////////
QString getSettingsPath()
{
  // Get log file name:
  QString fileName = QSettings().fileName();
  if (fileName.isEmpty())
    return "";

  // Compose path:
  fileName = QFileInfo(fileName).path() + QDir::separator();

  // Create path if needed:
  QString path = QFileInfo(fileName).path();
  if (!QDir(path).exists())
    QDir().mkpath(path);

  // Return the file name:
  return fileName;
}

////////////////////////////////////////////////////////////////////////////////
// getLibraryPaths()
////////////////////////////////////////////////////////////////////////////////
///\brief   Find the paths where the application's additional data is stored.
///\return  The base paths or an empty list if the paths cannot be found.
///\remarks The paths includes the trailing slash. The paths differs on
///         every paltform. Please look at the Qt docs for the details.
////////////////////////////////////////////////////////////////////////////////
QStringList getLibraryPaths()
{
  QString suffix(QDir::separator());
  suffix.append(QSettings().applicationName());
  suffix.append(QDir::separator());

#if QT_VERSION >= 0x050000
  QStringList paths = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
  for (int i = 0; i < paths.count(); i++)
    paths[i].append(suffix);
  return paths;
#else
  QStringList paths;
  paths.append(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + suffix);
  return paths;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// toggleDarkTheme()
////////////////////////////////////////////////////////////////////////////////
///\brief   Switch to a dark theme and back again.
///\param   [in] darkTheme: Use dark theme?
///\remarks The first argument holds the path to the executable.
////////////////////////////////////////////////////////////////////////////////
void toggleDarkTheme(bool darkTheme)
{
  // Default state on application start:
  static bool firstToggle = true;
  static QStyle* oldStyle = 0;
  static QPalette oldPalette;
  static QString oldStyleSheet;

  // Store default values on first call:
  if (firstToggle)
  {
    oldStyle      = qApp->style();
    oldPalette    = qApp->palette();
    oldStyleSheet = qApp->styleSheet();
    firstToggle = false;
  }

  // Switch to dark theme:
  if (darkTheme)
  {
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, QColor(192,192,192));
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, QColor(192,192,192));
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, QColor(192,192,192));
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #000000; background-color: #ffffff; border: 1px solid black; } QWidget:disabled { color: #404040; background-color: #323232; }");
  }

  // Restore default view:
  else
  {
    qApp->setStyle(0);
    qApp->setPalette(oldPalette);
    qApp->setStyleSheet(oldStyleSheet);
  }
}

///////////////////////////////// End of File //////////////////////////////////
