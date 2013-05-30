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
#if QT_VERSION >= 0x050000
  QStringList paths =  QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
  for (int i = 0; i < paths.count(); i++)
  {
    paths[i].append(QDir::separator());
    paths[i].append(QSettings().applicationName());
    paths[i].append(QDir::separator());
  }
  return paths;
#else
  return QDesktopServices.storageLocation();
#endif
}

///////////////////////////////// End of File //////////////////////////////////
