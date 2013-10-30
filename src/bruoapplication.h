////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    bruoapplication.h
///\ingroup bruo
///\brief   Application object include file.
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
#ifndef __BRUOAPPLICATION_H_INCLUDED__
#define __BRUOAPPLICATION_H_INCLUDED__

#include <QApplication>
#include "audio/audiodevice.h"

////////////////////////////////////////////////////////////////////////////////
///\class   BruoApplication bruoapplication.h
///\brief   Extended QApplication class.
///\remarks This class is the main class of this application. It is instantiated
///         in the main function so never create new instances aqnywhere else.
///         If you want to access the global application object use the qBruoApp
///         macro below.
////////////////////////////////////////////////////////////////////////////////
class BruoApplication : public QApplication
{
  Q_OBJECT // Qt Magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // BruoApplication::BruoApplication()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this class.
  ///\brief   [in] argc: Number of program arguments.
  ///\param   [in] argv: The actual arguments.
  ///\remarks Just initializes the members but does not start anything (audio
  ///         engine etc). This has to be done later.
  //////////////////////////////////////////////////////////////////////////////
  BruoApplication(int argc, char** argv);

  //////////////////////////////////////////////////////////////////////////////
  // BruoApplication::~BruoApplication()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default destructor of this class.
  ///\remarks Frees all used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~BruoApplication();

  //////////////////////////////////////////////////////////////////////////////
  // BruoApplication::audioSystem()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the audio system of this application.
  ///\return  A reference to the global audio system.
  //////////////////////////////////////////////////////////////////////////////
  AudioSystem& audioSystem();

  //////////////////////////////////////////////////////////////////////////////
  // BruoApplication::audioSystem()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the audio system of this application, const version.
  ///\return  A reference to the global audio system.
  //////////////////////////////////////////////////////////////////////////////
  const AudioSystem& audioSystem() const;

 private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  AudioSystem m_audioSystem; ///> The application's audio system.
};

// Custom replacement for Qt's qApp macro:
#define qBruoApp (static_cast<BruoApplication*>(QCoreApplication::instance()))

#endif // #ifndef __BRUOAPPLICATION_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
