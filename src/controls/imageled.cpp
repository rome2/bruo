////////////////////////////////////////////////////////////////////////////////
// (c) 2014 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    imageled.cpp
///\ingroup bruo
///\brief   Image based LED widget implementation.
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
#include "imageled.h"

////////////////////////////////////////////////////////////////////////////////
// ImageLED::ImageLED()
////////////////////////////////////////////////////////////////////////////////
///\brief   Default constructor of this widget.
///\param   [in] parent: Parent window for this widget.
///\remarks Just initializes the members.
////////////////////////////////////////////////////////////////////////////////
ImageLED::ImageLED(QWidget* parent) :
  ImageWidget(parent),
  m_value(false)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// ImageLED::~ImageLED()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Cleans up used resources.
////////////////////////////////////////////////////////////////////////////////
ImageLED::~ImageLED()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// ImageLED::value()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the Value property.
///\return  The current value of this LED.
///\remarks The value is either true or false.
////////////////////////////////////////////////////////////////////////////////
bool ImageLED::value() const
{
  // Return current value:
  return m_value;
}

////////////////////////////////////////////////////////////////////////////////
// ImageLED::setValue()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the Value property.
///\param   [in] newVal: The new value of this LED.
///\remarks The widget will be repainted.
////////////////////////////////////////////////////////////////////////////////
void ImageLED::setValue(const bool newVal)
{
  // Anything to do?
  if (m_value == newVal)
    return;

  // Set new value:
  m_value = newVal;

  // Force redraw:
  update();
}

////////////////////////////////////////////////////////////////////////////////
// ImageLED::drawWidget()
////////////////////////////////////////////////////////////////////////////////
///\brief Internal helper that paints the actual widget.
///\param [in] qp: Device context to use.
////////////////////////////////////////////////////////////////////////////////
void ImageLED::drawWidget(QPainter& qp)
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

void ImageLED::mouseReleaseEvent(QMouseEvent* /*event*/)
{
  // Notify listeners:
  if (!signalsBlocked())
    emit clicked();
}

///////////////////////////////// End of File //////////////////////////////////
