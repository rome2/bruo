#include "waveview.h"

WaveView::WaveView(Document* doc, QWidget* parent) :
  QWidget(parent),
  m_document(doc),
  m_drawHalfLine(true),
  m_drawChannelDivider(true),
  m_drawBackGradients(true),
  m_zoomV(1.0),
  m_zoomVOverlap(0.9),
  m_posV(0.5),
  m_viewLength(0),
  m_viewPosition(0),
  m_backColor(255, 255, 255),
  m_centerColor(192, 192, 192),
  m_halfColor(208, 208, 208),
  m_waveColor(103, 103, 103),
  m_upperColor(255, 255, 255),
  m_lowerColor(208, 208, 208),
  m_dividerColor(35, 35, 35),
  m_selectionBackColor(128, 128, 255, 64),
  m_selectionBorderColor(128, 128, 255, 255)
{
  // We should be at least 3 pixels high:
  setMinimumSize(3, 3);

  // Got a document:
  if (m_document == 0)
    return;

  // Attach event handlers to the document:
  connect(m_document, SIGNAL(peaksChanged()),     this, SLOT(peaksChanged()));
  connect(m_document, SIGNAL(selectionChanged()), this, SLOT(update()));
  connect(m_document, SIGNAL(selectionChanging()),this, SLOT(update()));
  connect(m_document, SIGNAL(cursorPosChanged()), this, SLOT(update()));

  // Init view:
  m_viewLength = m_document->sampleCount();
}

WaveView::~WaveView()
{
  // Nothing to do here.
}

Document* WaveView::document()
{
  // Return current document:
  return m_document;
}

const Document* WaveView::document() const
{
  // Return current document:
  return m_document;
}

void WaveView::setDocument(Document* doc)
{
  // Update document:
  m_document = doc;

  // Redraw control:
  update();
}

bool WaveView::drawHalfLine() const
{
  // Return current state:
  return m_drawHalfLine;
}

void WaveView::setDrawHalfLine(bool newState)
{
  // Anything to do?
  if (m_drawHalfLine == newState)
    return;

  // Update value:
  m_drawHalfLine = newState;

  // Redraw control:
  update();
}

bool WaveView::drawChannelDivider() const
{
  // Return current state:
  return m_drawChannelDivider;
}

void WaveView::setDrawChannelDivider(bool newState)
{
  // Anything to do?
  if (m_drawChannelDivider == newState)
    return;

  // Update value:
  m_drawChannelDivider = newState;

  // Redraw control:
  update();
}

bool WaveView::drawBackGradients() const
{
  // Return current state:
  return m_drawBackGradients;
}

void WaveView::setDrawBackGradients(bool newState)
{
  // Anything to do?
  if (m_drawBackGradients == newState)
    return;

  // Update value:
  m_drawBackGradients = newState;

  // Redraw control:
  update();
}

double WaveView::zoomV() const
{
  // Return current zoom:
  return m_zoomV;
}

void WaveView::setZoomV(double newZoom)
{
  // Anything to do?
  if (m_zoomV == newZoom)
    return;

  // Update value:
  m_zoomV = newZoom;

  // Notify listeners:
  emitViewportChanged();

  // Redraw control:
  update();
}

double WaveView::posV() const
{
  // Return current zoom:
  return m_posV;
}

void WaveView::setPosV(double newPos)
{
  // Clip value:
  if (newPos < 0.0)
    newPos = 0.0;
  if (newPos > 1.0)
    newPos = 1.0;

  // Anything to do?
  if (m_posV == newPos)
    return;

  // Set new position:
  m_posV = newPos;

  // Notify listeners:
  emitViewportChanged();

  // Redraw control:
  update();
}

double WaveView::zoomVOverlap() const
{
  // Return current overlap:
  return m_zoomVOverlap;
}

void WaveView::setZoomVOverlap(double newOverlap)
{
  // Anything to do?
  if (m_zoomVOverlap == newOverlap)
    return;

  // Update value:
  m_zoomVOverlap = newOverlap;

  // Notify listeners:
  emitViewportChanged();

  // Redraw control:
  update();
}

qint64 WaveView::viewLength() const
{
  // Return current length:
  return m_viewLength;
}

void WaveView::setViewLength(qint64 newLength)
{
  // Got a document?
  if (m_document == 0)
    return;

  // Clip length:
  if (newLength < 5)
    newLength = 5;
  if (newLength > m_document->sampleCount())
    newLength = m_document->sampleCount();

  // Anything to do?
  if (m_viewLength == newLength)
    return;

  // Set value:
  m_viewLength = newLength;

  // Notify listeners:
  emitViewportChanged();

  // Redraw control:
  update();
}

qint64 WaveView::viewPosition() const
{
  // Return current position:
  return m_viewPosition;
}

void WaveView::setViewPosition(qint64 newPos)
{
  // Got a document?
  if (m_document == 0)
    return;

  // Clip value:
  if (newPos < 0)
    newPos = 0;
  if (newPos > (m_document->sampleCount() - m_viewLength))
    newPos = m_document->sampleCount() - m_viewLength;

  // Anything to do?
  if (m_viewPosition == newPos)
    return;

  // Set value:
  m_viewPosition = newPos;

  // Notify listeners:
  emitViewportChanged();

  // Redraw control:
  update();
}

void WaveView::setViewport(qint64 newPos, qint64 newLength)
{
  // Got a document?
  if (m_document == 0)
    return;

  // Clip position:
  if (newPos < 0)
    newPos = 0;
  else if (newPos >= m_document->sampleCount())
    newPos = m_document->sampleCount() - 1;

  // Clip length:
  if (newLength < 5)
    newLength = 5;
  if ((newPos + newLength) > m_document->sampleCount())
    newLength = m_document->sampleCount() - newPos;

  // Anything to do?
  if (m_viewPosition == newPos && m_viewLength == newLength)
    return;

  // Set value:
  m_viewPosition = newPos;
  m_viewLength = newLength;

  // Notify listeners:
  emitViewportChanged();

  // Redraw control:
  update();
}

const QColor& WaveView::backColor() const
{
  // Return current color:
  return m_backColor;
}

void WaveView::setBackColor(const QColor& newColor)
{
  // Anything to do?
  if (m_backColor == newColor)
    return;

  // Save value:
  m_backColor = newColor;

  // Redraw the client area:
  update();
}

const QColor& WaveView::centerColor() const
{
  // Return current color:
  return m_centerColor;
}

void WaveView::setCenterColor(const QColor& newColor)
{
  // Anything to do?
  if (m_centerColor == newColor)
    return;

  // Save value:
  m_centerColor = newColor;

  // Redraw the client area:
  update();
}

const QColor& WaveView::halfColor() const
{
  // Return current color:
  return m_halfColor;
}

void WaveView::setHalfColor(const QColor& newColor)
{
  // Anything to do?
  if (m_halfColor == newColor)
    return;

  // Save value:
  m_halfColor = newColor;

  // Redraw the client area:
  update();
}

const QColor& WaveView::waveColor() const
{
  // Return current color:
  return m_waveColor;
}

void WaveView::setWaveColor(const QColor& newColor)
{
  // Anything to do?
  if (m_waveColor == newColor)
    return;

  // Save value:
  m_waveColor = newColor;

  // Redraw the client area:
  update();
}

const QColor& WaveView::upperColor() const
{
  // Return current color:
  return m_upperColor;
}

void WaveView::setUpperColor(const QColor& newColor)
{
  // Anything to do?
  if (m_upperColor == newColor)
    return;

  // Save value:
  m_upperColor = newColor;

  // Redraw the client area:
  update();
}

const QColor& WaveView::lowerColor() const
{
  // Return current color:
  return m_lowerColor;
}

void WaveView::setLowerColor(const QColor& newColor)
{
  // Anything to do?
  if (m_lowerColor == newColor)
    return;

  // Save value:
  m_lowerColor = newColor;

  // Redraw the client area:
  update();
}

const QColor& WaveView::dividerColor() const
{
  // Return current color:
  return m_dividerColor;
}

void WaveView::setDividerColor(const QColor& newColor)
{
  // Anything to do?
  if (m_dividerColor == newColor)
    return;

  // Save value:
  m_dividerColor = newColor;

  // Redraw the client area:
  update();
}

const QColor& WaveView::selectionBackColor() const
{
  // Return current color:
  return m_selectionBackColor;
}

void WaveView::setSelectionBackColor(const QColor& newColor)
{
  // Anything to do?
  if (m_selectionBackColor == newColor)
    return;

  // Save value:
  m_selectionBackColor = newColor;

  // Redraw the client area:
  update();
}

const QColor& WaveView::selectionBorderColor() const
{
  // Return current color:
  return m_selectionBorderColor;
}

void WaveView::setSelectionBorderColor(const QColor& newColor)
{
  // Anything to do?
  if (m_selectionBorderColor == newColor)
    return;

  // Save value:
  m_selectionBorderColor = newColor;

  // Redraw the client area:
  update();
}

void WaveView::onViewportChanged()
{
  // Nothing to do here.
}

void WaveView::drawPeaks(QRect& waveRect, QPainter& painter)
{
  // Are we empty?
  if (m_document == 0 || !m_document->peakData().valid())
  {
    // Fill background:
    painter.fillRect(0, 0, width(), height(), m_backColor);

    // Done:
    return;
  }

  // Get height of a single channel:
  double channelHeight = (double)waveRect.height() / m_document->channelCount();

  // Calc y scaling for the peaks:
  double yscale = channelHeight * 0.5 * m_zoomV * m_zoomVOverlap;

  // Clear wave background:
  if (m_drawBackGradients)
  {
    // Create gradient brush:
    QLinearGradient gradient(0.0, 0.0, 0.0, channelHeight);
    gradient.setColorAt(0.0, m_upperColor);
    gradient.setColorAt(1.0, m_lowerColor);
    gradient.setSpread(QGradient::RepeatSpread);

    // Fill background:
    QBrush backBrush(gradient);
    painter.setBrushOrigin(waveRect.topLeft());
    painter.fillRect(waveRect, backBrush);
    painter.setBrushOrigin(0, 0);

    // Area outside the wave visible?
    double factor = (double)m_document->sampleCount() / m_viewLength;
    if (factor < 1.0)
    {
      // Update gradient brush:
      gradient.setColorAt(0.0, QColor((int)(m_upperColor.red() * 0.9), (int)(m_upperColor.green() * 0.9), (int)(m_upperColor.blue() * 0.9)));
      gradient.setColorAt(1.0, QColor((int)(m_lowerColor.red() * 0.9), (int)(m_lowerColor.green() * 0.9), (int)(m_lowerColor.blue() * 0.9)));

      // Adjust drawing rect:
      QRect waveRect2(waveRect);
      waveRect2.setLeft(waveRect2.left() + (factor * waveRect.width()));

      // Fill background:
      QBrush backBrush2(gradient);
      painter.setBrushOrigin(waveRect2.topLeft());
      painter.fillRect(waveRect2, backBrush2);
      painter.setBrushOrigin(0, 0);
    }
  }
  else
  {
    // No gradient, use solid color:
    painter.fillRect(waveRect, m_backColor);
  }

  // Select mip map:
  double factor = (double)m_viewLength / waveRect.width();
  int mip = m_document->peakData().mipmapCount() - 1;
  while (mip >= 0 && factor < m_document->peakData().mipmaps()[mip].divisionFactor())
    mip--;

  // Read direct data if needed:
  SampleBuffer buffer;
  if (mip < 0)
  {
    // Calc number of samples to read:
    qint64 numSamples = m_viewLength;
    if (numSamples > m_document->sampleCount())
      numSamples = m_document->sampleCount();

    // Create buffer and read data:
    if (numSamples > 0)
    {
      buffer.createBuffers(m_document->channelCount(), numSamples);
      m_document->readSamples(m_viewPosition, buffer, numSamples);
    }
  }

  // Draw channels:
  for (int channel = 0; channel < m_document->channelCount(); channel++)
  {
    // Create target rect:
    QRect destRect(waveRect.left(), channel * channelHeight + waveRect.top(), waveRect.width(), channelHeight);
    int ymin = destRect.top();
    int ymax = destRect.bottom();

    // Set clip rect:
    painter.setClipRect(destRect);
    painter.setClipping(true);

    // Get center line position:
    double y = destRect.top() + (channelHeight * 0.5) + (((channelHeight * m_zoomV) - channelHeight) * (m_posV - 0.5));

    // Draw center line:
    if (y >= ymin && y < ymax)
    {
      painter.setPen(m_centerColor);
      painter.drawLine(destRect.left(), y, destRect.right(), y);
    }

    // Draw additional lines?
    if (channelHeight > 8 && m_drawHalfLine)
    {
      // Draw -6dB lines:
      painter.setPen(m_halfColor);
      int y1 = y + channelHeight * m_zoomV * m_zoomVOverlap / 4.0 + 0.5;
      int y2 = y - channelHeight * m_zoomV * m_zoomVOverlap / 4.0 - 0.5;
      painter.drawLine(destRect.left(), y1, destRect.right(), y1);
      painter.drawLine(destRect.left(), y2, destRect.right(), y2);

      // Draw 0dB lines:
      y1 = y + channelHeight * m_zoomV * m_zoomVOverlap / 2.0 + 0.5;
      y2 = y - channelHeight * m_zoomV * m_zoomVOverlap / 2.0 - 0.5;
      painter.drawLine(destRect.left(), y1, destRect.right(), y1);
      painter.drawLine(destRect.left(), y2, destRect.right(), y2);
    }

    // Set color to the wave color:
    painter.setPen(m_waveColor);

    // Use mip maps?
    if (mip >= 0)
    {
      // Calc stepping, start and end position:
      double cnt = (double)m_viewLength / m_document->peakData().mipmaps()[mip].divisionFactor();
      double inc = cnt / destRect.width();
      double pos = (double)m_viewPosition / m_document->peakData().mipmaps()[mip].divisionFactor();
      if (pos < 0)
        pos = 0;
      double maxpos = m_document->peakData().mipmaps()[mip].sampleCount();

      // Draw peaks:
      const PeakSample* samples = m_document->peakData().mipmaps()[mip].samples()[channel];
      for (int x = destRect.left(); x < destRect.right() && pos < maxpos; x++, pos += inc)
      {
        // Find min and max in the current sample range:
        int ipos = (int)floor(pos);
        int ipos2 = (int)floor(pos + inc) ;
        if (ipos2 >= maxpos)
          ipos2 = maxpos - 1;
        double minVal = samples[ipos].minVal;
        double maxVal = samples[ipos].maxVal;
        for (int sub = ipos + 1; sub < ipos2; sub++)
        {
          if (samples[sub].minVal < minVal)
            minVal = samples[sub].minVal;
          if (samples[sub].maxVal > maxVal)
            maxVal = samples[sub].maxVal;
        }

        // Move into window:
        int y1 = (int)(y + (minVal * yscale) - 0.5);
        int y2 = (int)(y + (maxVal * yscale) + 0.5);

        // Draw the actual line:
        painter.drawLine(x, y1, x, y2);
      }
    }

    // No, draw directly:
    else if (buffer.sampleCount() > 0)
    {
      // Calc stepping:
      int numSamples = buffer.sampleCount();
      const double* samples = buffer.sampleBuffer(channel);
      double inc = (double)m_viewLength / destRect.width();

      // Draw peaks as min/max pairs?
      if (inc > 1.0)
      {
        double pos = 0.0;
        double lastMin = samples[0], lastMax = samples[0];
        for (int x = destRect.left(); x < destRect.right() && pos < numSamples; x++, pos += inc)
        {
          // Find min and max in the current sample range:
          int ipos = (int)floor(pos);
          int ipos2 = (int)floor(pos + inc) ;
          if (ipos2 >= numSamples)
            ipos2 = numSamples - 1;
          double minVal = samples[ipos];
          double maxVal = samples[ipos];
          for (int sub = ipos + 1; sub < ipos2; sub++)
          {
            if (samples[sub] < minVal)
              minVal = samples[sub];
            if (samples[sub] > maxVal)
              maxVal = samples[sub];
          }

          // Make sure that we connected to the previous edge:
          if (minVal > lastMax)
            minVal = lastMax;
          if (maxVal < lastMin)
            maxVal = lastMin;

          // Move into window:
          int y1 = (int)(y + (minVal * yscale) + 0.5);
          int y2 = (int)(y + (maxVal * yscale) + 0.5);

          // Draw the actual line:
          painter.drawLine(x, y1, x, y2);

          // Save current state for next round:
          lastMin = minVal;
          lastMax = maxVal;
        }
      }

      // No, we have to paint single samples:
      else
      {
        // Draw peaks:
        int pos = 0;
        inc = 1.0 / inc;
        double oldY = y + (samples[0] * yscale);
        double oldX = destRect.left();
        for (double x = destRect.left(); x < destRect.right() && pos < numSamples; x += inc, pos++)
        {
          // Move into window:
          double y1 = y + (samples[pos] * yscale);

          // Draw samples with handle?
          if (inc > 8)
          {
            // Draw the ball:
            painter.drawRect(x - 2, (int)y1 - 2, 4, 4);

            // Draw the actual line:
            painter.drawLine(x, (int)y1, x, y);
          }

          // No, draw as poly line:
          else
          {
            // Draw line:
            painter.drawLine(QPointF(oldX, oldY), QPointF(x, y1));

            oldX = x;
            oldY = y1;
          }
        }
      }
    }

    // Disable clip rect:
    painter.setClipping(false);

    // Draw channel divider:
    if (m_drawChannelDivider && channel > 0)
    {
      painter.setPen(m_dividerColor);
      painter.drawLine(destRect.left(), destRect.top(), destRect.right(), destRect.top());
    }
  }
}

void WaveView::drawSelection(QRect& waveRect, QPainter& painter)
{
  // Anything to do?
  if (m_document->selectionLength() <= 0)
    return;

  // Draw selection overlay for all at once?
  QRect destRect(waveRect);
  if (m_document->selectedChannel() >= 0)
  {
    // Get height of a single channel:
    double channelHeight = (double)waveRect.height() / m_document->channelCount();

    // Create target rect:
    destRect = QRect(waveRect.left(), m_document->selectedChannel() * channelHeight + waveRect.top(), waveRect.width(), channelHeight);
  }

  // Get selection:
  qint64 start = m_document->selectionStart();
  qint64 end   = start + m_document->selectionLength();
  if (end > m_viewPosition && start < (m_viewPosition + m_viewLength))
  {
    // Get left and right borders and clip to target:
    int x1 = sampleToClient(waveRect, start);
    int x2 = sampleToClient(waveRect, end);
    if (x1 < waveRect.left())
      x1 = waveRect.left();
    if (x2 > waveRect.right())
      x2 = waveRect.right();

    // Fill selection rect:
    painter.fillRect(x1, destRect.top(), x2 - x1 + 1, destRect.height(), m_selectionBackColor);

    // Draw borders if visible:
    painter.setPen(m_selectionBorderColor);
    if (start > m_viewPosition)
      painter.drawLine(x1, destRect.top(), x1, destRect.bottom());
    if (end < (m_viewPosition + m_viewLength))
      painter.drawLine(x2, destRect.top(), x2, destRect.bottom());
  }
}

void WaveView::drawPlayCursor(QRect& waveRect, QPainter& painter)
{
  // Anything to do?
  if (m_document->cursorPosition() < m_viewPosition || m_document->cursorPosition() > (m_viewPosition + m_viewLength))
    return;

  // Draw cursor:
  painter.setPen(QColor(255, 0, 0));
  int x = sampleToClient(waveRect, m_document->cursorPosition());
  painter.drawLine(x, waveRect.top(), x, waveRect.bottom());
}

void WaveView::drawUpdateState(QRect& waveRect, QPainter& painter)
{
  // Anything to do?
  if (!m_document->updatingPeaks())
    return;

  // Draw update state:
  painter.setPen(QColor(255, 255, 255));
  QRect r(waveRect);
  r.moveTo(waveRect.left() + 1, waveRect.top() + 1);
  painter.drawText(r, Qt::AlignHCenter | Qt::AlignVCenter, tr("updating peaks."));
  painter.setPen(QColor(0, 0, 0));
  painter.drawText(waveRect, Qt::AlignHCenter | Qt::AlignVCenter, tr("updating peaks."));
}

qint64 WaveView::clientToSample(const QRect& rc, const int x) const
{
  // Calc position:
  double zoom = (double)m_viewLength / rc.width();
  qint64 s = m_viewPosition + (zoom * ((x + 0.5) - rc.left()));
  if (s < 0)
    s = 0;
  return s;
}

int WaveView::sampleToClient(const QRect& rc, qint64 s) const
{
  // Calc position:
  double zoom = (double)m_viewLength / rc.width();
  return rc.left() + (int)(((double)(s - m_viewPosition) / zoom) + 0.5);
}

void WaveView::emitViewportChanged()
{
  // Notify listeners:
  if (!signalsBlocked())
  {
    onViewportChanged();
    emit viewportChanged();
  }
}

void WaveView::peaksChanged()
{
  // Update viewport:
  emitViewportChanged();
  update();
}
