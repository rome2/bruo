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

////////////////////////////////////////////////////////////////////////////////
// Foward declarations:
class DocumentManager;

class Document :
  public QObject
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // Document::Document()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this class.
  ///\brief   [in] manager: The parent manager of this document.
  ///\param   [in] parent:  Parent for this instance.
  ///\remarks Basically initializes the document.
  //////////////////////////////////////////////////////////////////////////////
  Document(DocumentManager* manager, QObject* parent = 0) :
    QObject(parent),
    m_dirty(false),
    m_selStart(0),
    m_selLength(0),
    m_cursorPos(0),
    m_undoStack(0),
    m_manager(manager),
    m_sampleRate(0.0),
    m_bitDepth(0),
    m_numChannels(0),
    m_sampleCount(0)
  {
    // Create undo stack:
    m_undoStack = new QUndoStack(this);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Document::~Document()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this document.
  ///\remarks Does final cleanup.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~Document()
  {
    // Nothing to do here.
  }

  QUndoStack* undoStack()
  {
    // Return our stack:
    return m_undoStack;
  }

  const QUndoStack* undoStack() const
  {
    // Return our stack:
    return m_undoStack;
  }

  DocumentManager* manager()
  {
    // Return our manager:
    return m_manager;
  }

  const DocumentManager* manager() const
  {
    // Return our manager:
    return m_manager;
  }

  bool dirty() const
  {
    // Return cuurent state:
    return m_dirty;
  }

  void setDirty(const bool newState = true)
  {
    // Anything to do?
    if (m_dirty == newState)
      return;

    // Update state:
    m_dirty = newState;

    // Notify listeners:
    emitDirtyChanged();
  }

  const QString& fileName() const
  {
    // Return file name:
    return m_fileName;
  }

  QString title() const
  {
    QString text = m_fileName.isEmpty() ? tr("Unamed Document") : QFileInfo(m_fileName).fileName();
    if (m_dirty)
      text.append("*");
    return text;
  }

  void setFileName(const QString& name)
  {
    // Set file name:
    m_fileName = name;
  }

  qint64 selectionStart() const
  {
    // Return current start:
    return m_selStart;
  }

  void setSelectionStart(qint16 start)
  {
    // Set current start:
    m_selStart = start;
  }

  qint64 selectionLength() const
  {
    // Return current length:
    return m_selLength;
  }

  void setSelectionLength(qint16 length)
  {
    // Set current length:
    m_selLength = length;
  }

  void setSelection(qint64 start, qint64 length)
  {
    // Save values:
    m_selStart  = start;
    m_selLength = length;
  }

  qint64 cursorPosition() const
  {
    // Return current position:
    return m_cursorPos;
  }

  void setCursorPosition(qint64 newPos)
  {
    // Update position:
    m_cursorPos = newPos;
  }

  double sampleRate() const
  {
    // Return current sample rate:
    return m_sampleRate;
  }

  int bitDepth() const
  {
    // Return current bit depth:
    return m_bitDepth;
  }

  int channelCount() const
  {
    // Return current channel count:
    return m_numChannels;
  }

  qint64 sampleCount() const
  {
    // Return current sample count:
    return m_sampleCount;
  }

  const PeakData& peakData() const
  {
    // Return our data:
    return m_peakData;
  }

  bool loadFile(QString fileName)
  {
    // Load peak data:
    if (!m_peakData.readReapeaks(fileName))
      return false;

    // Save file name:
    m_fileName = fileName;

    // Save properties:
    m_bitDepth    = 24;
    m_numChannels = m_peakData.channelCount();
    m_sampleRate  = m_peakData.sampleRate();
    m_sampleCount = m_peakData.mipmaps()->divisionFactor() * m_peakData.mipmaps()->sampleCount();

    // Return success:
    return true;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Document::close()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Close this document.
  ///\remarks This will cleanup the used resources.
  //////////////////////////////////////////////////////////////////////////////
  void close()
  {
    // Notify listeners:
    emitClosed();
  }

  void emitSelectionChanging()
  {
    // Notify listeners:
    if (!signalsBlocked())
      emit selectionChanged();
  }

  void emitSelectionChanged()
  {
    // Notify listeners:
    if (!signalsBlocked())
      emit selectionChanged();
  }

  void emitClosed()
  {
    // Notify listeners:
    if (!signalsBlocked())
      emit closed();
  }

  void emitDirtyChanged()
  {
    // Notify listeners:
    if (!signalsBlocked())
      emit dirtyChanged();
  }

signals:

  void selectionChanged();

  void selectionChanging();

  void closed();

  void dirtyChanged();

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  bool             m_dirty;       ///> Was this document modified?
  qint64           m_selStart;    ///> Start of the selection in samples.
  qint64           m_selLength;   ///> Length of the selection in samples.
  qint64           m_cursorPos;   ///> Current cursor position.
  QUndoStack*      m_undoStack;   ///> Undo stack for this document.
  DocumentManager* m_manager;     ///> Parent document manager.
  QString          m_fileName;    ///> File name of this document.
  PeakData         m_peakData;    ///> Current peak data.
  double           m_sampleRate;  ///> Samples per second of a single channel.
  int              m_bitDepth;    ///> Number of bits of a single sample.
  int              m_numChannels; ///> Number of channels of this document.
  qint64           m_sampleCount; ///> Total number of samples of a channel.
};

#endif // #ifndef __DOCUMENT_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
