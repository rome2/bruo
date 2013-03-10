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

#include <QtGui>
#include <QtWidgets>
#include "bruo.h"
#include "peakdata.h"


class Document :
  public QObject
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // Document::Document()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this clas.
  ///\param   [in] parent: Parent for this instance.
  ///\remarks Basically initializes the document.
  //////////////////////////////////////////////////////////////////////////////
  Document(QObject* parent = 0) :
    QObject(parent),
    m_dirty(false),
    m_selectionStart(0),
    m_selectionLength(0),
    m_undoStack(0)
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
  ~Document()
  {
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

  bool dirty() const
  {
    // Return cuurent state:
    return m_dirty;
  }

  void setDirty(const bool newState = true)
  {
    // Update state:
    m_dirty = newState;
  }

  const QString& fileName() const
  {
    // Return file name:
    return m_fileName;
  }

  QString title() const
  {
    return m_fileName.isEmpty() ? tr("Unamed Document") : QFileInfo(m_fileName).fileName();
  }

  void setFileName(const QString& name)
  {
    // Set file name:
    m_fileName = name;
  }

  qint64 selectionStart() const
  {
    // Return current start:
    return m_selectionStart;
  }

  void setSelectionStart(qint16 start)
  {
    // Set current start:
    m_selectionStart = start;
  }

  qint64 selectionLength() const
  {
    // Return current length:
    return m_selectionLength;
  }

  void setSelectionLength(qint16 length)
  {
    // Set current length:
    m_selectionLength = length;
  }

  void setSelection(qint64 start, qint64 length)
  {
    // Save values:
    m_selectionStart  = start;
    m_selectionLength = length;
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

    // Return success:
    return true;
  }

  void close()
  {
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

signals:

  void selectionChanged();

  void selectionChanging();

  void closed();

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  bool        m_dirty;           ///> Was this document modified?
  qint64      m_selectionStart;
  qint64      m_selectionLength;
  QUndoStack* m_undoStack;
  QString     m_fileName;
  PeakData    m_peakData;
};

#endif // #ifndef __DOCUMENT_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
