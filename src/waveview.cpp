#include "waveview.h"

WaveView::WaveView(Document* doc, QWidget* parent) :
  QWidget(parent),
  m_document(doc),
  m_drawHalfLine(true),
  m_drawChannelDivider(true),
  m_drawBackGradients(true),
  m_zoomV(1.0),
  m_zoomVOverlap(0.9),
  m_zoomH(1.0),
  m_posV(0.5),
  m_posH(0.0),
  m_backColor(255, 255, 255),
  m_centerColor(192, 192, 192),
  m_halfColor(208, 208, 208),
  m_waveColor(103, 103, 103),
  m_upperColor(255, 255, 255),
  m_lowerColor(208, 208, 208),
  m_dividerColor(35, 35, 35)
{
  // We should be at least 3 pixels high:
  setMinimumSize(3, 3);

  // Attach event handlers to the document:
  connect(doc, SIGNAL(peaksChanged()), this, SLOT(peaksChanged()));
}

WaveView::~WaveView()
{
}

Document* WaveView::document()
{
  return m_document;
}

const Document* WaveView::document() const
{
  return m_document;
}

void WaveView::setDocument(Document* doc)
{
  m_document = doc;
}

bool WaveView::drawHalfLine() const
{
  return m_drawHalfLine;
}

void WaveView::setDrawHalfLine(bool newState)
{
  if (m_drawHalfLine == newState)
    return;
  m_drawHalfLine = newState;
  update();
}

bool WaveView::drawChannelDivider() const
{
  return m_drawChannelDivider;
}

void WaveView::setDrawChannelDivider(bool newState)
{
  if (m_drawChannelDivider == newState)
    return;
  m_drawChannelDivider = newState;
  update();
}

bool WaveView::drawBackGradients() const
{
  return m_drawBackGradients;
}

void WaveView::setDrawBackGradients(bool newState)
{
  if (m_drawBackGradients == newState)
    return;
  m_drawBackGradients = newState;
  update();
}

double WaveView::zoomV() const
{
  return m_zoomV;
}

void WaveView::setZoomV(double newZoom)
{
  if (m_zoomV == newZoom)
    return;
  m_zoomV = newZoom;
  update();
}

double WaveView::zoomH() const
{
  return m_zoomH;
}

void WaveView::setZoomH(double newZoom)
{
  if (m_zoomH == newZoom)
    return;
  m_zoomH = newZoom;
  update();
}

double WaveView::posV() const
{
  return m_posV;
}

void WaveView::setPosV(double newPos)
{
  if (m_posV == newPos)
    return;
  m_posV = newPos;
  update();
}

double WaveView::posH() const
{
  return m_posH;
}

void WaveView::setPosH(double newPos)
{
  if (m_posH == newPos)
    return;
  m_posH = newPos;
  update();
}

double WaveView::zoomVOverlap() const
{
  return m_zoomVOverlap;
}

void WaveView::setZoomVOverlap(double newOverlap)
{
  if (m_zoomVOverlap == newOverlap)
    return;
  m_zoomVOverlap = newOverlap;
  update();
}

void WaveView::drawPeaks(QRect& waveRect, QPainter& painter)
{
  // Are we empty?
  if (!document() || !document()->peakData().valid())
  {
    // Fill red:
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
  }
  else
  {
    // No gradient, use solid color:
    painter.fillRect(waveRect, m_backColor);
  }

  // Draw channels:
  for (int channel = 0; channel < m_document->channelCount(); channel++)
  {
    // Create target rect:
    QRect destRect(waveRect.left(), channel * channelHeight + waveRect.top(), waveRect.width(), channelHeight);
    int ymin = destRect.top();
    int ymax = destRect.bottom();

    // Get center line position:
    double y = destRect.top() + (channelHeight * 0.5) + (((channelHeight * m_zoomV) - channelHeight) * (m_posV - 0.5));

    // Draw center line:
    if (y >= ymin && y < ymax)
    {
      painter.setPen(m_centerColor);
      painter.drawLine(destRect.left(), y, destRect.right(), y);
    }

    if (channelHeight > 8 && m_drawHalfLine)
    {
      // Draw -6dB lines:
      painter.setPen(m_halfColor);
      int y1 = y + channelHeight * m_zoomV * m_zoomVOverlap / 4;
      int y2 = y - channelHeight * m_zoomV * m_zoomVOverlap / 4;
      if (y1 >= ymin && y1 < ymax)
        painter.drawLine(destRect.left(), y1, destRect.right(), y1);
      if (y2 >= ymin && y2 < ymax)
        painter.drawLine(destRect.left(), y2, destRect.right(), y2);

      // Draw 0dB lines:
      y1 = y + channelHeight * m_zoomV * m_zoomVOverlap / 2;
      y2 = y - channelHeight * m_zoomV * m_zoomVOverlap / 2;
      if (y1 >= ymin && y1 < ymax)
        painter.drawLine(destRect.left(), y1, destRect.right(), y1);
      if (y2 >= ymin && y2 < ymax)
        painter.drawLine(destRect.left(), y2, destRect.right(), y2);
    }

    // Select mip map:
    int mip = 0;
    const PeakSample* samples = m_document->peakData().mipmaps()[mip].samples()[channel];
    int cnt = m_document->peakData().mipmaps()[mip].sampleCount();

    // Calc stepping and start position:
    double inc = (double)cnt / (m_zoomH * destRect.width());
    double pos = (1.0 - (1.0 / m_zoomH)) * m_posH * cnt;
    if (pos < 0)
      pos = 0;

    // Draw peaks:
    painter.setPen(m_waveColor);
    for (int x = destRect.left(); x < destRect.right() && pos < cnt; x++, pos += inc)
    {
      // Move into window:
      int y1 = (int)(y + (samples[(int)pos].minVal * yscale));
      int y2 = (int)(y + (samples[(int)pos].maxVal * yscale));

      // Visible at all?
      if (y2 < ymin || y1 > ymax)
        continue;

      // Clip the values:
      if (y1 < ymin)
        y1 = ymin;
      if (y2 > ymax)
        y2 = ymax;

      // Draw the actual line:
      painter.drawLine(x, y1, x, y2);
    }

    // Draw channel divider:
    if (m_drawChannelDivider && channel > 0)
    {
      painter.setPen(m_dividerColor);
      painter.drawLine(destRect.left(), destRect.top(), destRect.right(), destRect.top());
    }
  }

  // Draw update state:
  if (m_document->updatingPeaks())
  {
    painter.setPen(QColor(0, 0, 0));
    painter.drawText(waveRect, Qt::AlignHCenter | Qt::AlignVCenter, tr("updating peaks."));
  }
}

void WaveView::peaksChanged()
{
  // Just redraw:
  update();
}
