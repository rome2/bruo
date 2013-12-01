////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    audiosnippet.h
///\ingroup bruo
///\brief   Playlist item class definition.
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
#ifndef __AUDIOSNIPPET_H_INCLUDED__
#define __AUDIOSNIPPET_H_INCLUDED__

#include <qglobal.h>
#include "bruo.h"
#include "samplebuffer.h"

////////////////////////////////////////////////////////////////////////////////
///\class   AudioSnippet audiosnippet.h
///\brief   Base class for all audio snippets in a document's play list.
////////////////////////////////////////////////////////////////////////////////
class AudioSnippet
{
public:
  //////////////////////////////////////////////////////////////////////////////
  // AudioSnippet::AudioSnippet()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this class.
  ///\param   [in] numSamples: The number of sample frames of this document.
  //////////////////////////////////////////////////////////////////////////////
  AudioSnippet(qint64 numSamples);

  //////////////////////////////////////////////////////////////////////////////
  // AudioSnippet::~AudioSnippet()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this class.
  ///\remarks Does final cleanup.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~AudioSnippet();

  //////////////////////////////////////////////////////////////////////////////
  // AudioSnippet::sampleCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the total sample count of this snippet.
  ///\return  The sample count.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         count it doesn't matter how many channels there are.
  //////////////////////////////////////////////////////////////////////////////
  virtual qint64 sampleCount() const;

  //////////////////////////////////////////////////////////////////////////////
  // AudioSnippet::readSamples()
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
  qint64 m_sampleCount; ///> Number of samples.
};

#endif // #ifndef __AUDIOSNIPPET_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
