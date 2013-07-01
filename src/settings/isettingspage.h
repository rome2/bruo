////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    isettingspage.h
///\ingroup bruo
///\brief   Settings dialog page interface declaration.
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
#ifndef __ISETTINGSPAGE_H_INCLUDED__
#define __ISETTINGSPAGE_H_INCLUDED__

////////////////////////////////////////////////////////////////////////////////
///\class   ISettingsPage isettingspage.h
///\brief   Base interface for all settings dialog pages.
///\remarks This interface must be implemented by all the setting's child
///         widgets. It contains properties for the page icon and title as well
///         as common functions.
////////////////////////////////////////////////////////////////////////////////
class ISettingsPage
{
public:

  //////////////////////////////////////////////////////////////////////////////
  // Document::categoryTitle()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the title of this settings page.
  ///\return  A string describing this page.
  ///\remarks This title is displayed in the category list and the title of the
  ///         application's settings dialog.
  //////////////////////////////////////////////////////////////////////////////
  virtual QString categoryTitle() = 0;

  //////////////////////////////////////////////////////////////////////////////
  // Document::categoryIcon()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Access the icon of this settings page.
  ///\return  An icon for this page.
  ///\remarks This icon is displayed in the category list of the application's
  ///         settings dialog.
  //////////////////////////////////////////////////////////////////////////////
  virtual QIcon categoryIcon() = 0;

  //////////////////////////////////////////////////////////////////////////////
  // Document::apply()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Apply the settings of this dialog page.
  ///\remarks This function is called when the dialog's apply or close button
  ///         was clicked.
  //////////////////////////////////////////////////////////////////////////////
  virtual void apply() = 0;

  //////////////////////////////////////////////////////////////////////////////
  // Document::finish()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Cleanup for the this dialog page.
  ///\remarks This function is called when the dialog is closed.
  //////////////////////////////////////////////////////////////////////////////
  virtual void finish() = 0;
};

#endif // #ifndef __ISETTINGSPAGE_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
