////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    peakthread.cpp
///\ingroup bruo
///\brief   Peak update class implementation.
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
#include "peakthread.h"
#include "document.h"

////////////////////////////////////////////////////////////////////////////////
// PeakThread::PeakThread()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this class.
///\param   [in] doc: The document that we are working on.
////////////////////////////////////////////////////////////////////////////////
PeakThread::PeakThread(class Document* doc) :
  m_doc(doc)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// PeakThread::run()
////////////////////////////////////////////////////////////////////////////////
///\brief The actual thread function.
////////////////////////////////////////////////////////////////////////////////
void PeakThread::run()
{
  // Update the peaks:
  m_doc->updatePeakData();
}

///////////////////////////////// End of File //////////////////////////////////
