////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    keymap.h
///\ingroup bruo
///\brief   Shortcut map definition.
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
#ifndef __KEYMAP_H_INCLUDED__
#define __KEYMAP_H_INCLUDED__

////////////////////////////////////////////////////////////////////////////////
///\class   Keymap keymap.h
///\brief   A small helper class that maps action IDs to shortcuts.
///\remarks This class is used for loading and saving of key maps and for
///         shortcut presets.
////////////////////////////////////////////////////////////////////////////////
class Keymap
{
public:
  //////////////////////////////////////////////////////////////////////////////
  // Keymap::Keymap()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this class.
  ///\remarks Initializes the class.
  //////////////////////////////////////////////////////////////////////////////
  Keymap();

  //////////////////////////////////////////////////////////////////////////////
  // Keymap::~Keymap()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this class.
  ///\remarks Does final cleanup.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~Keymap();

  //////////////////////////////////////////////////////////////////////////////
  // Keymap::load()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Load a key map from file.
  ///\param   [in] fileName: Name of the file to load.
  ///\remarks The current state of this map is cleared.
  //////////////////////////////////////////////////////////////////////////////
  bool load(const QString& fileName);

  //////////////////////////////////////////////////////////////////////////////
  // Keymap::save()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Save this key map to a file.
  ///\param   [in] fileName: Name of the file to save.
  ///\remarks The map file is a simple xml file.
  //////////////////////////////////////////////////////////////////////////////
  bool save(const QString& fileName);

  //////////////////////////////////////////////////////////////////////////////
  // Keymap::key()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get a key sequence for an action ID.
  ///\param   [in] id: ID of the action for this key sequence.
  ///\return  The key sequence for the action or an empty one if not found.
  ///\remarks If you want to make sure that a key sequence is assigned at all to
  ///         a specific id use the hasKey() function.
  //////////////////////////////////////////////////////////////////////////////
  QKeySequence key(QString id) const;

  //////////////////////////////////////////////////////////////////////////////
  // Keymap::setKey()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set a key sequence for an action ID.
  ///\param   [in] id:  ID of the action for this key sequence.
  ///\param   [in] seq: The key sequence for the action.
  ///\remarks The id must be unique. That means that this function will
  ///         overwrite any existing assignment.
  //////////////////////////////////////////////////////////////////////////////
  void setKey(QString id, QKeySequence seq);

  //////////////////////////////////////////////////////////////////////////////
  // Keymap::hasKey()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Check if an action has assigned a key sequence.
  ///\param   [in] id: ID of the action to check.
  ///\return  True if the action has a key sequence or false otherwise.
  //////////////////////////////////////////////////////////////////////////////
  bool hasKey(const QString& id) const;

  //////////////////////////////////////////////////////////////////////////////
  // Keymap::name()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the friendly name of this map.
  ///\return  The name of this map.
  ///\remarks This name is usually shown in dialog boxes etc instead of the raw
  ///         file name.
  //////////////////////////////////////////////////////////////////////////////
  const QString& name() const;

  //////////////////////////////////////////////////////////////////////////////
  // Keymap::setName()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set the friendly name of this map.
  ///\param   [in] newName: The name of this map.
  ///\remarks This name is usually shown in dialog boxes etc instead of the raw
  ///         file name.
  //////////////////////////////////////////////////////////////////////////////
  void setName(const QString& newName);

private:
  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QString                      m_name; ///> Name of this map.
  QHash<QString, QKeySequence> m_keys; ///> The actual key map.
};

#endif // #ifndef __KEYMAP_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
