#include "waveeditview.h"

WaveEditView::WaveEditView(Document* doc, QWidget* parent) :
  WaveView(doc, parent),
  m_showRuler(true),
  m_showScales(true),
  m_scalesDB(false),
  m_showScrollBars(true),
  m_buttonSize(24),
  m_rulerHeight(24),
  m_scalesWidth(40)
{
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

WaveEditView::~WaveEditView()
{
}

bool WaveEditView::showRuler() const
{
  return m_showRuler;
}

void WaveEditView::setShowRuler(bool newState)
{
  if (m_showRuler == newState)
    return;
  m_showRuler = newState;
  update();
}

bool WaveEditView::showScales() const
{
  return m_showScales;
}

void WaveEditView::setShowScales(bool newState)
{
  if (m_showScales == newState)
    return;
  m_showScales = newState;
  update();
}

bool WaveEditView::showScrollBars() const
{
  return m_showScrollBars;
}

void WaveEditView::setShowScrollBars(bool newState)
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

void WaveEditView::paintEvent(QPaintEvent* /* event */)
{
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

  // Draw ruler, scales and peak:
  drawRuler(waveArea,  painter);
  drawScales(waveArea, painter);
  drawPeaks(waveArea, painter);
}

void WaveEditView::resizeEvent(QResizeEvent* /* event */)
{
  // Don't do anything without a document:
  if (!document())
    return;

  // Disable updates:
  bool oldV = m_scrollV->blockSignals(true);
  bool oldH = m_scrollH->blockSignals(true);

  // Move scrollbars:
  m_scrollH->setGeometry(0, height() - m_buttonSize, width() - (3 * m_buttonSize), m_buttonSize);
  m_scrollV->setGeometry(width() - m_buttonSize, 0, m_buttonSize, height() - (3 * m_buttonSize));

  // Enable updates again:
  m_scrollV->blockSignals(oldV);
  m_scrollH->blockSignals(oldH);

  // Move zoom buttons:
  m_btnPlusH->setGeometry(width() - (3 * m_buttonSize), height() - m_buttonSize, m_buttonSize, m_buttonSize);
  m_btnMinusH->setGeometry(width() - (2 * m_buttonSize), height() - m_buttonSize, m_buttonSize, m_buttonSize);
  m_btnPlusV->setGeometry(width() - m_buttonSize, height() - (3 * m_buttonSize), m_buttonSize, m_buttonSize);
  m_btnMinusV->setGeometry(width() - m_buttonSize, height() - (2 * m_buttonSize), m_buttonSize, m_buttonSize);
  m_btnNull->setGeometry(width() - m_buttonSize, height() - m_buttonSize, m_buttonSize, m_buttonSize);
}

void WaveEditView::btnPlusHPressed()
{
  // Don't zoom too far:
  if (zoomH() > 100.0)
    return;

  // Zoom in:
  setZoomH(zoomH() * 2.0);

  // Update view:
  updateScrollbars();
  update();
}

void WaveEditView::btnMinusHPressed()
{
  // Zoom out:
  setZoomH(zoomH() / 2.0);

  // Clip zoom:
  if (zoomH() <= 0.25)
    setZoomH(0.25);

  // Center view on magnification:
  if (zoomH() <= 1.0)
    setPosH(0.0);

  // Update view:
  updateScrollbars();
  update();
}

void WaveEditView::btnPlusVPressed()
{
  // Don't zoom too far:
  if (zoomV() > 100.0)
    return;

  // Zoom in:
  setZoomV(zoomV() * 2.0);

  // Update view:
  updateScrollbars();
  update();
}

void WaveEditView::btnMinusVPressed()
{
  // Zoom out:
  setZoomV(zoomV() / 2.0);

  // Clip zoom:
  if (zoomV() <= 0.25)
    setZoomV(0.25);

  // Center view on magnification:
  if (zoomV() <= 1.0)
    setPosV(0.5);

  // Update view:
  updateScrollbars();
  update();
}

void WaveEditView::btnNullPressed()
{
  // Reset zoom and position:
  setZoomV(1.0);
  setZoomH(1.0);
  setPosV(0.5);
  setPosH(0.0);

  // Update view:
  updateScrollbars();
  update();
}

void WaveEditView::scrollHChanged(int value)
{
  if (m_scrollH->maximum() == 0)
    return;
  setPosH((double)value / (m_scrollH->maximum()));
  update();
}

void WaveEditView::scrollVChanged(int value)
{
  if (m_scrollV->maximum() == 0)
    return;
  setPosV(1.0 - ((double)value / m_scrollV->maximum()));
  update();
}

void WaveEditView::updateScrollbars()
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

  // Zoomed out fully?
  if (zoomV() <= 1.0)
  {
    // Disable scrollbar:
    m_scrollV->setMinimum(0);
    m_scrollV->setMaximum(0);
    m_scrollV->setPageStep(0);
  }
  else
  {
    // This value is arbitrary as the zoom and position values are independent
    // from the actual pixel size of the view. This value is just high enough to
    // provide a smooth stepping in the scroll bar.
    double normalHeight = 1000;

    // Calc maximum value for the scroll bar:
    double maxVal = (normalHeight * zoomV()) - normalHeight;

    // Update values:
    m_scrollV->setMaximum((int)maxVal);
    m_scrollV->setPageStep((int)(normalHeight));
    m_scrollV->setValue((int)(maxVal * (1.0 - posV())));
    m_scrollV->setSingleStep((int)(normalHeight / 5.0));
  }

  // Zoomed out fully?
  if (zoomH() <= 1.0)
  {
    // Disable scrollbar:
    m_scrollH->setMinimum(0);
    m_scrollH->setMaximum(0);
    m_scrollH->setPageStep(0);
  }
  else
  {
    // This value is arbitrary as the zoom and position values are independent
    // from the actual pixel size of the view. This value is just high enough to
    // provide a smooth stepping in the scroll bar.
    double normalWidth = 10000;

    // Calc maximum value for the scroll bar:
    double maxVal = (normalWidth * zoomH()) - normalWidth;

    // Update values:
    m_scrollH->setMaximum((int)maxVal);
    m_scrollH->setPageStep((int)(normalWidth));
    m_scrollH->setValue((int)(maxVal * posH()));
    m_scrollH->setSingleStep((int)(normalWidth / 5.0));
  }

  // Enable updates again:
  m_scrollV->blockSignals(oldV);
  m_scrollH->blockSignals(oldH);
}

void WaveEditView::drawRuler(QRect& waveRect, QPainter& painter)
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

  // Got a document?
  if (!document() || !document()->peakData().valid())
    return;
}

void WaveEditView::drawScales(QRect& waveRect, QPainter& painter)
{
  // Scales visible?
  if (!m_showScales)
    return;

  // Set scale area dimensions:
  QRect scaleRect(0, waveRect.top(), m_scalesWidth, waveRect.height());

  // Clear background:
  painter.fillRect(scaleRect, QColor(160, 160,160));

  // Draw divider:
  painter.setPen(QColor(0, 0, 0));
  painter.drawLine(scaleRect.right(), scaleRect.top(), scaleRect.right(), scaleRect.bottom());

  // Got a document?
  if (!document() || !document()->peakData().valid())
    return;

  // Get height of a single channel:
  double channelHeight = (double)waveRect.height() / document()->channelCount();

  // This where the marker lines start:
  int markerX = (int)(0.75 * scaleRect.width());

  // Calc maximum text height:
  int fontHeight = painter.fontMetrics().height();
  int lineHeight = fontHeight * 3;

  // Draw channels:
  for (int channel = 0; channel < document()->channelCount(); channel++)
  {
    // Create target rect:
    QRect destRect(scaleRect.left(), channel * channelHeight + scaleRect.top(), scaleRect.width(), channelHeight);
    int ymin = destRect.top();
    int ymax = destRect.bottom();

    // Get center line:
    double y = destRect.top() + (channelHeight * 0.5) + (((channelHeight * zoomV()) - channelHeight) * (posV() - 0.5));

    // Draw center line:
    if (y > ymin && y < ymax)
      painter.drawLine(markerX, y, destRect.right(), y);

    // Draw center text:
    QRect centerRect(0, (int)(y - (lineHeight / 2.0)), markerX - 2, lineHeight);
    if (centerRect.top() > destRect.top() && centerRect.bottom() < destRect.bottom())
    {
      if (m_scalesDB)
        painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "-oo");
      else
        painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "0");
    }

    // Calc division factor for the optimal display spacing:
    double div = 1.0;
    while ((channelHeight * zoomV() * div) > lineHeight)
      div *= 0.5;
    double dy = channelHeight * zoomV() * div * zoomVOverlap();

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
        if (m_scalesDB && val > 0)
          painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString("+") + QString::number(val));
        else
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
        ty = y + (pow(10.0, (int)val / 20.0) * dy / (div * 2.0));
      else
        ty = y + (int)val * dy / (div * 200.0);
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
        if (m_scalesDB && val > 0)
          painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString("+") + QString::number(val));
        else
          painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(val));
      }
    }

    // Draw channel divider:
    if (drawChannelDivider() && channel > 0)
      painter.drawLine(0, destRect.top(), destRect.right(), destRect.top());
  }
}
