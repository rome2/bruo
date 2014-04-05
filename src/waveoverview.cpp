#include "waveoverview.h"

WaveOverView::WaveOverView(Document* doc, WaveEditView* slave, QWidget* parent) :
  WaveView(doc, parent),
  m_slave(slave),
  m_slaveViewPort(0, 0, 0, 0),
  m_drawOverlay(true),
  m_overlayColor(128, 128, 128, 64),
  m_overlayBorderColor(0, 0, 0, 255),
  m_dragBorderDist(5),
  m_mouseDownPos(0, 0),
  m_mouseDownViewPos(0),
  m_mouseDownViewLen(0),
  m_dragOverlay(false),
  m_dragLeft(false),
  m_dragRight(false)
{
  // Set options:
  setDrawHalfLine(false);
  setDrawBackGradients(false);
  setDrawChannelDivider(false);
  setZoomVOverlap(1.0);
  setMouseTracking(true);

  // Connect to slave:
  if (m_slave != 0)
    connect(m_slave, SIGNAL(viewportChanged()), this, SLOT(slaveViewportChanged()));

  // Init slave view rect:
  updateViewPort();
}

WaveOverView::~WaveOverView()
{
  // Nothing to do here.
}

bool WaveOverView::drawViewOverlay() const
{
  // Return current state:
  return m_drawOverlay;
}

void WaveOverView::setDrawViewOverlay(bool newState)
{
  // Anything to do?
  if (m_drawOverlay == newState)
    return;

  // Save state:
  m_drawOverlay = newState;

  // Redraw the client area:
  update();
}

const QColor& WaveOverView::overlayColor() const
{
  // Return current color:
  return m_overlayColor;
}

void WaveOverView::setOverlayColor(const QColor& newColor)
{
  // Anything to do?
  if (m_overlayColor == newColor)
    return;

  // Save value:
  m_overlayColor = newColor;

  // Redraw the client area:
  update();
}

const QColor& WaveOverView::overlayBorderColor() const
{
  // Return current color:
  return m_overlayBorderColor;
}

void WaveOverView::setOverlayBorderColor(const QColor& newColor)
{
  // Anything to do?
  if (m_overlayBorderColor == newColor)
    return;

  // Save value:
  m_overlayBorderColor = newColor;

  // Redraw the client area:
  update();
}

void WaveOverView::slaveViewportChanged()
{
  // Update slave view rect:
  updateViewPort();

  // Redraw the client area:
  update();
}

void WaveOverView::paintEvent(QPaintEvent* /* event */)
{
  // Create painter:
  QPainter painter(this);

  // Update wave area:
  QRect waveRect(0, 0, width(), height());
  drawPeaks(waveRect, painter);

  // Got a slave?
  if (m_slave == 0)
    return;

  // Draw overlay:
  if (m_drawOverlay)
  {
    painter.fillRect(m_slaveViewPort, m_overlayColor);
    painter.setPen(m_overlayBorderColor);
    painter.drawRect(m_slaveViewPort);
  }
}

void WaveOverView::resizeEvent(QResizeEvent* /* event */)
{
  // Update slave view rect:
  updateViewPort();
}

void WaveOverView::mouseMoveEvent(QMouseEvent* event)
{
  // Are we dragging the overlay?
  if (m_dragOverlay)
  {
    // Move slave to click position:
    int x = event->x() - m_mouseDownPos.x();
    m_slave->setViewPosition(m_mouseDownViewPos + clientToSample(x));
  }

  // Are we dragging the left border?
  else if (m_dragLeft)
  {
    // Move slave to click position:
    qint64 x = clientToSample(event->x() - m_mouseDownPos.x());
    if (m_mouseDownViewLen - x > 0)
      m_slave->setViewport(m_mouseDownViewPos + x, m_mouseDownViewLen - x);
  }

  // Are we dragging the right border?
  else if (m_dragRight)
  {
    // Move slave to click position:
    int x = event->x() - m_mouseDownPos.x();
    m_slave->setViewLength(m_mouseDownViewLen + clientToSample(x));
  }

  // None of the above, update the cursor:
  else
  {
    // Hit borders?
    bool hitLeft  = abs(event->x() - m_slaveViewPort.left()) < m_dragBorderDist;
    bool hitRight = abs(event->x() - m_slaveViewPort.right()) < m_dragBorderDist;
    bool hitBox   = event->x() >= m_slaveViewPort.left() && event->x() <= m_slaveViewPort.right();
    if (m_slave->viewLength() >= document()->sampleCount())
      hitBox = false;

    // Set cursor:
    bool dragLeft    = hitLeft && !hitRight;
    bool dragRight   = !hitLeft && hitRight;
    bool dragOverlay = (hitLeft && hitRight) || (hitBox && !hitLeft && !hitRight);
    if (dragLeft || dragRight)
      setCursor(Qt::SizeHorCursor);
    else if (dragOverlay)
      setCursor(Qt::OpenHandCursor);
    else
      setCursor(Qt::ArrowCursor);
  }
}

void WaveOverView::mousePressEvent(QMouseEvent* event)
{
  // No overlay, no fun:
  if (!m_drawOverlay)
    return;

  // Already in action?
  if (m_dragLeft || m_dragRight || m_dragOverlay)
    return;

  // We move with middle and left buttons:
  if (event->buttons() != Qt::LeftButton && event->buttons() != Qt::MiddleButton)
    return;

  // Hit borders?
  bool hitLeft  = abs(event->x() - m_slaveViewPort.left())  < m_dragBorderDist;
  bool hitRight = abs(event->x() - m_slaveViewPort.right()) < m_dragBorderDist;
  bool hitBox   = event->x() >= m_slaveViewPort.left() && event->x() <= m_slaveViewPort.right();
  if (m_slave->viewLength() >= document()->sampleCount())
    hitBox = false;

  // Set flags:
  m_dragLeft    = hitLeft && !hitRight;
  m_dragRight   = !hitLeft && hitRight;
  m_dragOverlay = (hitLeft && hitRight) || (hitBox &&!hitLeft && !hitRight);
  if (!m_dragLeft && !m_dragRight && !m_dragOverlay)
    return;

  // Save down position:
  m_mouseDownPos     = event->pos();
  m_mouseDownViewPos = m_slave->viewPosition();
  m_mouseDownViewLen = m_slave->viewLength();

  // Update cursor if needed:
  if (m_dragOverlay)
    setCursor(Qt::ClosedHandCursor);
}

void WaveOverView::mouseReleaseEvent(QMouseEvent* /* event */)
{
  // Update cursor if needed:
  if (m_dragOverlay)
    setCursor(Qt::OpenHandCursor);

  // Reset flags:
  m_dragOverlay = false;
  m_dragLeft    = false;
  m_dragRight   = false;
}

void WaveOverView::mouseDoubleClickEvent(QMouseEvent* event)
{
  // We only want left double clicks:
  if (event->buttons() != Qt::LeftButton)
    return;

  // Move slave to click position:
  m_slave->setViewPosition(clientToSample(event->x()) - (m_slave->viewLength() / 2));
}

void WaveOverView::updateViewPort()
{
  // Got a slave?
  if (m_slave == 0)
    return;

  // Get zoom factor:
  double zoom = (double)width() / document()->sampleCount();

  // Calc length on display:
  double w = m_slave->viewLength();
  if (w > document()->sampleCount())
    w = document()->sampleCount();
  w *= zoom;

  // Calc start position:
  int x = (int)(zoom * m_slave->viewPosition());

  // Update rect:
  m_slaveViewPort.setRect(x, 0, w - 1, height() - 1);
}

qint64 WaveOverView::clientToSample(int x)
{
  // Calc position:
  double zoom = (double)width() / document()->sampleCount();
  return x / zoom;
}
