////////////////////////////////////////////////////////////////////////////////
// (c) 2014 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    imagedial.cpp
///\ingroup bruo
///\brief   Image based rotary widget implementation.
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
#include "imagedial.h"

////////////////////////////////////////////////////////////////////////////////
// ImageDial::ImageDial()
////////////////////////////////////////////////////////////////////////////////
///\brief   Default constructor of this widget.
///\param   [in] parent: Parent window for this widget.
///\remarks Just initializes the members.
////////////////////////////////////////////////////////////////////////////////
ImageDial::ImageDial(QWidget* parent) :
  ImageWidget(parent),
  m_value(0.5),
  m_defaultValue(0.5),
  m_frameCount(0),
  m_absoluteMode(false),
  m_circularMode(false),
  m_startY(0),
  m_startVal(0.5),
  m_linearSize(128)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::~ImageDial()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Cleans up used resources.
////////////////////////////////////////////////////////////////////////////////
ImageDial::~ImageDial()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::value()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the Value property.
///\return  The current value of this dial.
///\remarks The value is always in the range [0,1].
////////////////////////////////////////////////////////////////////////////////
double ImageDial::value() const
{
  // Return current value:
  return m_value;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::setValue()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the Value property.
///\param   [in] newVal: The new value of this dial.
///\remarks The value is clipped to the range [0,1]. If this value is set then
///         the widget will be repainted and a valueChanged event will be
///         emitted.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::setValue(const double newVal)
{
  // Set new value:
  m_value = newVal;

  // Clip value in the range [0,1]:
  if (m_value < 0.0)
    m_value = 0.0;
  else if (m_value > 1.0)
    m_value = 1.0;

  // Schedule redraw:
  update();

  // Notify listeners:
  if (!signalsBlocked())
    emit valueChanged(m_value);
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::defaultValue()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the DefaultValue property.
///\return  The default value of this dial.
///\remarks The value is always in the range [0,1]. To load the default value
///\        just double click the widget.
////////////////////////////////////////////////////////////////////////////////
double ImageDial::defaultValue() const
{
  // Return current value:
  return m_defaultValue;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::setDefaultValue()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the DefaultValue property.
///\param   [in] newVal: The new default value of this dial.
///\remarks The value is clipped to the range [0,1]. To load the default value
///\        just double click the widget.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::setDefaultValue(const double newVal)
{
  // Set new value:
  m_defaultValue = newVal;

  // Clip value in the range [0,1]:
  if (m_defaultValue < 0.0)
    m_defaultValue = 0.0;
  else if (m_defaultValue > 1.0)
    m_defaultValue = 1.0;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::frameCount()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the FrameCount property.
///\return  The current number of frames for the knob movie.
///\remarks This property tells this widget how man frames there are in the
///         knob movie and thus how to determine the subpicture to show.
////////////////////////////////////////////////////////////////////////////////
int ImageDial::frameCount() const
{
  // Return current count:
  return m_frameCount;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::setFrameCount()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the FrameCount property.
///\param   [in] newCount: The new number of frames for the knob movie.
///\remarks This property tells this widget how man frames there are in the
///         knob movie and thus how to determine the subpicture to show.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::setFrameCount(const int newCount)
{
  // Set new value:
  m_frameCount = newCount;

  // Clip value:
  if (m_frameCount <= 0)
    m_frameCount = 1;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::absoluteMode()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the AbsoluteMode property.
///\return  Whether absolute mode is enabled or not.
///\remarks In absolute mode the dial value will instantly change to the
///         value indicated by the mouse click position. In relative mode it
///         will be changed relative to the click position.
////////////////////////////////////////////////////////////////////////////////
bool ImageDial::absoluteMode() const
{
  // Return current state:
  return m_absoluteMode;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::setAbsoluteMode()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the AbsoluteMode property.
///\param   [in] newState: Whether to enable absolute mode or not.
///\remarks In absolute mode the dial value will instantly change to the
///         value indicated by the mouse click position. In relative mode it
///         will be changed relative to the click position.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::setAbsoluteMode(const bool newState)
{
  // Set new state:
  m_absoluteMode = newState;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::circularMode()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the CircularMode property.
///\return  Whether circular motion is enabled or linear.
///\remarks In circular mode value can be changed by a circular move around
///         the center of the widget. In linear mode it is just an up down
///         movement. The linear movement range is defined by the LinearSize
///         property.
////////////////////////////////////////////////////////////////////////////////
bool ImageDial::circularMode() const
{
  // Return current state:
  return m_circularMode;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::setCircularMode()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the CircularMode property.
///\param   [in]WnewState: Whether to enable circular motion or linear.
///\remarks In circular mode value can be changed by a circular move around
///         the center of the widget. In linear mode it is just an up down
///         movement. The linear movement range is defined by the LinearSize
///         property.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::setCircularMode(const bool newState)
{
  // Set new state:
  m_circularMode = newState;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::linearSize()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the LinearSize property.
///\return  The current extends of the linear move mode.
///\remarks In linear mode this property describes the virtual fader size or,
///         in different words, the number of pixels that the value range of
///         [0,1] will be mapped to.
////////////////////////////////////////////////////////////////////////////////
int ImageDial::linearSize() const
{
  // Return current size:
  return m_linearSize;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::setLinearSize()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the LinearSize property.
///\param   [in] newSize: The new extends of the linear move mode.
///\remarks In linear mode this property describes the virtual fader size or,
///         in different words, the number of pixels that the value range of
///         [0,1] will be mapped to.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::setLinearSize(const int newSize)
{
  // Set new size:
  m_linearSize = newSize;

  // Clip size:
  if (m_linearSize <= 0)
    m_linearSize = 1;
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::wheelEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the mouse wheel signal.
///\param [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::wheelEvent(QWheelEvent* event)
{
  //Clac value delta:
  double delta = event->delta() * 0.00025;

  // Update value:
  setValue(m_value + delta);
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::mousePressEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the mouse button pressed signal.
///\param [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::mousePressEvent(QMouseEvent* event)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Left click?
  if (event->buttons() == Qt::LeftButton)
  {
    // Use circular mode?
    if (m_circularMode)
    {
      // Get value from the mouse position point:
      m_startVal = valueFromMousePos(event->x(), event->y());

      // Make value current if needed:
      if (m_absoluteMode)
        setValue(m_startVal);
    }

    // No, linear is the way to go:
    else
    {
      // Save start values:
      m_startVal = m_value;
      m_startY   = event->y();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageButton::mouseReleaseEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the mouse button released signal.
///\param [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::mouseReleaseEvent(QMouseEvent* /* event */)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Notify listeners:
  if (!signalsBlocked())
    emit mouseReleased();
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::mouseDoubleClickEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the mouse double click signal.
///\param [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::mouseDoubleClickEvent(QMouseEvent* event)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Left click?
  if (event->buttons() == Qt::LeftButton)
  {
    // Load default:
    setValue(m_defaultValue);
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::mouseMoveEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the mouse moved signal.
///\param [in] event: Provides further details about the event.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::mouseMoveEvent(QMouseEvent* event)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Left drag:
  if (event->buttons() == Qt::LeftButton)
  {
    // Running in circles?
    if (m_circularMode)
    {
      // Get value from the mouse position point:
      double val = valueFromMousePos(event->x(), event->y());

      // Set value:
      if (m_absoluteMode)
        setValue(val);
      else
      {
        // Set new value relative to the last value:
        setValue(m_value + (val - m_startVal));

        // Save current value for the next round:
        m_startVal = val;
      }
    }

    // No, we're imitating a fader:
    else
    {
      // Calc movement in pixels:
      double dy = m_startY - event->y();

      // Scale into a more usable range:
      double diff = dy / m_linearSize;

      // Set new value relative to the start value:
      setValue(m_startVal + diff);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::drawWidget()
////////////////////////////////////////////////////////////////////////////////
///\brief Internal helper that paints the actual widget.
///\param [in] qp: Device context to use.
////////////////////////////////////////////////////////////////////////////////
void ImageDial::drawWidget(QPainter& qp)
{
  qp.fillRect(0, 0, width(), height(), QColor(106, 115, 123));
  qp.setRenderHint(qp.Antialiasing);

  int ref = qMin(width(), height());
  float x = 0.5f * width();
  float y = 0.5f * height();
  float r = 0.3f * ref;
  int penWidth = qMax(1, ref / 22);

  QPen pen(QColor(84, 93, 102));
  pen.setWidth(penWidth);
  qp.setPen(pen);
  qp.setBrush(QBrush(QColor(56, 61, 69)));
  qp.drawEllipse(QPointF(x, y), r, r);

  float offset = 6.28f / 8.0f;
  float arcOffset = offset / 6.28f * 360.0f * 16.0f + 180.0f * 16.0f;
  float arcLen = (6.28f - 2.0f * offset) / 6.28f * 360.0f * 16.0f;
  QRectF arcRect(ref * 0.1f, ref * 0.1f, ref * 0.8f, ref * 0.8f);
  qp.drawArc(arcRect, arcOffset, -arcLen);

  float a = offset + (m_value * (6.28f - (2.0f * offset)));

  float dx = -sin(a) * r;
  float dy = cos(a) * r;

  // Calc new coords:
  float x2 = x + dx;
  float y2 = y + dy;
  x += dx * 0.25f;
  y += dy * 0.25f;

  QPen pen2(QColor(225, 226, 227));
  pen2.setWidth(penWidth);
  qp.setPen(pen2);
  qp.drawLine(QPointF(x, y), QPointF(x2, y2));

  QPen pen3(QColor(239, 134, 19));
  //QPen pen3(QColor(180, 180, 180));
  pen3.setWidth(penWidth);
  qp.setPen(pen3);

  qp.drawArc(arcRect, arcOffset, -arcLen * m_value);

  return;

  if (isEnabled() || disabledImage().isNull())
  {
    // Get size of a single sub image:
    int w = image().width() / m_frameCount;
    int h = image().height();

    // Calc active frame:
    int l = (int)(m_value * (m_frameCount - 1));
    if (l < 0)
      l = 0;
    if (l > (m_frameCount - 1))
      l = m_frameCount - 1;

    // Calc source position:
    int x = w * l;
    int y = 0;

    // Finally blit the image:
    qp.drawPixmap(0, 0, image(), x, y, w, h);
  }

  else
  {
    // Just show the disabled image:
    qp.drawPixmap(0, 0, disabledImage());
  }
}

////////////////////////////////////////////////////////////////////////////////
// ImageDial::valueFromMousePos()
////////////////////////////////////////////////////////////////////////////////
///\brief  Internal helper to calc a dial value for mouse coordinates.
///\param  [in] mx: Input position, X component.
///\param  [in] my: Input position, Y component.
///\return The matching value for the input coordinates.
////////////////////////////////////////////////////////////////////////////////
double ImageDial::valueFromMousePos(const int mx, const int my) const
{
  // Get coordinates with respect to the control center:
  double x = (size().width() / 2.0) - mx;
  double y = (size().height() / 2.0) - my;

  // Normalize the values to get a direction vector:
  double len = sqrt(x * x + y * y);
  if (len > 0.0)
  {
    x /= len;
    y /= len;

    // Calc value:
    double val = acos(y) * (x < 0.0 ? 1.0 : -1.0);

    // Move into range [0,1]:
    val += 3.14;
    val /= 6.28;

    // Return the value:
    return val;
  }

  // We hit the center, return current value:
  return m_value;
}

///////////////////////////////// End of File //////////////////////////////////

