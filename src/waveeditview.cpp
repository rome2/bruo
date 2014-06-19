#include "waveeditview.h"
#include "commands/selectingcommand.h"
#include "commands/clearselectioncommand.h"

WaveEditView::WaveEditView(Document* doc, QWidget* parent) :
  WaveView(doc, parent),
  m_showRuler(true),
  m_showScales(true),
  m_showScrollBars(true),
  m_buttonSize(24),
  m_rulerHeight(30),
  m_scalesWidth(60),
  m_rulerBackColor(192, 192,192),
  m_cornerColor(0, 0, 0),
  m_cornerBackColor(176, 176, 176),
  m_dragBorderDist(5),
  m_scrollOverlap(10),
  m_backBuff(0),
  m_scrollbarsLocked(false),
  m_draggingMode(None),
  m_extendingSelection(false),
  m_dragStarted(false)
{
  // We want the keyboard focus:
  setFocusPolicy(Qt::StrongFocus);

  // Enable all mouse move events:
  setMouseTracking(true);

  // Create scales:
  m_scales = new WaveScales(this, this);
  m_ruler = new WaveRuler(this, this);

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
  // Delete back buffer:
  if (m_backBuff != 0)
    delete m_backBuff;
  m_backBuff = 0;
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
  m_ruler->setVisible(newState);
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
  m_scales->setVisible(newState);
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

void WaveEditView::paintEvent(QPaintEvent* /* event */)
{
  // Create painter:
  QPainter painter(this);

  // Draw peaks:
  if (m_backBuff != 0)
    painter.drawPixmap(m_waveArea.left(), m_waveArea.top(), *m_backBuff);

  // Draw selection overlay:
  drawSelection(m_waveArea, painter);

  // Draw cursor:
  drawPlayCursor(m_waveArea, painter);

  // Draw update state:
  drawUpdateState(m_waveArea, painter);

  // Draw corner:
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

  // Move scales:
  if (m_showScales)
    m_scales->setGeometry(0, m_waveArea.top(), m_scalesWidth, m_waveArea.height());

  // Move ruler:
  if (m_showRuler)
    m_ruler->setGeometry(m_waveArea.left(), 0, m_waveArea.width(), m_rulerHeight);
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

    // No horizontal zooming:
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

  // Moving the content with the middle button?
  if (m_draggingMode == ContentDrag)
  {
    // Horizontal move?
    if (dx != 0)
    {
      // Calc movement in sample space:
      dx = clientToSample(m_lastMousePos.x()) - clientToSample(event->pos().x());

      // Update position:
      setViewPosition(viewPosition() + dx);
    }

    // Vertical move?
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

  // Are we selecting?
  else if (m_draggingMode == Selecting)
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

  // Hit the wave area?
  if (m_waveArea.contains(event->pos()))
  {
    if (event->buttons() == Qt::MiddleButton)
    {
      m_draggingMode = ContentDrag;
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

  // Redraw peaks:
  QPainter wavePainter(m_backBuff);
  QRect peaksRect(0, 0, m_waveArea.width(), m_waveArea.height());
  drawPeaks(peaksRect, wavePainter);

  // Update view:
  update();
}

void WaveEditView::btnPlusHPressed()
{
  // Get current center of view:
  qint64 center = viewPosition() + (viewLength() / 2);

  // If the play cursor is visible use this instead:
  if (document()->cursorPosition() > viewPosition() && document()->cursorPosition() < (viewPosition() + viewLength()))
    center = document()->cursorPosition();

  // Zoom:
  zoomAt(center, 0.5);
}

void WaveEditView::btnMinusHPressed()
{
  // Get current center of view:
  qint64 center = viewPosition() + (viewLength() / 2);

  // If the play cursor is visible use this instead:
  if (document()->cursorPosition() > viewPosition() && document()->cursorPosition() < (viewPosition() + viewLength()))
    center = document()->cursorPosition();

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

  // Modifi selection?
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

void WaveEditView::updateScrollbars()
{
  // Scrollbars locked?
  if (m_scrollbarsLocked || document() == 0)
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

  // Update back buffer if needed:
  if (m_backBuff == 0 || m_waveArea.width() != m_backBuff->width() || m_waveArea.height() != m_backBuff->height())
  {
    // Delete old buffer:
    if (m_backBuff != 0)
      delete m_backBuff;
    m_backBuff = 0;

    // Create new:
    if (m_waveArea.width() > 0 && m_waveArea.height() > 0)
    {
      m_backBuff = new QPixmap(m_waveArea.width(), m_waveArea.height());
      m_backBuff->fill();
    }
  }

  // Redraw peaks:
  if (m_backBuff != 0)
  {
    QPainter wavePainter(m_backBuff);
    QRect peaksRect(0, 0, m_waveArea.width(), m_waveArea.height());
    drawPeaks(peaksRect, wavePainter);
  }
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
