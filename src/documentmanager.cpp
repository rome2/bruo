////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    documentmanager.cpp
///\ingroup bruo
///\brief   Document manager class implementation.
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
#include "documentmanager.h"

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::DocumentManager()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this class.
///\param   [in] parent: Parent object for this class.
///\remarks Initializes this class.
////////////////////////////////////////////////////////////////////////////////
DocumentManager::DocumentManager(QObject* parent) :
  QObject(parent)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::~DocumentManager()
////////////////////////////////////////////////////////////////////////////////
///\brief Destructor of this class.
////////////////////////////////////////////////////////////////////////////////
DocumentManager::~DocumentManager()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::documents()
////////////////////////////////////////////////////////////////////////////////
///\brief   Accessor for the contained documents.
///\return  The list of open documents.
///\remarks This list may be empty of no document was loaded. If it's not
///         empty then the first document in the list is the active document.
////////////////////////////////////////////////////////////////////////////////
const QList<Document*>& DocumentManager::documents() const
{
  // Return our list:
  return m_documents;
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::newDocument()
////////////////////////////////////////////////////////////////////////////////
///\brief   Create a new empty document.
///\return  The newly created document.
///\remarks The new document is just appended to the document list so it's not
///         automatically activated.
////////////////////////////////////////////////////////////////////////////////
Document* DocumentManager::newDocument()
{
  // Create a new empty document:
  Document* doc = new Document(this);

  // Add to the list:
  m_documents.append(doc);

  // Give listeners time to attach their event handlers etc:
  emitDocumentCreated(doc);

  // Done:
  return doc;
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::closeDocument()
////////////////////////////////////////////////////////////////////////////////
///\brief   Close a specific document.
///\param   [in] doc: The document to close.
///\remarks The document will be deleted and removed from the document list.
///         If the document was the active one then the next in the list will be
///         activated.
////////////////////////////////////////////////////////////////////////////////
void DocumentManager::closeDocument(Document* doc)
{
  // Valid document?
  if (doc == 0)
    return;

  // Get current index:
  int index = m_documents.indexOf(doc);

  // Still in the list?
  if (index < 0)
    return;

  // Remove from list:
  m_documents.removeAt(index);

  // Close and delete it:
  doc->close();
  delete doc;

  // If it was at the tip then the active document has changed:
  if (index == 0)
    emitActiveDocumentChanged();
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::closeAllDocuments()
////////////////////////////////////////////////////////////////////////////////
///\brief   Close all documents.
///\remarks The documents will all be deleted and removed from the document
///         list.
////////////////////////////////////////////////////////////////////////////////
void DocumentManager::closeAllDocuments()
{
  // Anything to do?
  if (m_documents.empty())
    return;

  // Close and delete all documents:
  for (int i = 0; i < m_documents.count(); i++)
  {
    m_documents[i]->close();
    delete m_documents[i];
  }

  // Clear list:
  m_documents.clear();

  // Active document has changed to null:
  emitActiveDocumentChanged();
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::activeDocument()
////////////////////////////////////////////////////////////////////////////////
///\brief   Accessor for the currently active document.
///\return  The currently active document.
///\remarks The return value may be null if the document list is empty.
////////////////////////////////////////////////////////////////////////////////
Document* DocumentManager::activeDocument()
{
  // Got something to return?
  if (m_documents.empty())
    return 0;

  // Return first document:
  return m_documents[0];
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::activeDocument()
////////////////////////////////////////////////////////////////////////////////
///\brief   Accessor for the currently active document, const version.
///\return  The currently active document.
///\remarks The return value may be null if the document list is empty.
////////////////////////////////////////////////////////////////////////////////
const Document* DocumentManager::activeDocument() const
{
  // Got something to return?
  if (m_documents.empty())
    return 0;

  // Return first document:
  return m_documents[0];
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::setActiveDocument()
////////////////////////////////////////////////////////////////////////////////
///\brief Sets the new currently active document.
///\param [in] doc: The new active document.
////////////////////////////////////////////////////////////////////////////////
void DocumentManager::setActiveDocument(Document* doc)
{
  // Valid document?
  if (doc == 0)
    return;

  // Activate by index:
  setActiveDocument(m_documents.indexOf(doc));
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::setActiveDocument()
////////////////////////////////////////////////////////////////////////////////
///\brief Sets the new currently active document.
///\param [in] index: The index of the new active document.
////////////////////////////////////////////////////////////////////////////////
void DocumentManager::setActiveDocument(int index)
{
  // Valid document or already at the tip?
  if (index <= 0 || index >= m_documents.count())
    return;

  // Get document:
  Document* doc = m_documents.at(index);

  // Remove from list:
  m_documents.removeAt(index);

  // Insert at the front:
  m_documents.insert(0, doc);

  // Notify listeners:
  emitActiveDocumentChanged();
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::emitDocumentCreated()
////////////////////////////////////////////////////////////////////////////////
///\brief Fire a documentCreated event.
///\param The newly created document.
////////////////////////////////////////////////////////////////////////////////
void DocumentManager::emitDocumentCreated(Document* doc)
{
  // Emit the signal if not blocked:
  if (!signalsBlocked())
    emit documentCreated(doc);
}

////////////////////////////////////////////////////////////////////////////////
// DocumentManager::emitActiveDocumentChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Fire a activeDocumentChanged event.
////////////////////////////////////////////////////////////////////////////////
void DocumentManager::emitActiveDocumentChanged()
{
  // Emit the signal if not blocked:
  if (!signalsBlocked())
    emit activeDocumentChanged();
}

///////////////////////////////// End of File //////////////////////////////////
