////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    document.h
///\ingroup bruo
///\brief   Document class definition.
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
#ifndef __DOCUMENT_H_INCLUDED__
#define __DOCUMENT_H_INCLUDED__

#include "bruo.h"
#include "peakdata.h"
#include "audio/audiosnippet.h"

////////////////////////////////////////////////////////////////////////////////
// Foward declarations:
class DocumentManager;

////////////////////////////////////////////////////////////////////////////////
///\class   Document document.h
///\brief   The application's document class.
///\remarks This class encapsulates the data of a document and it provides
///         access methods as well as document specific signals and slots. But
///         this class will never emit a signal by itself to avoid redundant
///         and superflous messages. So please call one of the emit*() functions
///         whenever needed.
////////////////////////////////////////////////////////////////////////////////
class Document :
  public QObject
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // Document::Document()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this class.
  ///\param   [in] manager: The parent manager of this document.
  ///\param   [in] parent:  Parent for this instance.
  ///\remarks Basically initializes the document.
  //////////////////////////////////////////////////////////////////////////////
  Document(DocumentManager* manager, QObject* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // Document::~Document()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this document.
  ///\remarks Does final cleanup.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~Document();

  //////////////////////////////////////////////////////////////////////////////
  // Document::undoStack()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the undo manager of this document.
  ///\return  The undo manager of this document.
  ///\remarks As the name says the undo manager handles undo and redo.
  //////////////////////////////////////////////////////////////////////////////
  QUndoStack* undoStack();

  //////////////////////////////////////////////////////////////////////////////
  // Document::undoStack()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the undo manager of this document, const version.
  ///\return  The undo manager of this document.
  ///\remarks As the name says the undo manager handles undo and redo.
  //////////////////////////////////////////////////////////////////////////////
  const QUndoStack* undoStack() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::manager()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the parent document manager of this document.
  ///\return  The parent of this document.
  ///\remarks The document manager handles manages all document states of the
  ///         application.
  //////////////////////////////////////////////////////////////////////////////
  DocumentManager* manager();

  //////////////////////////////////////////////////////////////////////////////
  // Document::manager()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the parent document manager of the document, const version.
  ///\return  The parent of this document.
  ///\remarks The document manager handles manages all document states of the
  ///         application.
  //////////////////////////////////////////////////////////////////////////////
  const DocumentManager* manager() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::dirty()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the dirty state of this document.
  ///\return  True if this document has unsaved changes or false otherwise.
  ///\remarks The dirty state is not set automatically when a property is
  ///         changed. It must be set explicitely by the calling function.
  //////////////////////////////////////////////////////////////////////////////
  bool dirty() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::setDirty()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set a new dirty state for this document.
  ///\param   [in] newState: New dirty state of this document.
  ///\remarks The dirtyChanged is not fired automatically when this function is
  ///         called. It must be emitted explicitely by the calling function.
  //////////////////////////////////////////////////////////////////////////////
  void setDirty(const bool newState = true);

  //////////////////////////////////////////////////////////////////////////////
  // Document::fileName()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the file name of this document.
  ///\return  The file name of the document (full path).
  ///\remarks The file name may be empty if the document has not been saved yet.
  //////////////////////////////////////////////////////////////////////////////
  const QString& fileName() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::setFileName()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set the file name of this document.
  ///\param   [in] name: The new file name of the document (full path).
  ///\remarks This just sets the file name and does not save anything.
  //////////////////////////////////////////////////////////////////////////////
  void setFileName(const QString& name);

  //////////////////////////////////////////////////////////////////////////////
  // Document::selectionStart()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the current selection start of this document.
  ///\return  The index of the first selected sample.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         selection it doesn't matter how many channels there are.
  //////////////////////////////////////////////////////////////////////////////
  qint64 selectionStart() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::setSelectionStart()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set the new selection start of this document.
  ///\param   [in] start: The index of the first selected sample.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         selection it doesn't matter how many channels there are.
  //////////////////////////////////////////////////////////////////////////////
  void setSelectionStart(qint16 start);

  //////////////////////////////////////////////////////////////////////////////
  // Document::selectionLength()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the current selection length of this document.
  ///\return  The length of the selection in samples.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         selection it doesn't matter how many channels there are.
  //////////////////////////////////////////////////////////////////////////////
  qint64 selectionLength() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::setSelectionLength()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set the new selection length of this document.
  ///\param   [in] length: The length of the selection in samples.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         selection it doesn't matter how many channels there are.
  //////////////////////////////////////////////////////////////////////////////
  void setSelectionLength(qint16 length);

  //////////////////////////////////////////////////////////////////////////////
  // Document::selectedChannel()
  ////////////////////////////////////////////////////SndFileAudioSnippet//////////////////////////
  ///\brief   Access the current selected channel of this document.
  ///\return  The index of the selected channel.
  ///\remarks If this value is -1 then the selection covers all channels.
  //////////////////////////////////////////////////////////////////////////////
  int selectedChannel() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::setSelectedChannel()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set the new select channel of this document.
  ///\param   [in] channel: The index of the selected channel.
  ///\remarks If this value is -1 then the selection covers all channels.
  //////////////////////////////////////////////////////////////////////////////
  void setSelectedChannel(int channel);

  //////////////////////////////////////////////////////////////////////////////
  // Document::setSelection()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set all selection properties of this document at once.
  ///\param   [in] start:   The index of the first selected sample.
  ///\param   [in] length:  The length of the selection in samples.
  ///\param   [in] channel: The index of the selected channel.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         selection it doesn't matter how many channels there are. If the
  ///         channel parameter is -1 then the selection covers all channels.
  //////////////////////////////////////////////////////////////////////////////
  void setSelection(qint64 start, qint64 length, int channel = -1);

  //////////////////////////////////////////////////////////////////////////////
  // Document::cursorPosition()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the current cursor position of this document.
  ///\return  The index of the sample where the cursor is.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         cursor it doesn't matter how many channels there are.
  //////////////////////////////////////////////////////////////////////////////
  qint64 cursorPosition() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::setCursorPosition()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set the new cursorposition for this document.
  ///\param   [in] newPos: The index of the sample where the cursor is.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         cursor it doesn't matter how many channels there are.
  //////////////////////////////////////////////////////////////////////////////
  void setCursorPosition(qint64 newPos);

  //////////////////////////////////////////////////////////////////////////////
  // Document::sampleRate()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the sample rate of this document.
  ///\return  The sample rate in samples/second.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         sample rate it doesn't matter how many channels there are.
  //////////////////////////////////////////////////////////////////////////////
  double sampleRate() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::channelCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the number of channels of this document.
  ///\return  The number of channels of this document.
  ///\remarks 1 is mono, 2 is stereo etc.
  //////////////////////////////////////////////////////////////////////////////
  int channelCount() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::sampleCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the total sample count of this document.
  ///\return  The sample count.
  ///\remarks Samples are only counted for a single channel here so for the
  ///         count it doesn't matter how many channels there are.
  //////////////////////////////////////////////////////////////////////////////
  qint64 sampleCount() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::peakData()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the peak data of this document.
  ///\return  The peak data.
  ///\remarks The peak data is a reduced version of the wave data for display.
  //////////////////////////////////////////////////////////////////////////////
  const PeakData& peakData() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::lastError()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the last error as string.
  ///\return  The last error.
  ///\remarks If loadFile() fails then this string will hold the reason for the
  ///         failure.
  //////////////////////////////////////////////////////////////////////////////
  const QString& lastError() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::composeTitle()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Create a meaningful title for this document.
  ///\return  A short tile for this document (including an asterix(*) if dirty).
  ///\remarks This title can be used for tabs, menues, window captions etc.
  //////////////////////////////////////////////////////////////////////////////
  QString composeTitle() const;

  //////////////////////////////////////////////////////////////////////////////
  // Document::loadFile()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Well, load a file.
  ///\oaram   [in] fileName: Name of the file to load.
  ///\return  true if successful or false otherwise.
  ///\remarks If the loading fails then you can get the reason with lastError().
  //////////////////////////////////////////////////////////////////////////////
  bool loadFile(const QString& fileName);

  //////////////////////////////////////////////////////////////////////////////
  // Document::close()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Close this document.
  ///\remarks This will cleanup the used resources.
  //////////////////////////////////////////////////////////////////////////////
  void close();

  //////////////////////////////////////////////////////////////////////////////
  // Document::emitSelectionChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Helper function to fire the selectionChanged() event.
  //////////////////////////////////////////////////////////////////////////////
  void emitSelectionChanged();

  //////////////////////////////////////////////////////////////////////////////
  // Document::emitSelectionChanging()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Helper function to fire the selectionChanging() event.
  //////////////////////////////////////////////////////////////////////////////
  void emitSelectionChanging();

  //////////////////////////////////////////////////////////////////////////////
  // Document::emitClosed()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Helper function to fire the closed() event.
  //////////////////////////////////////////////////////////////////////////////
  void emitClosed();

  //////////////////////////////////////////////////////////////////////////////
  // Document::emitDirtyChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Helper function to fire the dirtyChanged() event.
  //////////////////////////////////////////////////////////////////////////////
  void emitDirtyChanged();

signals:

  //////////////////////////////////////////////////////////////////////////////
  // Document::selectionChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This event is fired when the selection has been changed.
  ///\remarks This class will never emit a signal by itself to avoid redundant
  ///         and superflous messages. So please call the matching emit*()
  ///         function above whenever needed.
  //////////////////////////////////////////////////////////////////////////////
  void selectionChanged();

  //////////////////////////////////////////////////////////////////////////////
  // Document::selectionChanging()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This event is fired while the selection is changing.
  ///\remarks This class will never emit a signal by itself to avoid redundant
  ///         and superflous messages. So please call the matching emit*()
  ///         function above whenever needed.
  //////////////////////////////////////////////////////////////////////////////
  void selectionChanging();

  //////////////////////////////////////////////////////////////////////////////
  // Document::closed()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This event is fired when the document was closed.
  ///\remarks This class will never emit a signal by itself to avoid redundant
  ///         and superflous messages. So please call the matching emit*()
  ///         function above whenever needed.
  //////////////////////////////////////////////////////////////////////////////
  void closed();

  //////////////////////////////////////////////////////////////////////////////
  // Document::dirtyChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This event is fired when the dirty state has been changed.
  ///\remarks This class will never emit a signal by itself to avoid redundant
  ///         and superflous messages. So please call the matching emit*()
  ///         function above whenever needed.
  //////////////////////////////////////////////////////////////////////////////
  void dirtyChanged();

private:

  //////////////////////////////////////////////////////////////////////////////
  // Document::updatePeakData()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Update the peak data of this document.
  //////////////////////////////////////////////////////////////////////////////
  void updatePeakData();

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  bool                 m_dirty;       ///> Was this document modified?
  qint64               m_selStart;    ///> Start of the selection in samples.
  qint64               m_selLength;   ///> Length of the selection in samples.
  int                  m_selChan;     ///> The selected channel.
  qint64               m_cursorPos;   ///> Current cursor position.
  QUndoStack*          m_undoStack;   ///> Undo stack for this document.
  DocumentManager*     m_manager;     ///> Parent document manager.
  QString              m_fileName;    ///> File name of this document.
  void*                m_fileHandle;  ///> The handle for the file.
  QString              m_lastError;   ///> The last error as string.
  PeakData             m_peakData;    ///> Current peak data.
  double               m_sampleRate;  ///> Samples per second of a channel.
  int                  m_numChannels; ///> Number of channels of this document.
  qint64               m_sampleCount; ///> Total number of samples of a channel.
  int                  m_format;      ///> Id of the file format.
  QList<AudioSnippet*> m_playList;    ///> The sample buffer playback list.
};

#endif // #ifndef __DOCUMENT_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
