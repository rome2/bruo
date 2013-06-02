#include "waveview.h"

WaveView::WaveView(Document* doc, QWidget* parent) :
  QWidget(parent),
  m_document(doc),
  m_showRuler(true),
  m_showScales(true),
  m_drawHalfLine(true),
  m_drawBackGradients(true)
{
  setMinimumSize(3, 3);
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

bool WaveView::showRuler() const
{
  return m_showRuler;
}

void WaveView::setShowRuler(bool newState)
{
  if (m_showRuler == newState)
    return;
  m_showRuler = newState;
  update();
}

bool WaveView::showScales() const
{
  return m_showScales;
}

void WaveView::setShowScales(bool newState)
{
  if (m_showScales == newState)
    return;
  m_showScales = newState;
  update();
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

void WaveView::paintEvent(QPaintEvent* /* event */)
{
  // Are we empty?
  if (!m_document || !m_document->peakData().valid())
  {
    // Fill red:
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), Qt::white);

    // Done:
    return;
  }

  QColor backColor(255, 255, 255);
  QColor centerColor(192, 192, 192);
  QColor halfColor(220, 220, 220);
  QColor waveColor(103, 103, 103);
  QColor upperColor(255, 255, 255);
  QColor lowerColor(192, 192, 192);

  // Create painter:
  QPainter painter(this);

  QRect waveArea(0, 0, width(), height());

  // Draw ruler and scales:
  drawRuler(waveArea, painter);
  drawScales(waveArea, painter);

  // Get height of a single channel:
  double channelHeight = (double)waveArea.height() / m_document->channelCount();

  // Clear wave background:
  if (m_drawBackGradients)
  {
    QLinearGradient gradient(0.0, 0.0, 0.0, channelHeight);
    gradient.setColorAt(0.0, upperColor);
    gradient.setColorAt(1.0, lowerColor);
    gradient.setSpread(QGradient::RepeatSpread);
    QBrush backBrush(gradient);
    painter.setBrushOrigin(waveArea.topLeft());
    painter.fillRect(waveArea, backBrush);
    painter.setBrushOrigin(0, 0);
  }
  else
    painter.fillRect(waveArea, backColor);

  // Draw channels:
  for (int channel = 0; channel < m_document->channelCount(); channel++)
  {
    const PeakSample* samples = m_document->peakData().mipmaps()[1].samples()[channel];
    int cnt = m_document->peakData().mipmaps()[1].sampleCount();

    // Create target rect:
    QRect destRect(waveArea.left(), channel * channelHeight + waveArea.top(), width(), (channel + 1) * channelHeight + waveArea.top());

    // Draw center line:
    painter.setPen(centerColor);
    int y = destRect.top() + channelHeight / 2;
    painter.drawLine(destRect.left(), y, destRect.right(), y);

    // Draw half line:
    if (channelHeight > 8 && m_drawHalfLine)
    {
      painter.setPen(halfColor);
      int y1 = y + channelHeight / 4;
      int y2 = y - channelHeight / 4;
      painter.drawLine(destRect.left(), y1, destRect.right(), y1);
      painter.drawLine(destRect.left(), y2, destRect.right(), y2);
    }

    // Draw peaks:
    painter.setPen(waveColor);
    for (int i = 0; i < cnt && i < destRect.width(); i++)
    {
      // Convert span to -1..1:
      double minVal = (double)samples[i].minVal / 32768.0;
      double maxVal = (double)samples[i].maxVal / 32768.0;

      // Move into window (windows is top down so reverse range):
      double ymax = (0.5 * channelHeight) + (minVal * channelHeight * 0.5);
      double ymin = (0.5 * channelHeight) + (maxVal * channelHeight * 0.5);

      // Draw the value:
      int x = i + destRect.left();
      painter.drawLine(x, (int)ymax + destRect.top(), x, (int)ymin + destRect.top());
    }
  }
}

void WaveView::drawRuler(QRect& waveRect, QPainter& painter)
{
  // Ruler visible?
  if (!m_showRuler)
    return;

  // Set ruler dimensions:
  QRect rulerRect(0, 0, width(), 20);

  // Clear background:
  painter.fillRect(rulerRect, QColor(192, 192,192));

  // Draw divider:
  painter.setPen(QColor(0, 0, 0));
  painter.drawLine(rulerRect.left(), rulerRect.bottom(), rulerRect.right(), rulerRect.bottom());

  // Update wave drawing area:
  waveRect.setTop(rulerRect.bottom() + 1);
}

void WaveView::drawScales(QRect& waveRect, QPainter& painter)
{
  // Scales visible?
  if (!m_showScales)
    return;

  // Set scale area dimensions:
  QRect scaleRect(0, waveRect.top(), 30, height());

  // Clear background:
  painter.fillRect(scaleRect, QColor(160, 160,160));

  // Draw divider:
  painter.setPen(QColor(0, 0, 0));
  painter.drawLine(scaleRect.right(), scaleRect.top(), scaleRect.right(), scaleRect.bottom());

  // Update wave drawing area:
  waveRect.setLeft(scaleRect.right() + 1);
}
