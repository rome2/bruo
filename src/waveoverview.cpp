#include "waveoverview.h"

WaveOverView::WaveOverView(Document* doc, QWidget* parent) :
  WaveView(doc, parent)
{
  // Set options:
  setDrawHalfLine(false);
  setDrawBackGradients(false);
  setDrawChannelDivider(false);
  setZoomVOverlap(1.0);
}

WaveOverView::~WaveOverView()
{
}

void WaveOverView::paintEvent(QPaintEvent* /* event */)
{
  // Create painter:
  QPainter painter(this);

  // Update wave area:
  QRect waveRect(0, 0, width(), height());
  drawPeaks(waveRect, painter);
}
