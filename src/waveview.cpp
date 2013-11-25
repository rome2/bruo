#include "waveview.h"

WaveView::WaveView(Document* doc, QWidget* parent) :
  QWidget(parent),
  m_document(doc),
  m_showRuler(true),
  m_showScales(true),
  m_scalesDB(false),
  m_drawHalfLine(true),
  m_drawBackGradients(true),
  m_showScrollBars(true),
  m_buttonSize(24),
  m_rulerHeight(24),
  m_scalesWidth(40),
  m_zoomV(1.0),
  m_zoomH(1.0),
  m_posV(0.5),
  m_posH(0.0)
{
  // We should be at least 3 pixels high:
  setMinimumSize(3, 3);

  // Create scroll bars:
  m_scrollH = new QScrollBar(Qt::Horizontal, this);
  connect(m_scrollH, SIGNAL(valueChanged(int)), this ,SLOT(scrollHChanged(int)));
  m_scrollV = new QScrollBar(Qt::Vertical, this);
  connect(m_scrollV, SIGNAL(valueChanged(int)), this, SLOT(scrollVChanged(int)));

  // Create zoom buttons:
  m_btnPlusH = new QPushButton("+", this);
  connect(m_btnPlusH, SIGNAL(clicked()), this, SLOT(btnPlusHPressed()));
  m_btnMinusH = new QPushButton("-", this);
  connect(m_btnMinusH, SIGNAL(clicked()), this, SLOT(btnMinusHPressed()));
  m_btnPlusV = new QPushButton("+", this);
  connect(m_btnPlusV, SIGNAL(clicked()), this, SLOT(btnPlusVPressed()));
  m_btnMinusV = new QPushButton("-", this);
  connect(m_btnMinusV, SIGNAL(clicked()), this, SLOT(btnMinusVPressed()));
  m_btnNull = new QPushButton("0", this);
  connect(m_btnNull, SIGNAL(clicked()), this, SLOT(btnNullPressed()));

  // Init scroll bars:
  updateScrollbars();
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

bool WaveView::showScrollBars() const
{
  return m_showScrollBars;
}

void WaveView::setShowScrollBars(bool newState)
{
  if (m_showScrollBars == newState)
    return;
  m_showScrollBars = newState;
  m_scrollH->setVisible(newState);
  m_scrollV->setVisible(newState);
  m_btnPlusH->setVisible(newState);
  m_btnPlusV->setVisible(newState);
  m_btnMinusH->setVisible(newState);
  m_btnMinusV->setVisible(newState);
  m_btnNull->setVisible(newState);
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
  QColor halfColor(192, 192, 192);
  QColor waveColor(103, 103, 103);
  QColor upperColor(255, 255, 255);
  QColor lowerColor(208, 208, 208);

  // Create painter:
  QPainter painter(this);

  // Update wave drawing area:
  QRect waveArea(0, 0, width(), height());
  if (m_showScrollBars)
  {
    // Remove scroll bar areas:
    waveArea.setBottom(waveArea.bottom() - m_buttonSize);
    waveArea.setRight(waveArea.right() - m_buttonSize);
  }
  if (m_showScales)
    waveArea.setLeft(m_scalesWidth);
  if (m_showRuler)
    waveArea.setTop(m_rulerHeight);

  // Draw ruler and scales:
  drawRuler(waveArea,  painter);
  drawScales(waveArea, painter);

  // Get height of a single channel:
  double channelHeight = (double)waveArea.height() / m_document->channelCount();

  // Clear wave background:
  if (m_drawBackGradients)
  {
    // Create gradient brush:
    QLinearGradient gradient(0.0, 0.0, 0.0, channelHeight);
    gradient.setColorAt(0.0, upperColor);
    gradient.setColorAt(1.0, lowerColor);
    gradient.setSpread(QGradient::RepeatSpread);

    // Fill background:
    QBrush backBrush(gradient);
    painter.setBrushOrigin(waveArea.topLeft());
    painter.fillRect(waveArea, backBrush);
    painter.setBrushOrigin(0, 0);
  }
  else
  {
    // No gradient, use solid color:
    painter.fillRect(waveArea, backColor);
  }

  // Draw channels:
  for (int channel = 0; channel < m_document->channelCount(); channel++)
  {
    // Create target rect:
    QRect destRect(waveArea.left(), channel * channelHeight + waveArea.top(), waveArea.width(), channelHeight);
    int ymin = destRect.top();
    int ymax = destRect.bottom();

    // Get center line position:
    int y = destRect.top() + (channelHeight * 0.5) + (channelHeight * m_zoomV * (m_posV - 0.5));

    // Draw center line:
    if (y >= ymin && y < ymax)
    {
      painter.setPen(centerColor);
      painter.drawLine(destRect.left(), y, destRect.right(), y);
    }

    // Draw -6dB lines:
    if (channelHeight > 8 && m_drawHalfLine)
    {
      painter.setPen(halfColor);
      int y1 = y + channelHeight * m_zoomV / 4;
      int y2 = y - channelHeight * m_zoomV / 4;
      if (y1 >= ymin && y1 < ymax)
        painter.drawLine(destRect.left(), y1, destRect.right(), y1);
      if (y2 >= ymin && y2 < ymax)
        painter.drawLine(destRect.left(), y2, destRect.right(), y2);
    }

    // Draw 0dB lines:
    if (channelHeight > 8 && m_drawHalfLine)
    {
      painter.setPen(halfColor);
      int y1 = y + channelHeight * m_zoomV / 2;
      int y2 = y - channelHeight * m_zoomV / 2;
      if (y1 >= ymin && y1 < ymax)
        painter.drawLine(destRect.left(), y1, destRect.right(), y1);
      if (y2 >= ymin && y2 < ymax)
        painter.drawLine(destRect.left(), y2, destRect.right(), y2);
    }

    // Select mip map:
    int mip = 0;
    const PeakSample* samples = m_document->peakData().mipmaps()[mip].samples()[channel];
    int cnt = m_document->peakData().mipmaps()[mip].sampleCount();

    // Draw peaks:
    painter.setPen(waveColor);
    double inc = (double)cnt / destRect.width();
    double pos = 0.0;
    for (int x = destRect.left(); x < destRect.right(); x++, pos += inc)
    {
      // Move into window:
      int y1 = (int)(y + (samples[(int)pos].minVal * channelHeight * 0.5 * m_zoomV));
      int y2 = (int)(y + (samples[(int)pos].maxVal * channelHeight * 0.5 * m_zoomV));

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
    if (channel > 0)
      painter.drawLine(destRect.left(), destRect.top(), destRect.right(), destRect.top());
  }
}

void WaveView::resizeEvent(QResizeEvent* /* event */)
{
  // Don't do anything without a document:
  if (!m_document)
    return;

  // Move scrollbars:
  m_scrollH->setGeometry(0, height() - m_buttonSize, width() - (3 * m_buttonSize), m_buttonSize);
  m_scrollV->setGeometry(width() - m_buttonSize, 0, m_buttonSize, height() - (3 * m_buttonSize));

  // Move zoom buttons:
  m_btnPlusH->setGeometry(width() - (3 * m_buttonSize), height() - m_buttonSize, m_buttonSize, m_buttonSize);
  m_btnMinusH->setGeometry(width() - (2 * m_buttonSize), height() - m_buttonSize, m_buttonSize, m_buttonSize);
  m_btnPlusV->setGeometry(width() - m_buttonSize, height() - (3 * m_buttonSize), m_buttonSize, m_buttonSize);
  m_btnMinusV->setGeometry(width() - m_buttonSize, height() - (2 * m_buttonSize), m_buttonSize, m_buttonSize);
  m_btnNull->setGeometry(width() - m_buttonSize, height() - m_buttonSize, m_buttonSize, m_buttonSize);

  // Update scroll bars:
  updateScrollbars();
}

void WaveView::btnPlusHPressed()
{
}

void WaveView::btnMinusHPressed()
{
}

void WaveView::btnPlusVPressed()
{
  // Don't zoom too far:
  if (m_zoomV > 100.0)
    return;

  // Zoom in:
  m_zoomV *= 2.0;

  // Update view:
  updateScrollbars();
  update();
}

void WaveView::btnMinusVPressed()
{
  // Zoom out:
  m_zoomV /= 2.0;

  // Clip zoom:
  if (m_zoomV <= 0.25)
    m_zoomV = 0.25;

  // Center view on magnification:
  if (m_zoomV <= 1.0)
    m_posV  = 0.5;

  // Update view:
  updateScrollbars();
  update();
}

void WaveView::btnNullPressed()
{
  // Reset zoom and position:
  m_zoomV = 1.0;
  m_zoomH = 1.0;
  m_posV  = 0.5;
  m_posH  = 0.0;

  // Update view:
  updateScrollbars();
  update();
}

void WaveView::scrollHChanged(int value)
{
  if (m_scrollH->maximum() == 0)
    return;
  m_posH = (double)value / m_scrollH->maximum();
  update();
}

void WaveView::scrollVChanged(int value)
{
  if (m_scrollV->maximum() == 0)
    return;
  m_posV = 1.0 - ((double)value / m_scrollV->maximum());
  update();
}

void WaveView::updateScrollbars()
{
  // Get wave area:
  QRect waveArea(0, 0, width(), height());
  if (m_showScrollBars)
  {
    waveArea.setBottom(waveArea.bottom() - m_buttonSize);
    waveArea.setRight(waveArea.right() - m_buttonSize);
  }
  if (m_showRuler)
    waveArea.setTop(m_rulerHeight);
  if (m_showScales)
    waveArea.setLeft(m_scalesWidth);

  // Disable updates:
  bool oldV = m_scrollV->blockSignals(true);
  bool oldH = m_scrollH->blockSignals(true);

  if (m_zoomV <= 1.0)
  {
    m_scrollV->setMinimum(0);
    m_scrollV->setMaximum(0);
    m_scrollV->setPageStep(0);
  }
  else
  {
    // Get height of a single channel:
    double channelHeight = (double)waveArea.height() / m_document->channelCount();

    // Calc maximum value for the scroll bar:
    double maxVal = (channelHeight * m_zoomV) - channelHeight;

    // Update values:
    m_scrollV->setMaximum((int)maxVal);
    m_scrollV->setPageStep((int)channelHeight);
    m_scrollV->setValue((int)(maxVal * (1.0 - m_posV)));
    m_scrollV->setSingleStep((int)(channelHeight / 5.0));
  }

  if (m_zoomH <= 1.0)
  {
    m_scrollH->setMinimum(0);
    m_scrollH->setMaximum(0);
    m_scrollH->setPageStep(0);
  }
  else
  {
  }

  // Enable updates again:
  m_scrollV->blockSignals(oldV);
  m_scrollH->blockSignals(oldH);
}

void WaveView::drawRuler(QRect& waveRect, QPainter& painter)
{
  // Ruler visible?
  if (!m_showRuler)
    return;

  // Set ruler dimensions:
  QRect rulerRect(0, 0, waveRect.right(), m_rulerHeight);

  // Clear background:
  painter.fillRect(rulerRect, QColor(192, 192,192));

  // Draw divider:
  painter.setPen(QColor(0, 0, 0));
  painter.drawLine(rulerRect.left(), rulerRect.bottom(), rulerRect.right(), rulerRect.bottom());
}

void WaveView::drawScales(QRect& waveRect, QPainter& painter)
{
  // Scales visible?
  if (!m_showScales)
    return;

  // Set scale area dimensions:
  QRect scaleRect(0, waveRect.top(), m_scalesWidth, waveRect.height());

  // Clear background:
  painter.fillRect(scaleRect, QColor(160, 160,160));

  // Draw mode:
  if (m_scalesDB)
    painter.drawText(scaleRect, Qt::AlignLeft | Qt::AlignTop, "dB");
  else
    painter.drawText(scaleRect, Qt::AlignLeft | Qt::AlignTop, "%");

  // Draw divider:
  painter.setPen(QColor(0, 0, 0));
  painter.drawLine(scaleRect.right(), scaleRect.top(), scaleRect.right(), scaleRect.bottom());

  // Get height of a single channel:
  double channelHeight = (double)waveRect.height() / m_document->channelCount();

  // This where the marker lines start:
  int markerX = (int)(0.75 * scaleRect.width());

  // Calc maximum text height:
  int fontHeight = painter.fontMetrics().height();
  int lineHeight = fontHeight * 3;

  // Draw channels:
  for (int channel = 0; channel < m_document->channelCount(); channel++)
  {
    // Create target rect:
    QRect destRect(scaleRect.left(), channel * channelHeight + scaleRect.top(), scaleRect.width(), channelHeight);
    int ymin = destRect.top();
    int ymax = destRect.bottom();

    // Get center line and text position:
    double y = destRect.top() + (channelHeight * 0.5) + (channelHeight * m_zoomV * (m_posV - 0.5));
    QRect centerRect(0, (int)(y - (lineHeight / 2.0)), markerX - 2, lineHeight);

    // Draw center line:
    if (y > ymin && y < ymax)
      painter.drawLine(markerX, y, destRect.right(), y);
    if (centerRect.top() > destRect.top() && centerRect.bottom() < destRect.bottom())
    {
      if (m_scalesDB)
        painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "-oo");
      else
        painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "0");
    }

    // Calc division factor for the optimal display spacing:
    double div = 1.0;
    while ((channelHeight * m_zoomV * div) > lineHeight)
      div *= 0.5;
    double dy = channelHeight * m_zoomV * div;

    // Draw all negative values:
    int line = 0;
    int oldVal = 0;
    while (true)
    {
      // Select line:
      line++;

      // Calc display value:
      int val;
      if (m_scalesDB)
        val = (int)(20.0 * log10(line * div * 2.0));
      else
        val = (int)(line * div * 200.0);
      if (val == oldVal)
        continue;
      oldVal = val;

      // Reproject value to find int position:
      double ty;
      if (m_scalesDB)
        ty = y - (pow(10.0, val / 20.0) * dy / (div * 2.0));
      else
        ty = y - val * dy / (div * 200.0);
      if (ty < ymin)
        break;
      if (ty > ymax)
        continue;

      // Calc text rect:
      QRect textRect(0, (int)(ty - (fontHeight * 0.5)), markerX - 2, fontHeight);
      if (textRect.top() > destRect.top() && textRect.bottom() < destRect.bottom())
      {
        // Draw the scale line:
        painter.drawLine(markerX, ty, destRect.right(), ty);

        // Draw the text:
        painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(val));
      }
    }

    // Draw all positive values:
    line = 0;
    oldVal = 0;
    while (true)
    {
      // Select line:
      line++;

      // Calc display value:
      int val;
      if (m_scalesDB)
        val = (int)(20.0 * log10(line * div * 2.0));
      else
        val = (int)(line * div * 200.0);
      if (val == oldVal)
        continue;
      oldVal = val;

      // Reproject value to find int position:
      double ty;
      if (m_scalesDB)
        ty = y + (pow(10.0, val / 20.0) * dy / (div * 2.0));
      else
        ty = y + val * dy / (div * 200.0);
      if (ty > ymax)
        break;
      if (ty < ymin)
        continue;

      // Calc text rect:
      QRect textRect(0, (int)(ty - (fontHeight * 0.5)), markerX - 2, fontHeight);
      if (textRect.top() > destRect.top() && textRect.bottom() < destRect.bottom())
      {
        // Draw the scale line:
        painter.drawLine(markerX, ty, destRect.right(), ty);

        // Draw the text:
        painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(val));
      }
    }

    // Draw channel divider:
    if (channel > 0)
      painter.drawLine(0, destRect.top(), destRect.right(), destRect.top());
  }
}
