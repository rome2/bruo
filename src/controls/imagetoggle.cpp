////////////////////////////////////////////////////////////////////////////////
// (c) 2014 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    imagetoggle.cpp
///\ingroup bruo
///\brief   Image based toggle button widget implementation.
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
#include "imagetoggle.h"

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::ImageToggle()
////////////////////////////////////////////////////////////////////////////////
///\brief   Default constructor of this widget.
///\param   [in] parent: Parent window for this widget.
///\remarks Just initializes the members.
////////////////////////////////////////////////////////////////////////////////
ImageToggle::ImageToggle(QWidget* parent) :
  ImageWidget(parent),
  m_value(false),
  m_leftRight(false)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::~ImageToggle()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Cleans up used resources.
////////////////////////////////////////////////////////////////////////////////
ImageToggle::~ImageToggle()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::value()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the Value property.
///\return  The current value of this dial.
///\remarks The value is either true or false.
////////////////////////////////////////////////////////////////////////////////
bool ImageToggle::value() const
{
  // Return current value:
  return m_value;
}

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::setValue()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the Value property.
///\param   [in] newVal: The new value of this dial.
///\remarks The widget will be repainted and a valueChanged event will be
///         emitted.
////////////////////////////////////////////////////////////////////////////////
void ImageToggle::setValue(const bool newVal)
{
  // Anything to do?
  if (m_value == newVal)
    return;

  // Set new value:
  m_value = newVal;

  // Force redraw:
  update();

  // Notify listeners:
  if (!signalsBlocked())
    emit valueChanged();
}

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::leftRight()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the LeftRight property.
///\return  The current left to right mode.
///\remarks If this is true then we are working left to right instead of top
///         to bottom.
////////////////////////////////////////////////////////////////////////////////
bool ImageToggle::leftRight() const
{
  // Return current state:
  return m_leftRight;
}

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::setLeftRight()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the LinearSize property.
///\param   [in] newState: The new left to right state.
///\remarks If this is true then we are working left to right instead of top
///         to bottom.
////////////////////////////////////////////////////////////////////////////////
void ImageToggle::setLeftRight(const int newState)
{
  // Set new state:
  m_leftRight = newState;
}

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::mousePressEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the mouse button pressed signal.
///\param   [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageToggle::mousePressEvent(QMouseEvent* event)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Left click?
  if (event->buttons() == Qt::LeftButton)
  {
    // Save start position:
    m_startPos.setX(event->x());
    m_startPos.setY(event->y());
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::mouseReleaseEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the mouse button pressed signal.
///\param   [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageToggle::mouseReleaseEvent(QMouseEvent* event)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Left click?
  if (!(event->buttons() & Qt::LeftButton))
  {
    // Check bounds:
    if (event->x() < 0 || event->x() >= width())
      return;
    if (event->y() < 0 || event->y() >= height())
      return;

    // Get current value:
    bool newVal = valueFromMousePos(event->x(), event->y());

    // Compare to start position:
    if (newVal != valueFromMousePos(m_startPos.x(), m_startPos.y()))
      return;

    // Update widget:
    setValue(newVal);
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::drawWidget()
////////////////////////////////////////////////////////////////////////////////
///\brief Internal helper that paints the actual widget.
///\param [in] qp: Device context to use.
////////////////////////////////////////////////////////////////////////////////
void ImageToggle::drawWidget(QPainter& qp)
{
  if (isEnabled() || disabledImage().isNull())
  {
    // Get size of a single sub image:
    int w = image().width() / 2;
    int h = image().height();

    // Calc source position:
    int x = m_value ? w : 0;

    // Finally blit the image:
    qp.drawPixmap(0, 0, image(), x, 0, w, h);
  }
  else
  {
    // Just show the disabled image:
    qp.drawPixmap(0, 0, disabledImage());
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageToggle::valueFromMousePos()
////////////////////////////////////////////////////////////////////////////////
///\brief  Internal helper to calc a value for mouse coordinates.
///\param  [in] mx: Input position, X component.
///\param  [in] my: Input position, Y component.
///\return The matching value for the input coordinates.
////////////////////////////////////////////////////////////////////////////////
bool ImageToggle::valueFromMousePos(const int mx, const int my) const
{
  // Get center:
  int bx = width() / 2;
  int by = height() / 2;

  // Find matching half:
  if (m_leftRight)
    return mx < bx;
  return my < by;
}

///////////////////////////////// End of File //////////////////////////////////
