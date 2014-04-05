////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    document.cpp
///\ingroup bruo
///\brief   Document class implementation.
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
#include "document.h"
#include "audio/samplebuffer.h"
#include "audio/sndfilesnippet.h"
#include <sndfile.h>

////////////////////////////////////////////////////////////////////////////////
// Document::Document()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this class.
///\param   [in] manager: The parent manager of this document.
///\param   [in] parent:  Parent for this instance.
///\remarks Basically initializes the document.
////////////////////////////////////////////////////////////////////////////////
Document::Document(DocumentManager* manager, QObject* parent) :
  QObject(parent),
  m_dirty(false),
  m_selStart(0),
  m_selLength(0),
  m_selChan(-1),
  m_cursorPos(0),
  m_undoStack(0),
  m_manager(manager),
  m_fileHandle(0),
  m_sampleRate(0.0),
  m_numChannels(0),
  m_sampleCount(0),
  m_format(0),
  m_updatingPeaks(false),
  m_peakThread(this)
{
  // Create undo stack:
  m_undoStack = new QUndoStack(this);
}

////////////////////////////////////////////////////////////////////////////////
// Document::~Document()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this document.
///\remarks Does final cleanup.
////////////////////////////////////////////////////////////////////////////////
Document::~Document()
{
  // Close the source file:
  if (m_fileHandle != 0)
    sf_close(static_cast<SNDFILE*>(m_fileHandle));
  m_fileHandle = 0;

  // Clear play list:
  for (int i = 0; i < m_playList.size(); i++)
    delete m_playList[i];
  m_playList.clear();
}


////////////////////////////////////////////////////////////////////////////////
// Document::undoStack()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the undo manager of this document.
///\return  The undo manager of this document.
///\remarks As the name says the undo manager handles undo and redo.
////////////////////////////////////////////////////////////////////////////////
QUndoStack* Document::undoStack()
{
  // Return our stack:
  return m_undoStack;
}

////////////////////////////////////////////////////////////////////////////////
// Document::undoStack()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the undo manager of this document, const version.
///\return  The undo manager of this document.
///\remarks As the name says the undo manager handles undo and redo.
////////////////////////////////////////////////////////////////////////////////
const QUndoStack* Document::undoStack() const
{
  // Return our stack:
  return m_undoStack;
}

////////////////////////////////////////////////////////////////////////////////
// Document::manager()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the parent document manager of this document.
///\return  The parent of this document.
///\remarks The document manager handles manages all document states of the
///         application.
////////////////////////////////////////////////////////////////////////////////
DocumentManager* Document::manager()
{
  // Return our manager:
  return m_manager;
}

////////////////////////////////////////////////////////////////////////////////
// Document::manager()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the parent document manager of the document, const version.
///\return  The parent of this document.
///\remarks The document manager handles manages all document states of the
///         application.
////////////////////////////////////////////////////////////////////////////////
const DocumentManager* Document::manager() const
{
  // Return our manager:
  return m_manager;
}


////////////////////////////////////////////////////////////////////////////////
// Document::dirty()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the dirty state of this document.
///\return  True if this document has unsaved changes or false otherwise.
///\remarks The dirty state is not set automatically when a property is
///         changed. It must be set explicitely by the calling function.
////////////////////////////////////////////////////////////////////////////////
bool Document::dirty() const
{
  // Return current state:
  return m_dirty;
}

////////////////////////////////////////////////////////////////////////////////
// Document::setDirty()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set a new dirty state for this document.
///\param   [in] newState: New dirty state of this document.
///\remarks The dirtyChanged is not fired automatically when this function is
///         called. It must be emitted explicitely by the calling function.
////////////////////////////////////////////////////////////////////////////////
void Document::setDirty(const bool newState)
{
  // Update state:
  m_dirty = newState;
}

////////////////////////////////////////////////////////////////////////////////
// Document::fileName()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the file name of this document.
///\return  The file name of the document (full path).
///\remarks The file name may be empty if the document has not been saved yet.
////////////////////////////////////////////////////////////////////////////////
const QString& Document::fileName() const
{
  // Return file name:
  return m_fileName;
}

////////////////////////////////////////////////////////////////////////////////
// Document::setFileName()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set the file name of this document.
///\param   [in] name: The new file name of the document (full path).
///\remarks This just sets the file name and does not save anything.
////////////////////////////////////////////////////////////////////////////////
void Document::setFileName(const QString& name)
{
  // Set file name:
  m_fileName = name;
}

////////////////////////////////////////////////////////////////////////////////
// Document::selectionStart()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the current selection start of this document.
///\return  The index of the first selected sample.
///\remarks Samples are only counted for a single channel here so for the
///         selection it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
qint64 Document::selectionStart() const
{
  // Return current start:
  return m_selStart;
}

////////////////////////////////////////////////////////////////////////////////
// Document::setSelectionStart()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set the new selection start of this document.
///\param   [in] start: The index of the first selected sample.
///\remarks Samples are only counted for a single channel here so for the
///         selection it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
void Document::setSelectionStart(qint64 start)
{
  // Clip start value:
  if (start < 0)
    start = 0;
  else if (start >= m_sampleCount)
    start = m_sampleCount - 1;

  // Set current start:
  m_selStart = start;

  // Clip length:
  if ((m_selStart + m_selLength) > m_sampleCount)
    m_selLength = m_sampleCount - m_selStart;
}

////////////////////////////////////////////////////////////////////////////////
// Document::selectionLength()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the current selection length of this document.
///\return  The length of the selection in samples.
///\remarks Samples are only counted for a single channel here so for the
///         selection it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
qint64 Document::selectionLength() const
{
  // Return current length:
  return m_selLength;
}

////////////////////////////////////////////////////////////////////////////////
// Document::setSelectionLength()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set the new selection length of this document.
///\param   [in] length: The length of the selection in samples.
///\remarks Samples are only counted for a single channel here so for the
///         selection it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
void Document::setSelectionLength(qint64 length)
{
  // Clip length:
  if (length < 0)
    length = 0;
  else if ((m_selStart + length) > m_sampleCount)
    length = m_sampleCount - m_selStart;

  // Set current length:
  m_selLength = length;
}

////////////////////////////////////////////////////////////////////////////////
// Document::selectedChannel()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the current selected channel of this document.
///\return  The index of the selected channel.
///\remarks If this value is -1 then the selection covers all channels.
////////////////////////////////////////////////////////////////////////////////
int Document::selectedChannel() const
{
  // Return current channel:
  return m_selChan;
}

////////////////////////////////////////////////////////////////////////////////
// Document::setSelectedChannel()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set the new select channel of this document.
///\param   [in] channel: The index of the selected channel.
///\remarks If this value is -1 then the selection covers all channels.
////////////////////////////////////////////////////////////////////////////////
void Document::setSelectedChannel(int channel)
{
  // Set current channel:
  m_selChan = channel;
}

////////////////////////////////////////////////////////////////////////////////
// Document::setSelection()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set all selection properties of this document at once.
///\param   [in] start:   The index of the first selected sample.
///\param   [in] length:  The length of the selection in samples.
///\param   [in] channel: The index of the selected channel.
///\remarks Samples are only counted for a single channel here so for the
///         selection it doesn't matter how many channels there are. If the
///         channel parameter is -1 then the selection covers all channels.
////////////////////////////////////////////////////////////////////////////////
void Document::setSelection(qint64 start, qint64 length, int channel)
{
  // Clip start value:
  if (start < 0)
    start = 0;
  else if (start >= m_sampleCount)
    start = m_sampleCount - 1;

  // Clip length:
  if (length < 0)
    length = 0;
  else if ((start + length) > m_sampleCount)
    length = m_sampleCount - start;

  // Save values:
  m_selStart  = start;
  m_selLength = length;
  m_selChan   = channel;
}

////////////////////////////////////////////////////////////////////////////////
// Document::cursorPosition()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the current cursor position of this document.
///\return  The index of the sample where the cursor is.
///\remarks Samples are only counted for a single channel here so for the
///         cursor it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
qint64 Document::cursorPosition() const
{
  // Return current position:
  return m_cursorPos;
}

////////////////////////////////////////////////////////////////////////////////
// Document::setCursorPosition()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set the new cursorposition for this document.
///\param   [in] newPos: The index of the sample where the cursor is.
///\remarks Samples are only counted for a single channel here so for the
///         cursor it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
void Document::setCursorPosition(qint64 newPos)
{
  // Clip position:
  if (newPos < 0)
    newPos = 0;
  else if (newPos >= m_sampleCount)
    newPos = m_sampleCount - 1;

  // Update position:
  m_cursorPos = newPos;
}

////////////////////////////////////////////////////////////////////////////////
// Document::sampleRate()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the sample rate of this document.
///\return  The sample rate in samples/second.
///\remarks Samples are only counted for a single channel here so for the
///         sample rate it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
double Document::sampleRate() const
{
  // Return current sample rate:
  return m_sampleRate;
}

////////////////////////////////////////////////////////////////////////////////
// Document::channelCount()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the number of channels of this document.
///\return  The number of channels of this document.
///\remarks 1 is mono, 2 is stereo etc.
////////////////////////////////////////////////////////////////////////////////
int Document::channelCount() const
{
  // Return current channel count:
  return m_numChannels;
}

////////////////////////////////////////////////////////////////////////////////
// Document::sampleCount()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the total sample count of this document.
///\return  The sample count.
///\remarks Samples are only counted for a single channel here so for the
///         count it doesn't matter how many channels there are.
////////////////////////////////////////////////////////////////////////////////
qint64 Document::sampleCount() const
{
  // Return current sample count:
  return m_sampleCount;
}

////////////////////////////////////////////////////////////////////////////////
// Document::peakData()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the peak data of this document.
///\return  The peak data.
///\remarks The peak data is a reduced version of the wave data for display.
////////////////////////////////////////////////////////////////////////////////
const PeakData& Document::peakData() const
{
  // Return our data:
  return m_peakData;
}

////////////////////////////////////////////////////////////////////////////////
// Document::lastError()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the last error as string.
///\return  The last error.
///\remarks If loadFile() fails then this string will hold the reason for the
///         failure.
////////////////////////////////////////////////////////////////////////////////
const QString& Document::lastError() const
{
  // Return the text:
  return m_lastError;
}

////////////////////////////////////////////////////////////////////////////////
// Document::updatingPeaks()
////////////////////////////////////////////////////////////////////////////////
///\brief   Are we currently updating the peaks?
///\return  The current peak building thread status.
////////////////////////////////////////////////////////////////////////////////
bool Document::updatingPeaks() const
{
  // Return current state:
  return m_updatingPeaks;
}

////////////////////////////////////////////////////////////////////////////////
// Document::composeTitle()
////////////////////////////////////////////////////////////////////////////////
///\brief   Create a meaningful title for this document.
///\return  A short tile for this document (including an asterix(*) if dirty).
///\remarks This title can be used for tabs, menues, window captions etc.
////////////////////////////////////////////////////////////////////////////////
QString Document::composeTitle() const
{
  // Build title:
  QString text = m_fileName.isEmpty() ? tr("Unamed Document") : QFileInfo(m_fileName).fileName();
  if (m_dirty)
    text.append("*");

  // Return the text:
  return text;
}

////////////////////////////////////////////////////////////////////////////////
// Document::loadFile()
////////////////////////////////////////////////////////////////////////////////
///\brief   Well, load a file.
///\param   [in] fileName: Name of the file to load.
///\return  true if successful or false otherwise.
///\remarks If the loading fails then you can get the reason with lastError().
////////////////////////////////////////////////////////////////////////////////
bool Document::loadFile(const QString& fileName)
{
  // Close the previous file:
  close();

  // Open the file:
  SF_INFO info;
  QByteArray fn = fileName.toLocal8Bit();
  m_fileHandle = sf_open(fn, SFM_READ, &info);
  if (m_fileHandle == 0)
  {
    // Save failure reason:
    m_lastError = QString(sf_strerror(0));

    // Failure:
    return false;
  }

  // Create playlist entry:
  m_playList.append(new SndFileSnippet(m_fileHandle, info.frames));

  // Save file name:
  m_fileName = fileName;

  // Save properties:
  m_format      = info.format;
  m_numChannels = info.channels;
  m_sampleRate  = info.samplerate;
  m_sampleCount = info.frames;

  // Update peak data:
  m_peakThread.start();

  // Return success:
  return true;
}

//////////////////////////////////////////////////////////////////////////////
// Document::readSamples()
//////////////////////////////////////////////////////////////////////////////
///\brief   Read a group of samples from the current file.
///\param   [in] offset: Starting sample to read.
///\param   [in] buffer: The buffer to fill.
///\return  The actual number of samples read.
///\remarks Samples are only counted for a single channel here so for the
///         count it doesn't matter how many channels there are.
//////////////////////////////////////////////////////////////////////////////
qint64 Document::readSamples(qint64 offset, SampleBuffer& buffer)
{
  // Anything to do?
  if (m_fileHandle == 0 || m_playList.empty())
    return 0;

  // Loop through play list items:
  for (int i = 0; i < m_playList.size(); i++)
  {
    if (offset > m_playList[i]->sampleCount())
    {
      offset -= m_playList[i]->sampleCount();
      continue;
    }

    // Read first buffer:
    return m_playList[i]->readSamples(offset, buffer.sampleCount(), buffer);
//    while (samplesRead > 0 && m_updatingPeaks)
//    {
//      // Add to mipmaps:
//      for (int j = 0; j < m_peakData.mipmapCount() && m_updatingPeaks; j++)
//        m_peakData.mipmaps()[j].addSamples(samplesRead, buffer);

//      // Read next bunch of samples:
//      offset += samplesRead;
//      samplesRead = m_playList[i]->readSamples(offset, bufferSize, buffer);

//      // Need update?
//      updateCounter++;
//      if (updateCounter > 100)
//      {
//        updateCounter = 0;
//        emitPeaksChanged();
//      }
//    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Document::close()
////////////////////////////////////////////////////////////////////////////////
///\brief   Close this document.
///\remarks This will cleanup the used resources.
////////////////////////////////////////////////////////////////////////////////
void Document::close()
{
  // Stop peak thread:
  if (m_updatingPeaks)
  {
    m_updatingPeaks = false;
    m_peakThread.wait();
  }

  // Close the source file:
  if (m_fileHandle != 0)
    sf_close(static_cast<SNDFILE*>(m_fileHandle));
  m_fileHandle = 0;

  // Clear play list:
  for (int i = 0; i < m_playList.size(); i++)
    delete m_playList[i];
  m_playList.clear();

  // Reset properties:
  m_fileName    = "";
  m_format      = 0;
  m_numChannels = 0;
  m_sampleRate  = 0.0;
  m_sampleCount = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Document::emitSelectionChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Helper function to fire the selectionChanged() event.
////////////////////////////////////////////////////////////////////////////////
void Document::emitSelectionChanged()
{
  // Notify listeners:
  if (!signalsBlocked())
    emit selectionChanged();
}

////////////////////////////////////////////////////////////////////////////////
// Document::emitSelectionChanging()
////////////////////////////////////////////////////////////////////////////////
///\brief Helper function to fire the selectionChanging() event.
////////////////////////////////////////////////////////////////////////////////
void Document::emitSelectionChanging()
{
  // Notify listeners:
  if (!signalsBlocked())
    emit selectionChanged();
}

////////////////////////////////////////////////////////////////////////////////
// Document::emitClosed()
////////////////////////////////////////////////////////////////////////////////
///\brief Helper function to fire the closed() event.
////////////////////////////////////////////////////////////////////////////////
void Document::emitClosed()
{
  // Notify listeners:
  if (!signalsBlocked())
    emit closed();
}

////////////////////////////////////////////////////////////////////////////////
// Document::emitDirtyChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Helper function to fire the dirtyChanged() event.
////////////////////////////////////////////////////////////////////////////////
void Document::emitDirtyChanged()
{
  // Notify listeners:
  if (!signalsBlocked())
    emit dirtyChanged();
}

////////////////////////////////////////////////////////////////////////////////
// Document::emitPeaksChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Helper function to fire the peaksChanged() event.
////////////////////////////////////////////////////////////////////////////////
void Document::emitPeaksChanged()
{
  // Notify listeners:
  if (!signalsBlocked())
    emit peaksChanged();
}

////////////////////////////////////////////////////////////////////////////////
// Document::updatePeakData()
////////////////////////////////////////////////////////////////////////////////
///\brief   Update the peak data of this document.
////////////////////////////////////////////////////////////////////////////////
void Document::updatePeakData()
{
  // Already in loop?
  if (m_updatingPeaks)
    return;

  // Flag update:
  m_updatingPeaks = true;

  // Calc number of mip maps:
  int numMips = 3;
  if ((m_sampleCount / m_sampleRate) > 1000)
    numMips++;

  // Create the mip maps:
  m_peakData.allocateMipMaps(numMips, m_numChannels, m_sampleRate, m_sampleCount);

  // Create sample buffer:
  const int bufferSize = 4096;
  SampleBuffer buffer(m_numChannels, bufferSize);

  // Loop through play list items:
  for (int i = 0; i < m_playList.size() && m_updatingPeaks; i++)
  {
    // Read first buffer:
    int updateCounter = 0;
    qint64 offset = 0;
    int samplesRead = m_playList[i]->readSamples(offset, bufferSize, buffer);
    while (samplesRead > 0 && m_updatingPeaks)
    {
      // Add to mipmaps:
      for (int j = 0; j < m_peakData.mipmapCount() && m_updatingPeaks; j++)
        m_peakData.mipmaps()[j].addSamples(samplesRead, buffer);

      // Read next bunch of samples:
      offset += samplesRead;
      samplesRead = m_playList[i]->readSamples(offset, bufferSize, buffer);

      // Need update?
      updateCounter++;
      if (updateCounter > 100)
      {
        updateCounter = 0;
        emitPeaksChanged();
      }
    }
  }

  // Flag update:
  m_updatingPeaks = false;

  // Final update:
  emitPeaksChanged();
}

///////////////////////////////// End of File //////////////////////////////////
