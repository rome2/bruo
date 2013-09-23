////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    bruo.h
///\ingroup bruo
///\brief   Main application include file.
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
#ifndef __BRUO_H_INCLUDED__
#define __BRUO_H_INCLUDED__

#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#include <QtGui/QGuiApplication>
#else
#include <QApplication>
#endif
#include <QtGlobal>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QDialog>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QClipboard>
#include <QFileSystemModel>
#include <QTreeWidget>

////////////////////////////////////////////////////////////////////////////////
// getSettingsPath()
////////////////////////////////////////////////////////////////////////////////
///\brief   Find the path where the settings and other user data are stored.
///\return  The settings path or an empty string if the path cannot be found.
///\remarks The path includes the trailing slash. The settings path differs on
///         every paltform. Please look at the Qt docs for the details.
////////////////////////////////////////////////////////////////////////////////
QString getSettingsPath();

////////////////////////////////////////////////////////////////////////////////
// getLibraryPaths()
////////////////////////////////////////////////////////////////////////////////
///\brief   Find the paths where the application's additional data is stored.
///\return  The base paths or an empty list if the paths cannot be found.
///\remarks The paths includes the trailing slash. The paths differs on
///         every paltform. Please look at the Qt docs for the details.
////////////////////////////////////////////////////////////////////////////////
QStringList getLibraryPaths();

#endif // #ifndef __BRUO_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
