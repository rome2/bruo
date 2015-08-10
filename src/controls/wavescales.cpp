////////////////////////////////////////////////////////////////////////////////
// (c) 2014 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    wavescales.h
///\ingroup bruo
///\brief   Wave view peak scale widget implementation.
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
#include "wavescales.h"
#include "waveeditview.h"

////////////////////////////////////////////////////////////////////////////////
// WaveScales::WaveScales()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this class.
///\param   [in] editWindow: The master wave editor.
///\param   [in] parent:     Parent for this instance.
///\remarks Attaches itself to the master so it is synced automatically.
////////////////////////////////////////////////////////////////////////////////
WaveScales::WaveScales(WaveEditView* editWindow, QWidget* parent) :
  QWidget(parent),
  m_master(editWindow),
  m_backColor(160, 160,160),
  m_foreColor(0, 0, 0),
  m_backBuff(0),
  m_dragStarted(false),
  m_oldZoom(-1.0),
  m_oldPos(-1.0)
{
  if (m_master != 0)
  {
    // Connect to master:
    connect(m_master, SIGNAL(viewportChanged()), this, SLOT(masterViewportChanged()));

    // Connect to document:
    if (m_master->document() != 0)
      connect(m_master->document(), SIGNAL(scaleFormatChanged()), this, SLOT(scaleFormatChanged()));
  }

  // We want an arrow cursor:
  setCursor(Qt::ArrowCursor);
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::~WaveScales()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this widget.
///\remarks Does final cleanup.
////////////////////////////////////////////////////////////////////////////////
WaveScales::~WaveScales()
{
  // Delete back buffer:
  if (m_backBuff != 0)
    delete m_backBuff;
  m_backBuff = 0;
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::backColor()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the background color of this widget.
///\return  The background color of this widget.
////////////////////////////////////////////////////////////////////////////////
const QColor& WaveScales::backColor() const
{
  // Return current color:
  return m_backColor;
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::setBackColor()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set a new background color for this widget.
///\param   [in] newColor: The new background color of this widget.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::setBackColor(const QColor& newColor)
{
  // Anything to do?
  if (m_backColor == newColor)
    return;

  // Update value:
  m_backColor = newColor;

  // Redraw scales:
  redraw();
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::foreColor()
////////////////////////////////////////////////////////////////////////////////
///\brief   Access the foreground color of this widget.
///\return  The foreground color of this widget.
///\remarks This color is used for the lines and text.
////////////////////////////////////////////////////////////////////////////////
const QColor& WaveScales::foreColor() const
{
  // Return current color:
  return m_foreColor;
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::setForeColor()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set a new foreground color for this widget.
///\param   [in] newColor: The new foreground color of this widget.
///\remarks This color is used for the lines and text.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::setForeColor(const QColor& newColor)
{
  // Anything to do?
  if (m_foreColor == newColor)
    return;

  // Update value:
  m_foreColor = newColor;

  // Redraw scales:
  redraw();
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::paintEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the paint event.
///\param   [in] e: Details of the event.
///\remarks This widget uses double buffering so this function does a buffer
///         swap. The actual drawing is done with redraw().
////////////////////////////////////////////////////////////////////////////////
void WaveScales::paintEvent(QPaintEvent* /* event */)
{
  // Got a back buffer?
  if (m_backBuff == 0)
    return;

  // Create painter:
  QPainter painter(this);

  // Draw scales:
  painter.drawPixmap(0, 0, *m_backBuff);
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::resizeEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the resize event.
///\param   [in] e: Details of the event.
///\remarks Updates the back buffer.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::resizeEvent(QResizeEvent* /* event */)
{
  // Don't do anything without a master:
  if (m_master == 0)
    return;

  // Update back buffer if needed:
  if (m_backBuff == 0 || width() != m_backBuff->width() || height() != m_backBuff->height())
  {
    // Delete old buffer:
    if (m_backBuff != 0)
      delete m_backBuff;
    m_backBuff = 0;

    // Create new:
    if (width() > 0 && height() > 0)
    {
      m_backBuff = new QPixmap(width(), height());
      m_backBuff->fill();
    }
  }

  // Redraw scales:
  redraw();
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::wheelEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the paint event.
///\param   [in] e: Details of the event.
///\remarks Scrolls the view up and down.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::wheelEvent(QWheelEvent* event)
{
  // No scrolling while we are doing something else:
  if (event->buttons() != Qt::NoButton)
    return;

  // Vertical scrolling?
  if (event->orientation() == Qt::Vertical)
  {
    if (event->delta() > 0)
      m_master->setPosV(m_master->posV() + 0.03);
    else
      m_master->setPosV(m_master->posV() - 0.03);
  }
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::mouseMoveEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the mouse move event.
///\param   [in] e: Details of the event.
///\remarks This widget doesn't use mouse tracking so this is only called when
///         a button is pressed.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::mouseMoveEvent(QMouseEvent* event)
{
  // Don't do anything without a master:
  if (m_master == 0)
    return;

  // Already dragged enough?
  if (!m_dragStarted)
  {
    int dx = event->pos().x() - m_mouseDownPos.x();
    int dy = event->pos().y() - m_mouseDownPos.y();
    if (abs(dx) > 2 || abs(dy) > 2)
      m_dragStarted = true;
    else
      return;
  }

  // Get movement from the last position:
  int dy = event->pos().y() - m_lastMousePos.y();

  // Moving the content vertically with the middle button?
  if (dy != 0 && event->buttons() == Qt::MiddleButton)
  {
    // Zoomed out fully?
    if (m_master->zoomV() > 1.0)
    {
      // Calc visible height of a single channel:
      double channelHeight = (double)height() / m_master->document()->channelCount();

      // Calc relative movement in the zoomed wave:
      double dz = 1.0 / (channelHeight * m_master->zoomV() - channelHeight);

      // Update position:
      m_master->setPosV(m_master->posV() + dz * dy);
    }
  }

  // Save position for next call:
  m_lastMousePos = event->pos();
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::mousePressEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the mouse button press event.
///\param   [in] e: Details of the event.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::mousePressEvent(QMouseEvent* event)
{
  // Don't do anything without a master:
  if (m_master == 0)
    return;

  // Update cursor:
  if (event->buttons() == Qt::MiddleButton)
    setCursor(Qt::ClosedHandCursor);
  else
    return;

  // Save positions:
  m_lastMousePos = event->pos();
  m_mouseDownPos = event->pos();
  m_dragStarted = false;
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::mouseReleaseEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the mouse button release event.
///\param   [in] e: Details of the event.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::mouseReleaseEvent(QMouseEvent* event)
{
  // Show context menu?
  if (!m_dragStarted && event->button() == Qt::RightButton)
    showContextMenu(event->pos());

  // Reset mode:
  m_dragStarted = false;

  // Update the cursor:
  setCursor(Qt::ArrowCursor);
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::masterViewportChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the master viewport changed event.
///\remarks Updates the ruler.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::masterViewportChanged()
{
  // Anything to do?
  if (m_oldZoom == m_master->zoomV() && m_oldPos  == m_master->posV())
    return;

  // Redraw scales:
  redraw();

  // Save state:
  m_oldZoom = m_master->zoomV();
  m_oldPos  = m_master->posV();
}

//////////////////////////////////////////////////////////////////////////////
// WaveScales::scaleFormatChanged()
//////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the scale format changed event.
///\remarks Updates the ruler.
//////////////////////////////////////////////////////////////////////////////
void WaveScales::scaleFormatChanged()
{
  // Redraw scales:
  redraw();
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::redraw()
////////////////////////////////////////////////////////////////////////////////
///\brief   Force a redraw of the scales.
///\remarks Redraws the scales and invalidates the widget.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::redraw()
{
  // Redraw scales:
  if (m_backBuff != 0)
  {
    QPainter scalePainter(m_backBuff);
    drawScales(scalePainter);
  }

  // Update screen:
  update();
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::drawScales()
////////////////////////////////////////////////////////////////////////////////
///\brief   The actual scale drawing function.
///\param   [in] painter: The painter to use.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::drawScales(QPainter& painter)
{
  // Don't do anything without a master:
  if (m_master == 0)
    return;

  // Got a document?
  if (!m_master->document())
    return;

  // Get mode:
  Document::ScaleMode scaleMode = m_master->document()->scaleMode();

  // Clear background:
  painter.fillRect(rect(), m_backColor);

  // Draw divider:
  painter.setPen(m_master->dividerColor());
  painter.drawLine(width() - 1, 0, width() - 1, height());

  // Get height of a single channel:
  double channelHeight = (double)height() / m_master->document()->channelCount();

  // This where the marker lines start:
  int markerX = (int)(0.75 * width());

  // Calc maximum text height:
  int fontHeight = painter.fontMetrics().height();
  int lineHeight = fontHeight * 3;

  // Enable clipping:
  painter.setClipping(true);

  // Draw channels:
  for (int channel = 0; channel < m_master->document()->channelCount(); channel++)
  {
    // Set text and line color:
    painter.setPen(m_foreColor);

    // Create target rect:
    QRect destRect(0, channel * channelHeight, width(), channelHeight);
    int ymin = destRect.top();
    int ymax = destRect.bottom();

    // Clip to target:
    painter.setClipRect(destRect);

    // Get center line:
    double y = destRect.top() + (channelHeight * 0.5) + (((channelHeight * m_master->zoomV()) - channelHeight) * (m_master->posV() - 0.5));

    // Draw center line:
    painter.drawLine(markerX, y, destRect.right(), y);

    // Draw center text:
    QRect centerRect(0, (int)(y - (fontHeight * 0.5)), markerX - 2, fontHeight);
    if (scaleMode == Document::dB)
      painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "-oo");
    else if (scaleMode == Document::Normalized)
      painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "0.000");
    else if (scaleMode == Document::Percent)
      painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "0.0");
    else
      painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "0");

    // Calc division factor for the optimal display spacing:
    double div = 1.0;
    while ((channelHeight * m_master->zoomV() * div) > lineHeight)
      div *= 0.5;
    double dy = channelHeight * m_master->zoomV() * div * m_master->zoomVOverlap();

    // Draw all values:
    for (int i = 0; i < 2; i++)
    {
      int line = 0;
      while (true)
      {
        // Select line:
        line++;
        if (scaleMode == Document::Bits8 && line > 7)
          break;
        else if (scaleMode == Document::Bits16 && line > 15)
          break;
        else if (scaleMode == Document::Bits24 && line > 23)
          break;

        // Calc display value:
        double val;
        if (scaleMode == Document::dB)
          val = 20.0 * log10(line * div * 2.0);
        else if (scaleMode == Document::Normalized)
          val = line * div * 2.0;
        else if (scaleMode == Document::Percent)
          val = line * div * 200.0;
        else
          val = line;

        // Reproject value to find int position:
        double ty = 0.0;
        if (scaleMode == Document::dB)
          ty = (pow(10.0, val / 20.0) * dy / (div * 2.0));
        else if (scaleMode == Document::Normalized)
          ty = val * dy / (div * 2.0);
        else if (scaleMode == Document::Percent)
          ty = val * dy / (div * 200.0);
        else if (scaleMode == Document::Bits8)
        {
          ty = pow(2.0, val - 1.0) / 128.0 * dy / div;
          if (ty < fontHeight)
            continue;
        }
        else if (scaleMode == Document::Bits16)
        {
          ty = pow(2.0, val - 1.0) / 32768.0 * dy / div;
          if (ty < fontHeight)
            continue;
        }
        else if (scaleMode == Document::Bits24)
        {
          ty = pow(2.0, val - 1.0) / 8388608.0 * dy / div;
          if (ty < fontHeight)
            continue;
        }
        if (i == 0)
          ty = y - ty;
        else
          ty = y + ty;

        // Calc text rect:
        QRect textRect(0, (int)(ty - (fontHeight * 0.5)), markerX - 2, fontHeight);
        if (i == 0)
        {
          if (textRect.bottom() < ymin)
            break;
          if (textRect.top() > ymax)
            continue;
        }
        else
        {
          if (textRect.top() > ymax)
            break;
          if (textRect.bottom() < ymin)
            continue;
        }

        // Draw the scale line:
        painter.drawLine(markerX, ty, destRect.right(), ty);

        // Draw the text:
        if (scaleMode == Document::dB && val > 0)
          painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString("+") + QString::number(val, 'f', 1));
        else if (scaleMode == Document::Normalized)
          painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(val, 'f', 3));
        else if (scaleMode == Document::Percent)
          painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(val, 'f', 1));
        else
          painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(val, 'f', 0));
      }
    }

    // Draw channel divider:
    if (m_master->drawChannelDivider() && channel > 0)
    {
      painter.setPen(m_master->dividerColor());
      painter.drawLine(0, destRect.top(), destRect.right(), destRect.top());
    }
  }

  // Disable clipping:
  painter.setClipping(false);
}

////////////////////////////////////////////////////////////////////////////////
// WaveScales::showContextMenu()
////////////////////////////////////////////////////////////////////////////////
///\brief   Show this widget's context menu.
///\param   [in] pos: Position in client coordinates of the context menu.
////////////////////////////////////////////////////////////////////////////////
void WaveScales::showContextMenu(const QPoint& pos)
{
  // The context menu wants screen coordinates:
  QPoint globalPos = mapToGlobal(pos);

  // Compose the menu:
  QMenu myMenu;
  myMenu.addAction(tr("dB"))->setCheckable(true);
  myMenu.addAction(tr("Percent"))->setCheckable(true);
  myMenu.addAction(tr("Normalized"))->setCheckable(true);
  myMenu.addAction(tr("8 Bits"))->setCheckable(true);
  myMenu.addAction(tr("16 Bits"))->setCheckable(true);
  myMenu.addAction(tr("24 Bits"))->setCheckable(true);
  myMenu.addSeparator();
  myMenu.addAction(tr("Save as default)"));
  myMenu.actions().at((int)m_master->document()->scaleMode())->setChecked(true);

  // Show the menu:
  QAction* selectedItem = myMenu.exec(globalPos);
  if (selectedItem == 0)
    return;

  // Hit a mode?
  if (selectedItem->isCheckable())
  {
    // Update mode:
    Document::ScaleMode scaleMode = (Document::ScaleMode)myMenu.actions().indexOf(selectedItem);
    if (m_master->document()->scaleMode() != scaleMode)
    {
      m_master->document()->setScaleMode(scaleMode);
      m_master->document()->emitScaleFormatChanged();
    }
  }

  // Hit the save entry:
  else
  {
    // Save current mode as default:
    QSettings settings;
    settings.setValue("scaleDefaults/mode", (int)m_master->document()->scaleMode());
  }
}

///////////////////////////////// End of File //////////////////////////////////
