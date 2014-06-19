#include "waveruler.h"
#include "waveeditview.h"

WaveRuler::WaveRuler(class WaveEditView* editWindow, QWidget* parent) :
  QWidget(parent),
  m_master(editWindow),
  m_backColor(192, 192,192),
  m_foreColor(0, 0, 0),
  m_backBuff(0),
  m_dragStarted(false),
  m_oldViewPos(-1),
  m_oldViewLen(-1)
{
  // Connect to master:
  if (m_master != 0)
    connect(m_master, SIGNAL(viewportChanged()), this, SLOT(masterViewportChanged()));

  // We want an arrow cursor:
  setCursor(Qt::ArrowCursor);
}

WaveRuler::~WaveRuler()
{
  // Delete back buffer:
  if (m_backBuff != 0)
    delete m_backBuff;
  m_backBuff = 0;
}

const QColor& WaveRuler::backColor() const
{
  // Return current color:
  return m_backColor;
}

void WaveRuler::setBackColor(const QColor& newColor)
{
  // Anything to do?
  if (m_backColor == newColor)
    return;

  // Update value:
  m_backColor = newColor;

  // Redraw ruler:
  redraw();
}

const QColor& WaveRuler::foreColor() const
{
  // Return current color:
  return m_foreColor;
}

void WaveRuler::setForeColor(const QColor& newColor)
{
  // Anything to do?
  if (m_foreColor == newColor)
    return;

  // Update value:
  m_foreColor = newColor;

  // Redraw ruler:
  redraw();
}

void WaveRuler::paintEvent(QPaintEvent* /* event */)
{
  // Create painter:
  QPainter painter(this);

  // Draw ruler:
  if (m_backBuff != 0)
    painter.drawPixmap(0, 0, *m_backBuff);
}

void WaveRuler::resizeEvent(QResizeEvent* /* event */)
{
  // Don't do anything without a master:
  if (m_master == 0)
    return;

  // Update back buffer if needed:
  if (m_backBuff == 0 || width() != m_backBuff->width() || height() != m_backBuff->height())
  {
    // Delete old buffer:
    if (m_backBuff != 0)
      delete m_backBuff;
    m_backBuff = 0;

    // Create new:
    if (width() > 0 && height() > 0)
    {
      m_backBuff = new QPixmap(width(), height());
      m_backBuff->fill();
    }
  }

  // Redraw ruler:
  redraw();
}

void WaveRuler::wheelEvent(QWheelEvent* event)
{
  // No scrolling while we are doing something else:
  if (event->buttons() != Qt::NoButton)
    return;

  // Horizontal scrolling?
  if (event->orientation() == Qt::Horizontal || event->modifiers() & Qt::ShiftModifier)
  {
    int step = (int)((double)m_master->viewLength() / 5);
    if (!step)
      step = 1;
    if (event->delta() < 0)
      m_master->setViewPosition(m_master->viewPosition() + step);
    else
      m_master->setViewPosition(m_master->viewPosition() - step);
  }
}

void WaveRuler::mouseMoveEvent(QMouseEvent* event)
{
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
  if (event->buttons() == Qt::MiddleButton && dx != 0)
  {
    // Calc movement in sample space:
    dx = clientToSample(m_lastMousePos.x()) - clientToSample(event->pos().x());

    // Update position:
    m_master->setViewPosition(m_master->viewPosition() + dx);
  }

  // Are we dragging the ruler down with the left button (zooming)?
  else if (event->buttons() == Qt::LeftButton && dy != 0)
  {
    // Zoom at dragging position:
    m_master->zoomAt(clientToSample(m_mouseDownPos.x()), 1.0 - (0.01 * dy));
  }

  // Save position for next call:
  m_lastMousePos = event->pos();
}

void WaveRuler::mousePressEvent(QMouseEvent* event)
{
  // Save positions:
  m_lastMousePos = event->pos();
  m_mouseDownPos = event->pos();
  m_mouseDownSample = clientToSample(event->x());
  m_dragStarted = false;

  // Update cursor:
  if (event->buttons() == Qt::MiddleButton)
    setCursor(Qt::ClosedHandCursor);
}

void WaveRuler::mouseReleaseEvent(QMouseEvent* event)
{
  // Just clicked on the ruler?
  if (event->button() == Qt::LeftButton && !m_dragStarted)
  {
    // Set cursor position:
    m_master->document()->setCursorPosition(clientToSample(event->x()));
  }

  // Reset mode:
  m_dragStarted = false;

  // Update the cursor:
  setCursor(Qt::ArrowCursor);
}

void WaveRuler::masterViewportChanged()
{
  // Anything to do?
  if (m_oldViewPos == m_master->viewPosition() && m_oldViewLen == m_master->viewLength())
    return;

  // Redraw ruler:
  redraw();

  // Save state:
  m_oldViewPos = m_master->viewPosition();
  m_oldViewLen = m_master->viewLength();
}

void WaveRuler::redraw()
{
  // Redraw ruler:
  if (m_backBuff != 0)
  {
    QPainter painter(m_backBuff);
    drawRuler(painter);
  }

  // Update screen:
  update();
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

void WaveRuler::drawRuler(QPainter& painter)
{
  // Clear background:
  painter.fillRect(rect(), m_backColor);

  // Draw divider:
  painter.setPen(m_master->dividerColor());
  painter.drawLine(0, height() - 1, width(), height() - 1);

  // Got a document?
  if (m_master == 0 || !m_master->document())
    return;

  // Find time at the end of the ruler:
  double sr = m_master->document()->sampleRate();
  qint64 len = m_master->viewPosition() + m_master->viewLength();
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
  sampletime delta(m_master->document()->sampleRate());
  delta.samplePos = clientToSample(maxRect.width()) - m_master->viewPosition();
  delta.fromSamplePos();

  // Calc step width from one notch to the next:
  sampletime step(m_master->document()->sampleRate());
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

  // Get valid items of the step:
  int stepFlags = step.flags();

  // Calc cell width and cell count:
  double cellWidth = (double)width() * ((double)step.samplePos / (double)m_master->viewLength());
  int cellCount = ((double)width() / cellWidth) + 1;

  // Calc start time:
  sampletime curTime(m_master->document()->sampleRate());
  curTime.samplePos = m_master->viewPosition() - (m_master->viewPosition() % step.samplePos);
  curTime.fromSamplePos();
  int x = sampleToClient(curTime.samplePos);

  painter.setPen(m_foreColor);

  for (int i = 0; i < cellCount; i++)
  {
    // Compose text:
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

    // Draw text:
    QRect textRect(x + 2, 0, maxRect.width(), height());
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop, text);

    // Draw step line:
    int y1 = height() * 0.5;
    int y2 = height();
    int x1 = x;
    painter.drawLine(x1, y1, x1, y2);

    // Only draw in between steps when we are not on the lowest level
    if (step.samples == 0 || step.samples > 9)
    {
      // Middle:
      y1 = height() * 0.75;
      x1 = x + cellWidth * 0.5 + 0.5;
      painter.drawLine(x1, y1, x1, y2);

      // Steps 1, 2, 3, 4, 6, 7, 8, 9:
      y1 = height() * 0.875;
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

    // Next step:
    curTime.add(step);
    x += cellWidth;
  }
}

qint64 WaveRuler::clientToSample(int x) const
{
  // Calc position:
  double zoom = (double)m_master->viewLength() / width();
  qint64 s = m_master->viewPosition() + (zoom * ((x + 0.5)));
  if (s < 0)
    s = 0;
  return s;
}

int WaveRuler::sampleToClient(qint64 s) const
{
  // Calc position:
  double zoom = (double)m_master->viewLength() / width();
  return (int)(((double)(s - m_master->viewPosition()) / zoom) + 0.5);
}
