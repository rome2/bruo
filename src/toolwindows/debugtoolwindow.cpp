////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    debugtoolwindow.cpp
///\ingroup bruo
///\brief   Debug view tool window implementation.
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
#include "debugtoolwindow.h"
#include "settings/loggingsystem.h"

////////////////////////////////////////////////////////////////////////////////
// DebugToolWindow::DebugToolWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
DebugToolWindow::DebugToolWindow(QWidget* parent) :
  QDockWidget(tr("Debug"), parent)
{
  // Set constraints:
  setAllowedAreas(Qt::BottomDockWidgetArea);

  // Name for position serialization:
  setObjectName("debugToolWindow");

  // Create the actual view:
  QTextEdit* text = new QTextEdit(this);
  text->setReadOnly(true);
  text->setLineWrapMode(QTextEdit::NoWrap);
  text->setFontFamily("Courier");

  // Set to white/black look:
  text->setTextColor(QColor(255, 255, 255));
  QColor bgColor(0, 0, 0);
  QPalette pal;
  pal.setColor(QPalette::Base, bgColor);
  text->setPalette(pal);

  // Set the text as dock child:
  setWidget(text);

  // Set as debug target:
  LoggingSystem::setOutputWindow(text);
}

////////////////////////////////////////////////////////////////////////////////
// DebugToolWindow::~DebugToolWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Cleans up used resources.
////////////////////////////////////////////////////////////////////////////////
DebugToolWindow::~DebugToolWindow()
{
  // Remove as debug target:
  LoggingSystem::setOutputWindow(0);
}

///////////////////////////////// End of File //////////////////////////////////
