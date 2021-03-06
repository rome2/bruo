////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    main.cpp
///\ingroup bruo
///\brief   Main application entry point.
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
#include "version.h"
#include "mainframe.h"
#include "settings/loggingsystem.h"

////////////////////////////////////////////////////////////////////////////////
// main()
////////////////////////////////////////////////////////////////////////////////
///\brief   Main application entry point.
///\param   [in] argc: Number of command line arguments passed to this program.
///\param   [in] argv: Array of command line arguments.
///\return  Returns zero if successfull or an error code on failure.
///\remarks The first argument holds the path to the executable.
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  // Install debug handler:
  LoggingSystem::prepare();

  // Init the global application object:
  QApplication a(argc, argv);

  // Set programm properties, used by QSettings and others:
  QSettings::setDefaultFormat(QSettings::IniFormat);
  a.setApplicationName(VER_PRODUCTNAME_STR);
  a.setApplicationVersion(VER_PRODUCTVERSION_STR);
  a.setOrganizationName(VER_COMPANYNAME_STR);
  a.setOrganizationDomain(VER_COMPANYDOMAIN_STR);
  a.setWindowIcon(QIcon(":/images/icon128.png"));

  // Start logging:
  LoggingSystem::start();

  // Init theme:
  if (QSettings().value("darkTheme", QVariant(false)).toBool())
    toggleDarkTheme(true);

  // Create and show the main application window:
  MainFrame w;
  w.show();

  // Run the application:
  int ret = a.exec();

  // Return to sender:
  return ret;
}

///////////////////////////////// End of File //////////////////////////////////
