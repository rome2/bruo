#include "waveeditview.h"
#include "commands/selectingcommand.h"
#include "commands/clearselectioncommand.h"

WaveEditView::WaveEditView(Document* doc, QWidget* parent) :
  WaveView(doc, parent),
  m_showRuler(true),
  m_showScales(true),
  m_scalesDB(false),
  m_showScrollBars(true),
  m_buttonSize(24),
  m_rulerHeight(30),
  m_scalesWidth(60),
  m_rulerBackColor(192, 192,192),
  m_cornerColor(0, 0, 0),
  m_cornerBackColor(176, 176, 176),
  m_scalesBackColor(160, 160,160),
  m_dragBorderDist(5),
  m_scrollOverlap(10),
  m_scrollbarsLocked(false),
  m_draggingMode(None),
  m_extendingSelection(false),
  m_dragStarted(false)
{
  // We want the keyboard focus:
  setFocusPolicy(Qt::StrongFocus);

  // Enable all mouse move events:
  setMouseTracking(true);

  // Create scroll bars:
  m_scrollH = new QScrollBar(Qt::Horizontal, this);
  m_scrollH->setCursor(Qt::ArrowCursor);
  connect(m_scrollH, SIGNAL(valueChanged(int)), this ,SLOT(scrollHChanged(int)));
  m_scrollV = new QScrollBar(Qt::Vertical, this);
  m_scrollV->setCursor(Qt::ArrowCursor);
  connect(m_scrollV, SIGNAL(valueChanged(int)), this, SLOT(scrollVChanged(int)));

  // Create zoom buttons:
  m_btnPlusH = new QPushButton("+", this);
  m_btnPlusH->setWindowFlags(m_btnPlusH->windowFlags() & ~(Qt::WindowDoesNotAcceptFocus));
  m_btnPlusH->setFocusPolicy(Qt::NoFocus);
  m_btnPlusH->setCursor(Qt::ArrowCursor);
  connect(m_btnPlusH, SIGNAL(clicked()), this, SLOT(btnPlusHPressed()));
  m_btnMinusH = new QPushButton("-", this);
  m_btnMinusH->setWindowFlags(m_btnPlusH->windowFlags() & ~(Qt::WindowDoesNotAcceptFocus));
  m_btnMinusH->setFocusPolicy(Qt::NoFocus);
  m_btnMinusH->setCursor(Qt::ArrowCursor);
  connect(m_btnMinusH, SIGNAL(clicked()), this, SLOT(btnMinusHPressed()));
  m_btnPlusV = new QPushButton("+", this);
  m_btnPlusV->setWindowFlags(m_btnPlusH->windowFlags() & ~(Qt::WindowDoesNotAcceptFocus));
  m_btnPlusV->setFocusPolicy(Qt::NoFocus);
  m_btnPlusV->setCursor(Qt::ArrowCursor);
  connect(m_btnPlusV, SIGNAL(clicked()), this, SLOT(btnPlusVPressed()));
  m_btnMinusV = new QPushButton("-", this);
  m_btnMinusV->setWindowFlags(m_btnPlusH->windowFlags() & ~(Qt::WindowDoesNotAcceptFocus));
  m_btnMinusV->setFocusPolicy(Qt::NoFocus);
  m_btnMinusV->setCursor(Qt::ArrowCursor);
  connect(m_btnMinusV, SIGNAL(clicked()), this, SLOT(btnMinusVPressed()));
  m_btnNull = new QPushButton("0", this);
  m_btnNull->setWindowFlags(m_btnPlusH->windowFlags() & ~(Qt::WindowDoesNotAcceptFocus));
  m_btnNull->setFocusPolicy(Qt::NoFocus);
  m_btnNull->setCursor(Qt::ArrowCursor);
  connect(m_btnNull, SIGNAL(clicked()), this, SLOT(btnNullPressed()));

  // Create timer for scrolling:
  m_dragTimer = new QTimer(this);
  m_dragTimer->setInterval(100);
  connect(m_dragTimer, SIGNAL(timeout()), this, SLOT(dragTimerTick()));

  // Init scroll bars and wave area:
  updateScrollbars();
  updateWaveRect();
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
  updateWaveRect();
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
  updateWaveRect();
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
  updateWaveRect();
  update();
}

void WaveEditView::paintEvent(QPaintEvent* /* event */)
{
  // Create painter:
  QPainter painter(this);

  // Draw ruler, scales, peaks and corner:
  drawRuler(painter);
  drawScales(painter);
  drawPeaks(m_waveArea, painter);
  drawCorner(painter);
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

  // Update drawing area:
  updateWaveRect();
}

void WaveEditView::wheelEvent(QWheelEvent* event)
{
  // No scrolling while we are doing something else:
  if (event->buttons() != Qt::NoButton)
    return;

  // Horizontal scrolling?
  if (event->orientation() == Qt::Horizontal)
  {
    if (event->delta() < 0)
      setViewPosition(viewPosition() + m_scrollH->singleStep());
    else
      setViewPosition(viewPosition() - m_scrollH->singleStep());
  }

  // Zooming?
  else if (event->modifiers() & Qt::ControlModifier)
  {
    // Vertical zooming?
    if (event->modifiers() & Qt::ShiftModifier)
    {
      if (event->delta() > 0)
        btnPlusVPressed();
      else
        btnMinusVPressed();
    }
    else
    {
      // Get factor:
      double fac = 1.25;
      if (event->delta() > 0)
        fac = 1 / fac;

      // Get sample under mouse:
      qint64 center = clientToSample(event->x());

      // Zoom:
      zoomAt(center, fac);
    }
  }

  // Scrolling:
  else
  {
    // Horizontal scrolling?
    if (event->modifiers() & Qt::ShiftModifier)
    {
      if (event->delta() < 0)
        setViewPosition(viewPosition() + m_scrollH->singleStep());
      else
        setViewPosition(viewPosition() - m_scrollH->singleStep());
    }

    // Vertical scrolling:
    else
    {
      if (m_scrollV->maximum() == 0)
        return;

      if (event->delta() > 0)
        setPosV(posV() + 0.03);
      else
        setPosV(posV() - 0.03);
    }
  }
}

void WaveEditView::mouseMoveEvent(QMouseEvent* event)
{
  // Dragging anything?
  if (m_draggingMode == None)
  {
    // Update the cursor:
    updateCursor(event->pos());
    return;
  }

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
  int dx = event->pos().x() - m_lastMousePos.x();
  int dy = event->pos().y() - m_lastMousePos.y();

  // Moving the content horizontally with the middle button?
  if (m_draggingMode == ContentHorizontal || m_draggingMode == ContentBoth)
  {
    if (dx != 0)
    {
      // Calc movement in sample space:
      dx = clientToSample(m_lastMousePos.x()) - clientToSample(event->pos().x());

      // Update position:
      setViewPosition(viewPosition() + dx);
    }
  }

  // Moving the content vertically with the middle button?
  if (m_draggingMode == ContentVertical || m_draggingMode == ContentBoth)
  {
    if (dy != 0 && zoomV() > 1.0)
    {
      // Calc visible height of a single channel:
      double channelHeight = (double)m_waveArea.height() / document()->channelCount();

      // Calc relative movement in the zoomed wave:
      double dz = 1.0 / (channelHeight * zoomV() - channelHeight);

      // Update position:
      setPosV(posV() + dz * dy);
    }
  }

  // Are we dragging the ruler down with the left button (zooming)?
  if (m_draggingMode == ZoomRuler && dy != 0)
  {
    // Zoom at dragging position:
    //zoomAt(m_mouseDownSample, 1.0 - (0.01 * dy));
    zoomAt(clientToSample(m_mouseDownPos.x()), 1.0 - (0.01 * dy));
  }

  if (m_draggingMode == Selecting)
  {
    // Get selection samples:
    qint64 s1 = clientToSample(event->x());
    qint64 s2 = m_mouseDownSample;
    if (s1 > s2)
      std::swap(s1, s2);

    // Get selection channel:
    int channel = -1;
    if (m_extendingSelection)
      channel = document()->selectedChannel();
    else if (event->modifiers() & Qt::ShiftModifier)
      channel = clientToChannel(event->y());

    // Update selection:
    document()->undoStack()->push(new SelectingCommand(document(), s1, s2 - s1, channel));

    // Update scroll timer if needed:
    if (event->x() < (m_waveArea.left() + m_scrollOverlap) && !m_dragTimer->isActive())
      m_dragTimer->start();
    else if (event->x() > (m_waveArea.right() - m_scrollOverlap) && !m_dragTimer->isActive())
      m_dragTimer->start();
    else if (event->x() >= (m_waveArea.left() + m_scrollOverlap) && event->x() <= (m_waveArea.right() - m_scrollOverlap) && m_dragTimer->isActive())
      m_dragTimer->stop();
  }

  // Save position for next call:
  m_lastMousePos = event->pos();
}

void WaveEditView::mousePressEvent(QMouseEvent* event)
{
  // Already in action?
  if (m_draggingMode != None)
    return;

  // Set keyboard focus:
  setFocus();

  // Save positions:
  m_lastMousePos = event->pos();
  m_mouseDownPos = event->pos();
  m_mouseDownSample = clientToSample(event->x());
  m_dragStarted = false;

  // Let's see where we've been hit:
  if (event->x() < m_waveArea.left() && event->y() >= m_waveArea.top())
  {
    // Hit the scale...

    if (event->buttons() == Qt::MiddleButton)
    {
      m_draggingMode = ContentVertical;
      setCursor(Qt::ClosedHandCursor);
    }
  }

  else if (event->x() >= m_waveArea.left() && event->y() < m_waveArea.top())
  {
    // Hit the ruler...
    if (event->buttons() == Qt::LeftButton)
    {
      m_draggingMode = ZoomRuler;
    }
    else if (event->buttons() == Qt::MiddleButton)
    {
      m_draggingMode = ContentHorizontal;
      setCursor(Qt::ClosedHandCursor);
    }
  }

  else if (event->x() < m_waveArea.left() && event->y() < m_waveArea.top())
  {
  }

  // Hit the wave area:
  else
  {
    if (event->buttons() == Qt::MiddleButton)
    {
      m_draggingMode = ContentBoth;
      setCursor(Qt::ClosedHandCursor);
    }
    else if (event->buttons() == Qt::LeftButton)
    {
      // Set selection mode:
      m_draggingMode = Selecting;
      m_extendingSelection = false;

      // Get selection samples:
      qint64 start = document()->selectionStart();
      qint64 end   = start + document()->selectionLength();
      if (end > viewPosition() && start < (viewPosition() + viewLength()))
      {
        // Get selection borders:
        int x1 = sampleToClient(start);
        int x2 = sampleToClient(end);
        int channel = clientToChannel(event->y());
        if (document()->selectedChannel() == -1 || document()->selectedChannel() == channel)
        {
          // Hit the border?
          if (abs(event->x() - x1) < m_dragBorderDist)
          {
            m_mouseDownSample = end;
            m_extendingSelection = true;
          }
          else if (abs(event->x() - x2) < m_dragBorderDist)
          {
            m_mouseDownSample = start;
            m_extendingSelection = true;
          }
        }
      }
    }
  }
}

void WaveEditView::mouseReleaseEvent(QMouseEvent* event)
{
  // Finish selection:
  if (m_draggingMode == Selecting)
  {
    // Moved selection?
    if (m_dragStarted)
    {
      // Get selection samples:
      qint64 s1 = clientToSample(event->x());
      qint64 s2 = m_mouseDownSample;
      if (s1 > s2)
        std::swap(s1, s2);

      // Get selection channel:
      int channel = -1;
      if (m_extendingSelection)
        channel = document()->selectedChannel();
      else if (event->modifiers() & Qt::ShiftModifier)
        channel = clientToChannel(event->y());

      // Update selection:
      document()->undoStack()->push(new SelectingCommand(document(), s1, s2 - s1, channel, true));
    }

    // No, clear selection:
    else if (document()->selectedChannel() != -1 || document()->selectionStart() != 0 || document()->selectionLength() != 0)
      document()->undoStack()->push(new ClearSelectionCommand(document()));
  }

  // Finished zooming:
  else if (m_draggingMode == ZoomRuler)
  {
    // Just clicked on the ruler?
    if (!m_dragStarted)
    {
      // Set cursor position:
      document()->setCursorPosition(clientToSample(event->x()));
    }
  }

  // Reset mode:
  m_draggingMode = None;
  m_dragStarted = false;
  if (m_dragTimer->isActive())
    m_dragTimer->stop();

  // Update the cursor:
  updateCursor(event->pos());
}

void WaveEditView::mouseDoubleClickEvent(QMouseEvent* /* event */)
{
  // Set keyboard focus:
  setFocus();
}

void WaveEditView::keyPressEvent(QKeyEvent* event)
{
  // Get modifiers:
  bool shift   = (event->modifiers() & Qt::ShiftModifier) != 0;
  //bool control = (event->modifiers() & Qt::ControlModifier) != 0;
  //bool alt     = (event->modifiers() & Qt::AltModifier) != 0;

  if (event->key() == Qt::Key_Left)
  {
    // Move view:
    if (shift)
      setViewPosition(viewPosition() - m_scrollH->pageStep());
    else
      setViewPosition(viewPosition() - m_scrollH->singleStep());
  }

  else if (event->key() == Qt::Key_Right)
  {
    // Move view:
    if (shift)
      setViewPosition(viewPosition() + m_scrollH->pageStep());
    else
      setViewPosition(viewPosition() + m_scrollH->singleStep());
  }

  else if (event->key() == Qt::Key_Up)
  {
    // Not entirely zoomed out?
    if (zoomV() > 1.0)
    {
      // Calc visible height of a single channel:
      double channelHeight = (double)m_waveArea.height() / document()->channelCount();

      // Calc relative movement in the zoomed wave:
      double dz = 1.0 / (channelHeight * zoomV() - channelHeight);

      // Update position:
      if (shift)
        setPosV(posV() + dz * 25.0);
      else
        setPosV(posV() + dz * 5.0);
    }
  }

  else if (event->key() == Qt::Key_Down)
  {
    // Not entirely zoomed out?
    if (zoomV() > 1.0)
    {
      // Calc visible height of a single channel:
      double channelHeight = (double)m_waveArea.height() / document()->channelCount();

      // Calc relative movement in the zoomed wave:
      double dz = 1.0 / (channelHeight * zoomV() - channelHeight);

      // Update position:
      if (shift)
        setPosV(posV() - dz * 25.0);
      else
        setPosV(posV() - dz * 5.0);
    }
  }

  else if (event->key() == Qt::Key_Tab)
  {
    // Tab to transient...
  }

  else if (event->key() == Qt::Key_Escape)
  {
    // Cancel current operation...
  }
}

void WaveEditView::focusOutEvent(QFocusEvent* /* event */)
{
  // Reset mode:
  m_draggingMode = None;
  m_dragStarted = false;
  if (m_dragTimer->isActive())
    m_dragTimer->stop();
}

void WaveEditView::onViewportChanged()
{
  // Update scrollbars:
  updateScrollbars();

  // Update view:
  update();
}

void WaveEditView::btnPlusHPressed()
{
  // Get current center of view:
  qint64 center = viewPosition() + (viewLength() / 2);
  // If the play cursor is visible use this instead...

  // Zoom:
  zoomAt(center, 0.5);
}

void WaveEditView::btnMinusHPressed()
{
  // Get current center of view:
  qint64 center = viewPosition() + (viewLength() / 2);
  // If the play cursor is visible use this instead...

  // Zoom:
  zoomAt(center, 2.0);
}

void WaveEditView::btnPlusVPressed()
{
  // Don't zoom too far:
  if (zoomV() > 100.0)
    return;

  // Zoom in:
  setZoomV(zoomV() * 1.25);
}

void WaveEditView::btnMinusVPressed()
{
  // Zoom out:
  setZoomV(zoomV() / 1.25);

  // Clip zoom:
  if (zoomV() <= 0.25)
    setZoomV(0.25);

  // Center view on magnification:
  if (zoomV() <= 1.0)
    setPosV(0.5);
}

void WaveEditView::btnNullPressed()
{
  // Reset zoom and position:
  setZoomV(1.0);
  setPosV(0.5);
  setViewport(0, document()->sampleCount());
}

void WaveEditView::scrollHChanged(int value)
{
  // Already at the end?
  if (m_scrollH->maximum() == 0)
    return;

  // Update position:
  m_scrollbarsLocked = true;
  setViewPosition(value);
  m_scrollbarsLocked = false;
}

void WaveEditView::scrollVChanged(int value)
{
  // Already at the end?
  if (m_scrollV->maximum() == 0)
    return;

  // Update position:
  m_scrollbarsLocked = true;
  setPosV(1.0 - ((double)value / m_scrollV->maximum()));
  m_scrollbarsLocked = false;
}

void WaveEditView::dragTimerTick()
{
  // Get cursor position:
  QPoint p = mapFromGlobal(QCursor::pos());

  // Calc stepping:
  qint64 dx = m_scrollH->singleStep() / 5;
  if (dx < 1)
    dx = 1;

  // Get direction:
  bool left  = p.x() < (m_waveArea.left() + m_scrollOverlap);
  bool right = p.x() > (m_waveArea.right() - m_scrollOverlap);
  if (!left && !right)
    return;

  // Update view position:
  if (left)
    setViewPosition(viewPosition() - dx);
  else
    setViewPosition(viewPosition() + dx);

  if (m_draggingMode == Selecting)
  {
    // Get selection samples:
    qint64 s1 = clientToSample(left ? 0 : m_waveArea.right());
    qint64 s2 = m_mouseDownSample;
    if (s1 > s2)
      std::swap(s1, s2);

    // Get selection channel:
    int channel = -1;
    if (m_extendingSelection)
      channel = document()->selectedChannel();
    else if (QApplication::keyboardModifiers() & Qt::ShiftModifier)
      channel = clientToChannel(p.y());

    // Update selection:
    document()->undoStack()->push(new SelectingCommand(document(), s1, s2 - s1, channel));
  }
}

void WaveEditView::zoomAt(qint64 samplePos, double factor)
{
  // Do we have something to zoom?
  if (viewLength() <= 0)
    return;

  // Calc scaling factor to keep the sample position under the cursor:
  double posOffset = (double)(samplePos - viewPosition()) / viewLength();

  // Calc new length:
  qint64 newLen = viewLength() * factor;
  if (newLen == viewLength())
  {
    // Adjust for small increments on large scales:
    if (factor < 1.0)
      newLen--;
    else if (factor > 1.0)
      newLen++;
  }

  // Calc new start of view:
  qint64 newPos = samplePos - (newLen * posOffset);

  // Update viewport:
  setViewport(newPos, newLen);
}

void WaveEditView::updateScrollbars()
{
  // Scrollbars locked?
  if (m_scrollbarsLocked)
    return;

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
    double normalHeight = 10000;

    // Calc maximum value for the scroll bar:
    double maxVal = (normalHeight * zoomV()) - normalHeight;

    // Update values:
    m_scrollV->setMaximum((int)maxVal);
    m_scrollV->setPageStep((int)(normalHeight));
    m_scrollV->setValue((int)(maxVal * (1.0 - posV())));
    m_scrollV->setSingleStep((int)(normalHeight / 5.0));
  }

  // Zoomed out fully?
  if (viewLength() >= document()->sampleCount())
  {
    // Disable scrollbar:
    m_scrollH->setMinimum(0);
    m_scrollH->setMaximum(0);
    m_scrollH->setPageStep(0);
  }
  else
  {
    // Set sample count as maximum:
    qint64 normalWidth = document()->sampleCount();

    // Calc maximum value for the scroll bar:
    qint64 maxVal = normalWidth - viewLength();

    // Update values:
    m_scrollH->setMinimum(0);
    m_scrollH->setMaximum((int)maxVal);
    m_scrollH->setPageStep((int)viewLength());
    m_scrollH->setValue((int)viewPosition());
    int step = (int)((double)viewLength() / 5);
    if (!step)
      step = 1;
    m_scrollH->setSingleStep(step);
  }

  // Enable updates again:
  m_scrollV->blockSignals(oldV);
  m_scrollH->blockSignals(oldH);
}

void WaveEditView::updateWaveRect()
{
  // Update wave drawing area:
  m_waveArea = QRect(0, 0, width(), height());
  if (m_showScrollBars)
  {
    // Remove scroll bar areas:
    m_waveArea.setBottom(m_waveArea.bottom() - m_buttonSize);
    m_waveArea.setRight(m_waveArea.right() - m_buttonSize);
  }

  // Remove scale area:
  if (m_showScales)
    m_waveArea.setLeft(m_scalesWidth);

  // Remove ruler area:
  if (m_showRuler)
    m_waveArea.setTop(m_rulerHeight);
}

void WaveEditView::drawCorner(QPainter& painter)
{
  // Ruler visible?
  if (!m_showRuler && !m_showScales)
    return;

  // Set corner dimensions:
  QRect cornerRect(0, 0, m_waveArea.left(), m_waveArea.top());

  // Clear background:
  painter.fillRect(cornerRect, m_cornerBackColor);

  // Draw divider:
  painter.setPen(dividerColor());
  painter.drawLine(0, cornerRect.bottom(), cornerRect.right(), cornerRect.bottom());
  painter.drawLine(cornerRect.right(), cornerRect.top(), cornerRect.right(), cornerRect.bottom());

  // Draw button:
  int x = (cornerRect.width() - 16) / 2;
  int y = (cornerRect.height() - 16) / 2;
  QRect rc(x, y, 16, 16);
  painter.drawImage(rc, QImage(":/images/settings_bw.png"));
}

static const int SampleFlag = 1;
static const int MilliSecondFlag = 2;
static const int SecondFlag = 4;
static const int MinuteFlag = 8;
static const int HourFlag = 16;
static const int DayFlag = 32;

struct sampletime
{
  sampletime(double sampleRate) :
    sampleRate(sampleRate),
    samplePos(0),
    days(0),
    hours(0),
    minutes(0),
    seconds(0),
    milliseconds(0),
    samples(0)
  {
  }

  int flags()
  {
    if (days > 0)
      return SampleFlag | MilliSecondFlag | SecondFlag | MinuteFlag | HourFlag | DayFlag;
    if (hours > 0)
      return SampleFlag | MilliSecondFlag | SecondFlag | MinuteFlag | HourFlag;
    if (minutes > 0)
      return SampleFlag | MilliSecondFlag | SecondFlag | MinuteFlag;
    if (seconds > 0)
      return SampleFlag | MilliSecondFlag | SecondFlag;
    if (milliseconds > 0)
      return SampleFlag | MilliSecondFlag;
    if (samples > 0)
      return SampleFlag;
    return 0;
  }

  void fromSamplePos()
  {
    qint64 pos = samplePos;

    days = floor(pos / (sampleRate * 3600 * 24));
    pos -= days * sampleRate * 3600 * 24;

    hours = floor(pos / (sampleRate * 3600));
    pos -= hours * sampleRate * 3600;

    minutes = floor(pos / (sampleRate * 60));
    pos -= minutes * sampleRate * 60;

    seconds = floor(pos / sampleRate);
    pos -= seconds * sampleRate;

    milliseconds = floor((pos * 1000) / sampleRate);
    pos -= milliseconds * sampleRate / 1000;

    samples = pos;
  }

  void toSamplePos()
  {
    samplePos = days * sampleRate * 3600 * 24;

    samplePos += hours * sampleRate * 3600;

    samplePos += minutes * sampleRate * 60;

    samplePos += seconds * sampleRate;

    samplePos += (milliseconds * sampleRate) / 1000;

    samplePos += samples;
  }

  void add(const sampletime& other)
  {
    samples += other.samples;
    if (samples >= (sampleRate / 1000))
    {
      milliseconds++;
      samples -= sampleRate / 1000;
    }

    milliseconds += other.milliseconds;
    if (milliseconds >= 1000)
    {
      seconds++;
      milliseconds -= 1000;
    }

    seconds += other.seconds;
    if (seconds >= 60)
    {
      minutes++;
      seconds -= 60;
    }

    minutes += other.minutes;
    if (minutes >= 60)
    {
      hours++;
      minutes -= 60;
    }

    hours += other.hours;
    if (hours >= 24)
    {
      days++;
      hours -= 24;
    }

    days += other.days;
  }

  double sampleRate;
  qint64 samplePos;
  unsigned short days;
  unsigned char hours;
  unsigned char minutes;
  unsigned char seconds;
  unsigned short milliseconds;
  unsigned int samples;
};

void WaveEditView::drawRuler(QPainter& painter)
{
  // Ruler visible?
  if (!m_showRuler)
    return;

  // Set ruler dimensions:
  QRect rulerRect(m_waveArea.left(), 0, m_waveArea.width(), m_rulerHeight);

  // Clear background:
  painter.fillRect(rulerRect, m_rulerBackColor);

  // Draw divider:
  painter.setPen(dividerColor());
  painter.drawLine(rulerRect.left(), rulerRect.bottom(), rulerRect.right(), rulerRect.bottom());

  // Got a document?
  if (!document() || !document()->peakData().valid())
    return;

  // Find time at the end of the document:
  double sr = document()->sampleRate();
  qint64 len = viewPosition() + viewLength();
  double docDays = len / (sr * 3600 * 24);
  double docHours = len / (sr * 3600);
  double docMinutes = len / (sr * 60);
  double docSeconds = len / sr;
  double docMs = (len * 1000) / sr;
  double docSamples = len;

  // Find max text width:
  QString text = "";
  if (docDays > 0)
    text += "00d ";
  if (docHours > 0)
    text += "00h ";
  if (docMinutes > 0)
    text += "00m ";
  if (docSeconds > 0)
    text += "00s ";
  if (docMs > 0)
    text += "0000ms ";
  if (docSamples > 0)
    text += "00";
  QRect maxRect = painter.fontMetrics().boundingRect(text);

  // Find delta time:
  sampletime delta(document()->sampleRate());
  delta.samplePos = clientToSample(maxRect.width() + rulerRect.left()) - viewPosition();
  delta.fromSamplePos();

  sampletime step(document()->sampleRate());
  if (delta.days > 100)
    step.days = delta.days - (delta.days % 100);
  else if (delta.days > 50)
    step.days = delta.days - (delta.days % 50);
  else if (delta.days > 10)
    step.days = delta.days - (delta.days % 10);
  else if (delta.days > 0)
    step.days = delta.days;
  else if (delta.hours > 12)
    step.hours = delta.hours - (delta.hours % 12);
  else if (delta.hours > 0)
    step.hours = delta.hours;
  else if (delta.minutes > 30)
    step.minutes = delta.minutes - (delta.minutes % 30);
  else if (delta.minutes > 10)
    step.minutes = delta.minutes - (delta.minutes % 10);
  else if (delta.minutes > 0)
    step.minutes = delta.minutes;
  else if (delta.seconds > 30)
    step.seconds = delta.seconds - (delta.seconds % 30);
  else if (delta.seconds > 10)
    step.seconds = delta.seconds - (delta.seconds % 10);
  else if (delta.seconds > 0)
    step.seconds = delta.seconds;
  else if (delta.milliseconds > 500)
    step.milliseconds = delta.milliseconds - (delta.milliseconds % 500);
  else if (delta.milliseconds > 100)
    step.milliseconds = delta.milliseconds - (delta.milliseconds % 100);
  else if (delta.milliseconds > 10)
    step.milliseconds = delta.milliseconds - (delta.milliseconds % 10);
  else if (delta.milliseconds > 0)
    step.milliseconds = delta.milliseconds;
  else if (delta.samples > 1000)
    step.samples = delta.samples - (delta.samples % 1000);
  else if (delta.samples > 100)
    step.samples = delta.samples - (delta.samples % 100);
  else if (delta.samples > 10)
    step.samples = delta.samples - (delta.samples % 10);
  else
    step.samples = delta.samples;
  step.toSamplePos();
  if (step.samplePos == 0)
    step.samplePos = step.samples = 1;

  int stepFlags = step.flags();

  double cellWidth = (double)rulerRect.width() * ((double)step.samplePos / (double)viewLength());
  int cellCount = ((double)rulerRect.width() / cellWidth) + 1;

  sampletime curTime(document()->sampleRate());
  curTime.samplePos = viewPosition() - (viewPosition() % step.samplePos);
  curTime.fromSamplePos();

  int x = sampleToClient(curTime.samplePos);

  for (int i = 0; i < cellCount; i++)
  {
    int flags = curTime.flags() & ~(stepFlags >> 1);
    text = "";
    if (flags & DayFlag)
      text += QString("%1d ").arg(curTime.days);
    if (flags & HourFlag)
      text += QString("%1h ").arg(curTime.hours);
    if (flags & MinuteFlag)
      text += QString("%1m ").arg(curTime.minutes);
    if (flags & SecondFlag)
      text += QString("%1s ").arg(curTime.seconds);
    if (flags & MilliSecondFlag)
      text += QString("%1ms ").arg(curTime.milliseconds);
    if (flags & SampleFlag)
      text += QString("%1").arg(curTime.samples);
    if (text.isEmpty())
      text = "0";

    QRect textRect(x + 2, 0, maxRect.width(), rulerRect.height());
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop, text);

    int y1 = rulerRect.height() * 0.5;
    int y2 = rulerRect.height();
    int x1 = x;
    painter.drawLine(x1, y1, x1, y2);

    // Only draw in between steps when we are not on the lowest level
    if (step.samples == 0 || step.samples > 9)
    {
      y1 = rulerRect.height() * 0.75;
      x1 = x + cellWidth * 0.5 + 0.5;
      painter.drawLine(x1, y1, x1, y2);

      y1 = rulerRect.height() * 0.875;
      x1 = x + cellWidth * 0.1 + 0.5;
      painter.drawLine(x1, y1, x1, y2);
      x1 = x + cellWidth * 0.2 + 0.5;
      painter.drawLine(x1, y1, x1, y2);
      x1 = x + cellWidth * 0.3 + 0.5;
      painter.drawLine(x1, y1, x1, y2);
      x1 = x + cellWidth * 0.4 + 0.5;
      painter.drawLine(x1, y1, x1, y2);
      x1 = x + cellWidth * 0.6 + 0.5;
      painter.drawLine(x1, y1, x1, y2);
      x1 = x + cellWidth * 0.7 + 0.5;
      painter.drawLine(x1, y1, x1, y2);
      x1 = x + cellWidth * 0.8 + 0.5;
      painter.drawLine(x1, y1, x1, y2);
      x1 = x + cellWidth * 0.9 + 0.5;
      painter.drawLine(x1, y1, x1, y2);
    }

    curTime.add(step);
    x += cellWidth;
  }
}

void WaveEditView::drawScales(QPainter& painter)
{
  // Scales visible?
  if (!m_showScales)
    return;

  // Set scale area dimensions:
  QRect scaleRect(0, m_waveArea.top(), m_scalesWidth, m_waveArea.height());

  // Clear background:
  painter.fillRect(scaleRect, m_scalesBackColor);

  // Draw divider:
  painter.setPen(dividerColor());
  painter.drawLine(scaleRect.right(), scaleRect.top(), scaleRect.right(), scaleRect.bottom());

  // Got a document?
  if (!document() || !document()->peakData().valid())
    return;

  // Get height of a single channel:
  double channelHeight = (double)m_waveArea.height() / document()->channelCount();

  // This where the marker lines start:
  int markerX = (int)(0.75 * scaleRect.width());

  // Calc maximum text height:
  int fontHeight = painter.fontMetrics().height();
  int lineHeight = fontHeight * 3;

  // Enable clipping:
  painter.setClipping(true);

  // Draw channels:
  for (int channel = 0; channel < document()->channelCount(); channel++)
  {
    // Create target rect:
    QRect destRect(scaleRect.left(), channel * channelHeight + scaleRect.top(), scaleRect.width(), channelHeight);
    int ymin = destRect.top();
    int ymax = destRect.bottom();

    // Clip to target:
    painter.setClipRect(destRect);

    // Get center line:
    double y = destRect.top() + (channelHeight * 0.5) + (((channelHeight * zoomV()) - channelHeight) * (posV() - 0.5));

    // Draw center line:
    painter.drawLine(markerX, y, destRect.right(), y);

    // Draw center text:
    QRect centerRect(0, (int)(y - (fontHeight * 0.5)), markerX - 2, fontHeight);
    if (m_scalesDB)
      painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "-oo");
    else
      painter.drawText(centerRect, Qt::AlignRight | Qt::AlignVCenter, "0");

    // Calc division factor for the optimal display spacing:
    double div = 1.0;
    while ((channelHeight * zoomV() * div) > lineHeight)
      div *= 0.5;
    double dy = channelHeight * zoomV() * div * zoomVOverlap();

    // Draw all values:
    for (int i = 0; i < 2; i++)
    {
      int line = 0;
      while (true)
      {
        // Select line:
        line++;

        // Calc display value:
        double val;
        if (m_scalesDB)
          val = 20.0 * log10(line * div * 2.0);
        else
          val = line * div * 200.0;

        // Reproject value to find int position:
        double ty;
        if (m_scalesDB)
          ty = (pow(10.0, val / 20.0) * dy / (div * 2.0));
        else
          ty = val * dy / (div * 200.0);
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
        if (m_scalesDB && val > 0)
          painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString("+") + QString::number(val, 'f', 1));
        else
          painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, QString::number(val, 'f', 1));
      }
    }

    // Draw channel divider:
    if (drawChannelDivider() && channel > 0)
      painter.drawLine(0, destRect.top(), destRect.right(), destRect.top());
  }

  // Disable clipping:
  painter.setClipping(false);
}

qint64 WaveEditView::clientToSample(int x)
{
  // Calc position:
  return WaveView::clientToSample(m_waveArea, x);
}

int WaveEditView::sampleToClient(qint64 s)
{
  // Calc position:
  return WaveView::sampleToClient(m_waveArea, s);
}

int WaveEditView::clientToChannel(int y)
{
  // Calc channel:
  return (y - m_waveArea.top()) / (m_waveArea.height() / document()->channelCount());
}

void WaveEditView::updateCursor(const QPoint& pt)
{
  // Update the cursor:
  if (m_waveArea.contains(pt))
  {
    // Get selection samples:
    qint64 start = document()->selectionStart();
    qint64 end   = start + document()->selectionLength();

    // Selection visible?
    if (end > viewPosition() && start < (viewPosition() + viewLength()))
    {
      // Get selection pixels:
      int x1 = sampleToClient(start);
      int x2 = sampleToClient(end);

      // Are we hovering over the selection borders?
      if (abs(pt.x() - x1) < m_dragBorderDist || abs(pt.x() - x2) < m_dragBorderDist)
      {
        // Are we on the right channel?
        int channel = (pt.y() - m_waveArea.top()) / (m_waveArea.height() / document()->channelCount());
        if (document()->selectedChannel() < 0 || document()->selectedChannel() == channel)
          setCursor(Qt::SizeHorCursor);
        else
          setCursor(Qt::IBeamCursor);
      }
      else
        setCursor(Qt::IBeamCursor);
    }
    else
      setCursor(Qt::IBeamCursor);
  }
  else
    // Not in the wave area:
    setCursor(Qt::ArrowCursor);
}
