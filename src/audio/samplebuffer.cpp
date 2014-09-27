////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    samplebuffer.cpp
///\ingroup bruo
///\brief   Sample buffer implementation.
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
#include "samplebuffer.h"

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::SampleBuffer()
////////////////////////////////////////////////////////////////////////////////
///\brief   Default constructor of this class.
///\remarks Basically only initializes the buffer.
////////////////////////////////////////////////////////////////////////////////
SampleBuffer::SampleBuffer() :
  m_channelCount(0),
  m_sampleCount(0),
  m_sampleBuffer(0)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::SampleBuffer()
////////////////////////////////////////////////////////////////////////////////
///\brief   Copy constructor of this class.
///\brief   [in] other: The buffer to copy.
///\remarks Copies the entire buffer.
////////////////////////////////////////////////////////////////////////////////
SampleBuffer::SampleBuffer(const SampleBuffer& other) :
  m_channelCount(0),
  m_sampleCount(0),
  m_sampleBuffer(0)
{
  // Create a matching buffer:
  createBuffers(other.m_channelCount, other.m_sampleCount);

  // Copy data:
  memcpy(m_sampleBuffer, other.m_sampleBuffer, m_channelCount * m_sampleCount * sizeof(double));
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::SampleBuffer()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this class.
///\brief   [in] numChannels: Number of channels of this buffer.
///\param   [in] numSamples:  Number of samples for a single channel.
///\remarks The buffer is zeroed.
////////////////////////////////////////////////////////////////////////////////
SampleBuffer::SampleBuffer(int numChannels, int numSamples) :
  m_channelCount(0),
  m_sampleCount(0),
  m_sampleBuffer(0)
{
  // Just create a matching buffer:
  createBuffers(numChannels, numSamples);
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::~SampleBuffer()
////////////////////////////////////////////////////////////////////////////////
///\brief   Default destructor of this class.
///\remarks Frees all used resources. This is not virtual because this class
///         is not ment to be subclassed.
////////////////////////////////////////////////////////////////////////////////
SampleBuffer::~SampleBuffer()
{
  // Cleanup:
  if (m_sampleBuffer != 0)
    delete [] m_sampleBuffer;
  m_sampleBuffer = 0;
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::channelCount()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the number of channels of this buffer.
///\return  The number of channels of this buffer.
///\remarks 1 is mono, 2 is stereo etc.
////////////////////////////////////////////////////////////////////////////////
int SampleBuffer::channelCount() const
{
  // Return number of channels:
  return m_channelCount;
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::sampleCount()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the sample count of this buffer.
///\return  The sample count.
///\remarks Samples are only counted for a single channel here so for the
///         count it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
int SampleBuffer::sampleCount() const
{
  // Return number of samples:
  return m_sampleCount;
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::sampleBuffer()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the samples of this buffer.
///\param   [in] channel: The channel to access.
///\return  A pointer to the first sample of the requested channel.
///\remarks The data is not interleaved so each channel has it's own buffer.
////////////////////////////////////////////////////////////////////////////////
double* SampleBuffer::sampleBuffer(const int channel)
{
  // Sanity check:
  if (m_sampleBuffer == 0)
    return 0;

  // Return the buffer:
  return m_sampleBuffer + (channel * m_sampleCount);
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::sampleBuffer()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the samples of this buffer, const version.
///\param   [in] channel: The channel to access.
///\return  A pointer to the first sample of the requested channel.
///\remarks The data is not interleaved so each channel has it's own buffer.
////////////////////////////////////////////////////////////////////////////////
const double* SampleBuffer::sampleBuffer(const int channel) const
{
  // Sanity check:
  if (m_sampleBuffer == 0)
    return 0;

  // Return the buffer:
  return m_sampleBuffer + (channel * m_sampleCount);
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::sample()
////////////////////////////////////////////////////////////////////////////////
///\brief  Extract a single samples from this buffer.
///\param  [in] channel: The channel of the sample.
///\param  [in] sample:  The index of the sample.
///\return The requested sample.
////////////////////////////////////////////////////////////////////////////////
double SampleBuffer::sample(const int channel, const int sample) const
{
  // Sanity check:
  assert(m_sampleBuffer != 0);
  assert(channel >= 0 && channel < m_channelCount);
  assert(sample >= 0 && sample < m_sampleCount);

  // Return sample:
  return *(m_sampleBuffer + (channel * m_sampleCount) + sample);
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::setSample()
////////////////////////////////////////////////////////////////////////////////
///\brief  Set a single sample from this buffer.
///\param  [in] channel: The channel of the sample.
///\param  [in] sample:  The index of the sample.
///\param  [in] value:   The new sample.
////////////////////////////////////////////////////////////////////////////////
void SampleBuffer::setSample(const int channel, const int sample, const double value)
{
  // Sanity check:
  assert(m_sampleBuffer != 0);
  assert(channel >= 0 && channel < m_channelCount);
  assert(sample >= 0 && sample < m_sampleCount);

  // Set sample:
  *(m_sampleBuffer + (channel * m_sampleCount) + sample) = value;
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::makeSilence()
////////////////////////////////////////////////////////////////////////////////
///\brief Fill this buffer with zeroes.
////////////////////////////////////////////////////////////////////////////////
void SampleBuffer::makeSilence()
{
  // Sanity check:
  if (m_sampleBuffer == 0)
    return;

  // Clear the buffer:
  memset(m_sampleBuffer, 0, m_channelCount * m_sampleCount * sizeof(double));
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::operator = ()
////////////////////////////////////////////////////////////////////////////////
///\brief   Assignment operator of this class.
///\brief   [in] other: The buffer to copy.
///\return  A reference to this object.
///\remarks Copies the entire buffer without allocations if possible.
////////////////////////////////////////////////////////////////////////////////
SampleBuffer& SampleBuffer::operator = (const SampleBuffer& other)
{
  // Do we have to create a new buffer?
  if (other.m_sampleCount != m_sampleCount || other.m_channelCount != m_channelCount)
    createBuffers(other.m_channelCount, other.m_sampleCount);

  // Copy data:
  memcpy(m_sampleBuffer, other.m_sampleBuffer, m_channelCount * m_sampleCount * sizeof(double));

  // Always return this:
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
// SampleBuffer::createBuffers()
////////////////////////////////////////////////////////////////////////////////
///\brief   Create the actual storage space for this buffer.
///\brief   [in] numChannels: Number of channels of the buffer.
///\param   [in] numSamples:  Number of samples for a single channel.
///\remarks The old buffer (if any) will be deleted and the new buffer is
///         zeroed.
////////////////////////////////////////////////////////////////////////////////
void SampleBuffer::createBuffers(int numChannels, int numSamples)
{
  // Sanity check:
  assert(numChannels >= 0);
  assert(numSamples >= 0);

  // Free old buffers:
  if (m_sampleBuffer != 0)
    delete [] m_sampleBuffer;
  m_sampleBuffer = 0;

  // Set properties:
  m_channelCount = numChannels;
  m_sampleCount  = numSamples;

  // Alloc space for the samples:
  if (m_channelCount != 0 && m_sampleCount != 0)
    m_sampleBuffer = new double[m_channelCount * m_sampleCount];

  // Clear the buffer:
  makeSilence();
}

///////////////////////////////// End of File //////////////////////////////////
