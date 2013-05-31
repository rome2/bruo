#include "waveview.h"

WaveView::WaveView(Document* doc, QWidget* parent) :
  QWidget(parent),
  m_document(doc),
  m_showRuler(true),
  m_showScales(true)
{
  QScrollBar
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

void WaveView::closeEvent(QCloseEvent *event)
{
  // Only if we don't have a document anymore then closing is allowed:
  event->setAccepted(m_document == 0);
  if (m_document == 0)
    return;

  // Let the document's manager handle the close:
  if (m_document->manager() != 0)
    event->setAccepted(m_document->manager()->closeDocument(m_document));
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
  QColor waveColor(127, 127, 127);

  // Create painter:
  QPainter painter(this);

  QRect waveArea(0, 0, width(), height());

  // Ruler visible?
  QRect rulerRect(0, 0, 0, 0);
  if (m_showRuler)
  {
    // Set ruler dimensions:
    rulerRect.setCoords(0, 0, width(), 20);

    // Draw the ruler:
    drawRuler(rulerRect, painter);

    // Update wave drawing area:
    waveArea.setTop(rulerRect.bottom() + 1);
  }

  // Scales visible?
  QRect scaleRect(0, 0, 0, 0);
  if (m_showScales)
  {
    // Set scale area dimensions:
    if (rulerRect.height() > 0)
      scaleRect.setCoords(0, rulerRect.bottom() + 1, 30, height());
    else
      scaleRect.setCoords(0, rulerRect.bottom(), 30, height());

    // Draw the scales:
    drawScales(scaleRect, painter);

    // Update wave drawing area:
    waveArea.setLeft(scaleRect.right() + 1);
  }

  // Clear wave background:
  painter.fillRect(waveArea, backColor);

  // Get height of a single channel:
  double channelHeight = (double)waveArea.height() / m_document->channelCount();

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

void WaveView::drawRuler(const QRect& rulerRect, QPainter& painter)
{
  // Clear background:
  painter.fillRect(rulerRect, QColor(192, 192,192));

  // Draw divider:
  painter.setPen(QColor(0, 0, 0));
  painter.drawLine(rulerRect.left(), rulerRect.bottom(), rulerRect.right(), rulerRect.bottom());
}

void WaveView::drawScales(const QRect& scaleRect, QPainter& painter)
{
  // Clear background:
  painter.fillRect(scaleRect, QColor(160, 160,160));

  // Draw divider:
  painter.setPen(QColor(0, 0, 0));
  painter.drawLine(scaleRect.right(), scaleRect.top(), scaleRect.right(), scaleRect.bottom());
}
