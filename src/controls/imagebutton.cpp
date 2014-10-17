////////////////////////////////////////////////////////////////////////////////
// (c) 2014 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    imagebutton.cpp
///\ingroup bruo
///\brief   Image based button widget implementation.
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
#include "imagebutton.h"

////////////////////////////////////////////////////////////////////////////////
// ImageButton::ImageButton()
////////////////////////////////////////////////////////////////////////////////
///\brief   Default constructor of this widget.
///\param   [in] parent: Parent window for this widget.
///\remarks Just initializes the members.
////////////////////////////////////////////////////////////////////////////////
ImageButton::ImageButton(QWidget* parent) :
  ImageWidget(parent),
  m_down(false)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// ImageButton::~ImageButton()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Cleans up used resources.
////////////////////////////////////////////////////////////////////////////////
ImageButton::~ImageButton()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// ImageButton::mousePressEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the mouse button pressed signal.
///\param [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageButton::mousePressEvent(QMouseEvent* event)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Left click?
  if (event->buttons() == Qt::LeftButton)
  {
    // Update display:
    m_down = true;
    update();
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageButton::mouseReleaseEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the mouse button released signal.
///\param [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageButton::mouseReleaseEvent(QMouseEvent* event)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Left click?
  if (!(event->buttons() & Qt::LeftButton))
  {
    // Force redraw:
    m_down = false;
    update();

    // Check bounds:
    if (event->x() < 0 || event->x() >= width())
      return;
    if (event->y() < 0 || event->y() >= height())
      return;

    // Notify listeners:
    if (!signalsBlocked())
      emit clicked();
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageButton::mouseMoveEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the mouse moved signal.
///\param [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageButton::mouseMoveEvent(QMouseEvent* event)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Left drag:
  if (event->buttons() == Qt::LeftButton)
  {
    // Check bounds:
    bool inside = true;
    if (event->x() < 0 || event->x() >= width() || event->y() < 0 || event->y() >= height())
      inside = false;

    // State changed?
    if (inside != m_down)
    {
      // Repaint:
      m_down = inside;
      update();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageButton::drawWidget()
////////////////////////////////////////////////////////////////////////////////
///\brief Internal helper that paints the actual widget.
///\param [in] qp: Device context to use.
////////////////////////////////////////////////////////////////////////////////
void ImageButton::drawWidget(QPainter& qp)
{
  if (isEnabled() || disabledImage().isNull())
  {
    // Get size of a single sub image:
    int w = image().width() / 2;
    int h = image().height();

    // Calc source position:
    int x = m_down ? w : 0;

    // Finally blit the image:
    qp.drawPixmap(0, 0, image(), x, 0, w, h);
  }
  else
  {
    // Just show the disabled image:
    qp.drawPixmap(0, 0, disabledImage());
  }
}

///////////////////////////////// End of File //////////////////////////////////

