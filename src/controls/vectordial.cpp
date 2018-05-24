////////////////////////////////////////////////////////////////////////////////
// (c) 2017 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    vectordial.cpp
///\ingroup bruo
///\brief   Vector based rotary widget implementation.
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
#include "vectordial.h"

////////////////////////////////////////////////////////////////////////////////

VectorDial::VectorDial(QWidget* parent) :
  QWidget(parent),
  m_value(0.5),
  m_defaultValue(0.5),
  m_absoluteMode(false),
  m_circularMode(false),
  m_startY(0),
  m_startVal(0.5),
  m_linearSize(128),
  m_drawArc(true),
  m_arcBase(0.0)
{
  // Optimize performance:
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  setAttribute(Qt::WA_NoSystemBackground, true);

  // Make sure that are staying square:
  QSizePolicy p = sizePolicy();
  p.setHeightForWidth(true);
  setSizePolicy(p);
}

////////////////////////////////////////////////////////////////////////////////

VectorDial::~VectorDial()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////

double VectorDial::value() const
{
  // Return current value:
  return m_value;
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::setValue(const double newVal)
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

double VectorDial::defaultValue() const
{
  // Return current value:
  return m_defaultValue;
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::setDefaultValue(const double newVal)
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

bool VectorDial::absoluteMode() const
{
  // Return current state:
  return m_absoluteMode;
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::setAbsoluteMode(const bool newState)
{
  // Set new state:
  m_absoluteMode = newState;
}

////////////////////////////////////////////////////////////////////////////////

bool VectorDial::circularMode() const
{
  // Return current state:
  return m_circularMode;
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::setCircularMode(const bool newState)
{
  // Set new state:
  m_circularMode = newState;
}

////////////////////////////////////////////////////////////////////////////////

int VectorDial::linearSize() const
{
  // Return current size:
  return m_linearSize;
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::setLinearSize(const int newSize)
{
  // Set new size:
  m_linearSize = newSize;

  // Clip size:
  if (m_linearSize <= 0)
    m_linearSize = 1;
}

////////////////////////////////////////////////////////////////////////////////

double VectorDial::arcBase() const
{
  // Return current base:
  return m_arcBase;
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::setArcBase(const double base)
{
  if (base < 0.0)
    m_arcBase = 0.0;
  else if (base > 1.0)
    m_arcBase = 1.0;
  else
    m_arcBase = base;
}

////////////////////////////////////////////////////////////////////////////////

int VectorDial::tag() const
{
  // Return current tag:
  return m_tag;
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::setTag(const int newTag)
{
  // Set new tag:
  m_tag = newTag;
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::wheelEvent(QWheelEvent* event)
{
  //Clac value delta:
  double delta = event->delta() * 0.00025;

  // Update value:
  setValue(m_value + delta);
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::mousePressEvent(QMouseEvent* event)
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

void VectorDial::mouseReleaseEvent(QMouseEvent* /* event */)
{
  // Enabled?
  if (!isEnabled())
      return;

  // Notify listeners:
  if (!signalsBlocked())
    emit mouseReleased();
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::mouseDoubleClickEvent(QMouseEvent* event)
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

void VectorDial::mouseMoveEvent(QMouseEvent* event)
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

void VectorDial::paintEvent(QPaintEvent* /* event */)
{
  // Prpeare painter:
  QPainter qp(this);
  qp.setRenderHint(qp.Antialiasing);

  // Clear background:
  qp.fillRect(0, 0, width(), height(), QColor(106, 115, 123));

  int ref = qMin(width(), height());
  double x = 0.5 * width();
  double y = x;
  double r = (m_drawArc ? 0.3 : 0.4) * ref;
  int penWidth = ref > 25 ? 2 : 1;

  // Draw dial background and border:
  QPen pen(QColor(84, 93, 102));
  pen.setWidth(penWidth);
  qp.setPen(pen);
  qp.setBrush(QBrush(QColor(56, 61, 69)));
  qp.drawEllipse(QPointF(x, y), r, r);

  // Get indent angle:
  double offset = 6.28 / 8.0;
  double a = offset + (m_value * (6.28 - (2.0 * offset)));

  // Calc new coords for indent:
  double dx = -sin(a) * r;
  double dy = cos(a) * r;
  double x2 = x + dx;
  double y2 = y + dy;
  double x3 = x + dx * 0.25;
  double y3 = y + dy * 0.25;

  // Draw indent:
  QPen pen2(QColor(225, 226, 227));
  pen2.setWidth(penWidth);
  qp.setPen(pen2);
  qp.drawLine(QPointF(x3, y3), QPointF(x2, y2));

  double pt_dist = m_drawArc ? r * 1.6 : r * 1.2;
  dx = -sin(offset) * pt_dist;
  dy = cos(offset) * pt_dist;
  x2 = x + dx;
  y2 = y + dy;
  x3 = x - dx;
  y3 = y - pt_dist;

  QPen pen5(QColor(205, 206, 207));
  pen5.setWidth(penWidth);
  qp.setPen(pen5);
  qp.drawPoint(QPointF(x, y3));
  qp.drawPoint(QPointF(x2, y2));
  qp.drawPoint(QPointF(x3, y2));

  // Draw arc, if needed:
  if (m_drawArc)
  {
    // Convert angles to QPainter angles:
    double arcOffset = offset / 6.28 * 360.0 * 16.0 + 180 * 16;
    double arcLen = -(6.28 - 2.0 * offset) / 6.28 * 360.0 * 16.0;

    // Calc arc dimensions:
    QRectF arcRect(ref * 0.1, ref * 0.1, ref * 0.8, ref * 0.8);

    // Draw background:
    QPen pen3(QColor(84, 93, 102));
    pen3.setWidth(penWidth);
    qp.setPen(pen3);
    qp.drawArc(arcRect, arcOffset, arcLen);

    // Draw active area:
    QPen pen4(QColor(239, 134, 19));
    pen4.setWidth(penWidth);
    qp.setPen(pen4);
    qp.drawArc(arcRect, arcOffset + (arcLen * m_arcBase), arcLen * (m_value - m_arcBase));
  }
}

////////////////////////////////////////////////////////////////////////////////

void VectorDial::changeEvent(QEvent* event)
{
  // Base handling:
  QWidget::changeEvent(event);

  // Redraw if the enabled state changed:
  if (event->type() == QEvent::EnabledChange)
    update();
}

////////////////////////////////////////////////////////////////////////////////

double VectorDial::valueFromMousePos(const int mx, const int my) const
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
