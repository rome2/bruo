////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    sndfilesnippet.h
///\ingroup bruo
///\brief   LIBSND file based playlist item class definition.
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
#ifndef __SNDFILESNIPPET_H__INCLUDED__
#define __SNDFILESNIPPET_H__INCLUDED__

#include "audiosnippet.h"

////////////////////////////////////////////////////////////////////////////////
///\class   SndFileSnippet sndfilesnippet.h
///\brief   libsnd file based audio snippet for the document's play list.
////////////////////////////////////////////////////////////////////////////////
class SndFileSnippet :
  public AudioSnippet
{
public:

  //////////////////////////////////////////////////////////////////////////////
  // SndFileSnippet::SndFileSnippet()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this class.
  ///\param   [in] handle:     The SNDFILE handle.
  ///\param   [in] numSamples: The number of sample frames of this document.
  //////////////////////////////////////////////////////////////////////////////
  SndFileSnippet(void* handle, qint64 numSamples);

  //////////////////////////////////////////////////////////////////////////////
  // SndFileSnippet::~SndFileSnippet()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this class.
  ///\remarks Does final cleanup.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~SndFileSnippet();

  //////////////////////////////////////////////////////////////////////////////
  // SndFileSnippet::readSamples()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Read a number of samples frames from this snippet.
  ///\param   [in]  offset: Position where to start reading.
  ///\param   [in]  count:  Number of sample frames to read.
  ///\param   [out] buffer: The target buffer for the samples.
  ///\return  The number of samples frames read.
  ///\remarks If there are no more samples to read zero is returned.
  //////////////////////////////////////////////////////////////////////////////
  virtual qint64 readSamples(const qint64 offset, const qint64 count, SampleBuffer& buffer);

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  void*   m_handle;     ///> The SND file handle.
  double* m_tempBuffer; ///> Temporary buffer.
  size_t  m_tempSize;   ///> Size of the temporary buffer.
};

#endif // #ifndef __SNDFILESNIPPET_H__INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
