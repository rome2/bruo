////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    keymap.cpp
///\ingroup bruo
///\brief   Shortcut map implementation.
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
#include "keymap.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

////////////////////////////////////////////////////////////////////////////////
// Keymap::Keymap()
////////////////////////////////////////////////////////////////////////////////
///\brief   Default constructor of this class.
///\remarks Initializes the class.
////////////////////////////////////////////////////////////////////////////////
Keymap::Keymap()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// Keymap::~Keymap()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this class.
///\remarks Does final cleanup.
////////////////////////////////////////////////////////////////////////////////
Keymap::~Keymap()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// Keymap::load()
////////////////////////////////////////////////////////////////////////////////
///\brief   Load a key map from file.
///\param   [in] fileName: Name of the file to load.
///\remarks The current state of this map is cleared.
////////////////////////////////////////////////////////////////////////////////
bool Keymap::load(const QString& fileName)
{
  // Open the file:
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Error while reading the key map: File not found: " << fileName;
    return false;
  }

  QHash<QString, QKeySequence> keys;

  // Parse the file:
  QXmlStreamReader xml(&file);
  while (!xml.atEnd() && !xml.hasError())
  {
    // Read next element:
    QXmlStreamReader::TokenType token = xml.readNext();

    // If token is just StartDocument, we'll go to next:
    if (token == QXmlStreamReader::StartDocument)
      continue;

    // If token is StartElement, we'll see if we can read it:
    if (token == QXmlStreamReader::StartElement)
    {
      // If it's the key list, we'll go to the next:
      if (xml.name() == "keys")
      {
        // Get the attributes:
        QXmlStreamAttributes attributes = xml.attributes();
        if (attributes.hasAttribute("name"))
          m_name = attributes.value("name").toString();
      }

      // If it's a key, we'll dig the information from there:
      else if (xml.name() == "key")
      {
        // Get the attributes:
        QXmlStreamAttributes attributes = xml.attributes();
        if (attributes.hasAttribute("id"))
        {
          // Get the id:
          QString id = attributes.value("id").toString();

          // Get the keys:
          QString key = "";
          if (attributes.hasAttribute("value"))
            key = attributes.value("value").toString();

          // Create map entry:
          keys[id] = QKeySequence(key);
        }
      }
    }
  }

  // Got errors?
  bool error = xml.hasError();
  if (error)
    qDebug() << "Error while reading the key map";

  // No, assimilate data:
  else
    m_keys = keys;

  // Close file:
  xml.clear();
  file.close();

  // Return reader result:
  return !error;
}

////////////////////////////////////////////////////////////////////////////////
// Keymap::save()
////////////////////////////////////////////////////////////////////////////////
///\brief   Save this key map to a file.
///\param   [in] fileName: Name of the file to save.
///\remarks The map file is a simple xml file.
////////////////////////////////////////////////////////////////////////////////
bool Keymap::save(const QString& fileName)
{
  // Open the file:
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    qDebug() << "Error while writing the key map " << fileName;
    return false;
  }

  // Write the file:
  QXmlStreamWriter xml(&file);

  // Start the file:
  xml.setAutoFormatting( true );
  xml.writeStartDocument();
  xml.writeStartElement("keys");
  if (!m_name.isNull())
    xml.writeAttribute("name", m_name);

  // Write the keys:
  for (QHash<QString, QKeySequence>::iterator i = m_keys.begin(); i != m_keys.end(); i++)
  {
    xml.writeEmptyElement("key");
    xml.writeAttribute("id", i.key());
    xml.writeAttribute("value", i->toString());
  }

  // Done:
  xml.writeEndDocument();

  // Got errors?
  bool error = xml.hasError();
  if (error)
    qDebug() << "Error while writing the key map";

  // Close file:
  file.close();

  // Return reader result:
  return !error;
}

////////////////////////////////////////////////////////////////////////////////
// Keymap::key()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get a key sequence for an action ID.
///\param   [in] id: ID of the action for this key sequence.
///\return  The key sequence for the action or an empty one if not found.
///\remarks If you want to make sure that a key sequence is assigned at all to
///         a specific id use the hasKey() function.
////////////////////////////////////////////////////////////////////////////////
QKeySequence Keymap::key(QString id) const
{
  // Check for the key:
  if (m_keys.contains(id))
    return m_keys[id];

  // Not found:
  return QKeySequence();
}

////////////////////////////////////////////////////////////////////////////////
// Keymap::setKey()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set a key sequence for an action ID.
///\param   [in] id:  ID of the action for this key sequence.
///\param   [in] seq: The key sequence for the action.
///\remarks The id must be unique. That means that this function will
///         overwrite any existing assignment.
////////////////////////////////////////////////////////////////////////////////
void Keymap::setKey(QString id, QKeySequence seq)
{
  // Set key:
  m_keys[id] = seq;
}

////////////////////////////////////////////////////////////////////////////////
// Keymap::hasKey()
////////////////////////////////////////////////////////////////////////////////
///\brief   Check if an action has assigned a key sequence.
///\param   [in] id: ID of the action to check.
///\return  True if the action has a key sequence or false otherwise.
////////////////////////////////////////////////////////////////////////////////
bool Keymap::hasKey(const QString& id) const
{
  // Check map:
  return m_keys.contains(id);
}

////////////////////////////////////////////////////////////////////////////////
// Keymap::name()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the friendly name of this map.
///\return  The name of this map.
///\remarks This name is usually shown in dialog boxes etc instead of the raw
///         file name.
////////////////////////////////////////////////////////////////////////////////
const QString& Keymap::name() const
{
  // Return our name:
  return m_name;
}

////////////////////////////////////////////////////////////////////////////////
// Keymap::setName()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set the friendly name of this map.
///\param   [in] newName: The name of this map.
///\remarks This name is usually shown in dialog boxes etc instead of the raw
///         file name.
////////////////////////////////////////////////////////////////////////////////
void Keymap::setName(const QString& newName)
{
  // Set new name:
  m_name = newName;
}

///////////////////////////////// End of File //////////////////////////////////
