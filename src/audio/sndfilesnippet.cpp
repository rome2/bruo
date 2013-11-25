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
#include "sndfilesnippet.h"
#include <sndfile.h>

////////////////////////////////////////////////////////////////////////////////
// SndFileSnippet::SndFileSnippet()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this class.
///\param   [in] handle:     The SNDFILE handle.
///\param   [in] numSamples: The number of sample frames of this document.
////////////////////////////////////////////////////////////////////////////////
SndFileSnippet::SndFileSnippet(void* handle, qint64 numSamples) :
  AudioSnippet(numSamples),
  m_handle(handle),
  m_tempBuffer(0),
  m_tempSize(0)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// SndFileSnippet::~SndFileSnippet()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this class.
///\remarks Does final cleanup.
////////////////////////////////////////////////////////////////////////////////
SndFileSnippet::~SndFileSnippet()
{
  // Clear temp buffer:
  if (m_tempBuffer != 0)
    delete [] m_tempBuffer;
  m_tempBuffer = 0;
  m_tempSize   = 0;
}

////////////////////////////////////////////////////////////////////////////////
// SndFileSnippet::readSamples()
////////////////////////////////////////////////////////////////////////////////
///\brief   Read a number of samples frames from this snippet.
///\param   [in]  offset: Position where to start reading.
///\param   [in]  count:  Number of sample frames to read.
///\param   [out] buffer: The target buffer for the samples.
///\return  The number of samples frames read.
///\remarks If there are no more samples to read zero is returned.
////////////////////////////////////////////////////////////////////////////////
qint64 SndFileSnippet::readSamples(const qint64 offset, const qint64 count, SampleBuffer& buffer)
{
  // Sanity check:
  if (static_cast<SNDFILE*>(m_handle) == 0)
    return 0;

  // Calc required buffer size:
  size_t size = count * buffer.channelCount();
  if (size > m_tempSize)
  {
    // Delete old buffer:
    if (m_tempBuffer != 0)
      delete [] m_tempBuffer;

    // Create new temp buffer:
    m_tempBuffer = new double[size];
    m_tempSize = size;
  }

  // Seek to position:
  sf_seek(static_cast<SNDFILE*>(m_handle), offset, SEEK_SET);

  // Read the samples:
  qint64 readFrames = sf_readf_double(static_cast<SNDFILE*>(m_handle), m_tempBuffer, count);

  // Deinterleave data:
  int channelCount = buffer.channelCount();
  for (int channel = 0; channel < channelCount; channel++)
  {
    double* target = buffer.sampleBuffer(channel);
    double* src = m_tempBuffer + channel;
    for (qint64 sample = 0; sample < readFrames; sample++, src += channelCount)
      target[sample] = *src;
  }

  // Return number of frames read:
  return readFrames;
}

///////////////////////////////// End of File //////////////////////////////////
