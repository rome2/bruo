////////////////////////////////////////////////////////////////////////////////
// (c) 2014 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    imagetoggle.h
///\ingroup bruo
///\brief   Image based toggle button widget definition.
///\author  Rolf Meyerhoff (rm@matrix44.de)
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
#ifndef __IMAGETOGGLE_H_INCLUDED__
#define __IMAGETOGGLE_H_INCLUDED__

#include "imagewidget.h"

////////////////////////////////////////////////////////////////////////////////
///\class ImageToggle imagetoggle.h
///\brief -
////////////////////////////////////////////////////////////////////////////////
class ImageToggle :
  public ImageWidget
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::ImageToggle()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent: Parent window for this widget.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  ImageToggle(QWidget* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::~ImageToggle()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~ImageToggle();

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::value()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Value property.
  ///\return  The current value of this dial.
  ///\remarks The value is either true or false.
  //////////////////////////////////////////////////////////////////////////////
  bool value() const;

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::setValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Value property.
  ///\param   [in] newVal: The new value of this dial.
  ///\remarks The widget will be repainted and a valueChanged event will be
  ///         emitted.
  //////////////////////////////////////////////////////////////////////////////
  void setValue(const bool newVal);

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::leftRight()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the LeftRight property.
  ///\return  The current left to right mode.
  ///\remarks If this is true then we are working left to right instead of top
  ///         to bottom.
  //////////////////////////////////////////////////////////////////////////////
  bool leftRight() const;

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::setLeftRight()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the LinearSize property.
  ///\param   [in] newState: The new left to right state.
  ///\remarks If this is true then we are working left to right instead of top
  ///         to bottom.
  //////////////////////////////////////////////////////////////////////////////
  void setLeftRight(const int newState);

signals:

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::valueChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This signal is emitted when the value of this button changes.
  //////////////////////////////////////////////////////////////////////////////
  void valueChanged();

protected:

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::mousePressEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse button pressed signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mousePressEvent(QMouseEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::mouseReleaseEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse button pressed signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleaseEvent(QMouseEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::drawWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper that paints the actual widget.
  ///\param [in] qp: Device context to use.
  //////////////////////////////////////////////////////////////////////////////
  void drawWidget(QPainter& qp);

private:

  //////////////////////////////////////////////////////////////////////////////
  // ImageToggle::valueFromMousePos()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Internal helper to calc a value for mouse coordinates.
  ///\param  [in] mx: Input position, X component.
  ///\param  [in] my: Input position, Y component.
  ///\return The matching value for the input coordinates.
  //////////////////////////////////////////////////////////////////////////////
  bool valueFromMousePos(const int mx, const int my) const;

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QPoint m_startPos; ///\> Mouse down position.
  bool m_value;      ///\> The current value of this toggle.
  bool m_leftRight;  ///\> Work left to right?
};

#endif // __IMAGETOGGLE_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
