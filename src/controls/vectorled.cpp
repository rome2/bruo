////////////////////////////////////////////////////////////////////////////////
// (c) 2017 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    vectorled.cpp
///\ingroup bruo
///\brief   Vector based LED widget implementation.
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
#include "vectorled.h"

////////////////////////////////////////////////////////////////////////////////

VectorLED::VectorLED(QWidget* parent) :
  QWidget(parent),
  m_value(false),
  m_tag(0),
  m_backColor(192, 192, 192)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////

VectorLED::~VectorLED()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////

bool VectorLED::value() const
{
  // Return current value:
  return m_value;
}

////////////////////////////////////////////////////////////////////////////////

void VectorLED::setValue(const bool newVal)
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

int VectorLED::tag() const
{
  // Return current tag:
  return m_tag;
}

////////////////////////////////////////////////////////////////////////////////

void VectorLED::setTag(const int newTag)
{
  // Set new tag:
  m_tag = newTag;
}

////////////////////////////////////////////////////////////////////////////////

const QColor& VectorLED::backColor() const
{
  // Return current color:
  return m_backColor;
}

////////////////////////////////////////////////////////////////////////////////

void VectorLED::setBackColor(const QColor& color)
{
  // Update color:
  m_backColor = color;

  // Force redraw:
  update();
}

////////////////////////////////////////////////////////////////////////////////

void VectorLED::paintEvent(QPaintEvent* /* event */)
{
  QPainter painter(this);

  // Fill background:
  QRect rc = rect();
  painter.fillRect(rc, m_backColor);

  QRect rc2;
  rc2.setX(rc.width() / 4);
  rc2.setY(rc.height() / 4);
  rc2.setWidth(rc.width() / 2);
  rc2.setHeight(rc.height() / 2);

  if (!isEnabled())
  {
    painter.fillRect(rc2, QColor(96, 105, 113));
    painter.setPen(QColor(86, 95, 103));
    painter.drawRect(rc2);
  }
  else if (m_value)
  {
    painter.fillRect(rc2, QColor(239, 134, 19));
    painter.setPen(QColor(96, 105, 113));
    painter.drawRect(rc2);
  }
  else
  {
    painter.fillRect(rc2, QColor(56, 61, 69));
    painter.setPen(QColor(120, 129, 137));
    painter.drawRect(rc2);
  }
}

////////////////////////////////////////////////////////////////////////////////

void VectorLED::mouseReleaseEvent(QMouseEvent* /*event*/)
{
  // Notify listeners:
  if (!signalsBlocked())
    emit clicked();
}

////////////////////////////////////////////////////////////////////////////////

void VectorLED::changeEvent(QEvent* event)
{
  // Base handling:
  QWidget::changeEvent(event);

  // Redraw if the enabled state changed:
  if (event->type() == QEvent::EnabledChange)
    update();
}

///////////////////////////////// End of File //////////////////////////////////
