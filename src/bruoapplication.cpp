////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    bruoapplication.cpp
///\ingroup bruo
///\brief   Application object implementation.
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
#include "bruoapplication.h"

////////////////////////////////////////////////////////////////////////////////
// BruoApplication::BruoApplication()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this class.
///\brief   [in] argc: Number of program arguments.
///\param   [in] argv: The actual arguments.
///\remarks Just initializes the members but does not start anything (audio
///         engine etc). This has to be done later.
////////////////////////////////////////////////////////////////////////////////
BruoApplication::BruoApplication(int argc, char** argv) :
  QApplication(argc, argv)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// BruoApplication::~BruoApplication()
////////////////////////////////////////////////////////////////////////////////
///\brief   Default destructor of this class.
///\remarks Frees all used resources.
////////////////////////////////////////////////////////////////////////////////
BruoApplication::~BruoApplication()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// BruoApplication::audioSystem()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the audio system of this application.
///\return  A reference to the global audio system.
////////////////////////////////////////////////////////////////////////////////
AudioSystem& BruoApplication::audioSystem()
{
  // Return the system:
  return m_audioSystem;
}

////////////////////////////////////////////////////////////////////////////////
// BruoApplication::audioSystem()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the audio system of this application, const version.
///\return  A reference to the global audio system.
////////////////////////////////////////////////////////////////////////////////
const AudioSystem& BruoApplication::audioSystem() const
{
  // Return the system:
  return m_audioSystem;
}

///////////////////////////////// End of File //////////////////////////////////
