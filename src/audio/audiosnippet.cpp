////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    audiosnippet.cpp
///\ingroup bruo
///\brief   Playlist item class implementation.
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
#include "audiosnippet.h"

////////////////////////////////////////////////////////////////////////////////
// AudioSnippet::AudioSnippet()
////////////////////////////////////////////////////////////////////////////////
///\brief Initialization constructor of this class.
///\param [in] numSamples: The number of sample frames of this snippet.
////////////////////////////////////////////////////////////////////////////////
AudioSnippet::AudioSnippet(qint64 numSamples) :
  m_sampleCount(numSamples)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// AudioSnippet::~AudioSnippet()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this class.
///\remarks Does final cleanup.
////////////////////////////////////////////////////////////////////////////////
AudioSnippet::~AudioSnippet()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// AudioSnippet::sampleCount()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the total sample count of this snippet.
///\return  The sample count.
///\remarks Samples are only counted for a single channel here so for the
///         count it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
qint64 AudioSnippet::sampleCount() const
{
  // Return our sample count:
  return m_sampleCount;
}

////////////////////////////////////////////////////////////////////////////////
// AudioSnippet::readSamples()
////////////////////////////////////////////////////////////////////////////////
///\brief   Read a number of samples frames from this snippet.
///\param   [in]  offset: Position where to start reading.
///\param   [in]  count:  Number of sample frames to read.
///\param   [out] buffer: The target buffer for the samples.
///\return  The number of samples frames read.
///\remarks If there are no more samples to read zero is returned.
////////////////////////////////////////////////////////////////////////////////
qint64 AudioSnippet::readSamples(const qint64 /* offset */, const qint64 /* count */, SampleBuffer& /* buffer */)
{
  // Returns always zero:
  return 0;
}

///////////////////////////////// End of File //////////////////////////////////
