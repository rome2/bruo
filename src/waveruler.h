#ifndef WAVERULER_H
#define WAVERULER_H

#include "bruo.h"

class WaveRuler :
  public QWidget
{
  Q_OBJECT // Qt magic...

public:

  WaveRuler(class WaveEditView* editWindow, QWidget* parent);
  virtual ~WaveRuler();

  const QColor& backColor() const;
  void setBackColor(const QColor& newColor);
  const QColor& foreColor() const;
  void setForeColor(const QColor& newColor);

protected:

  virtual void paintEvent(QPaintEvent* event);
  virtual void resizeEvent(QResizeEvent* event);
  virtual void wheelEvent(QWheelEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);

public slots:

  void masterViewportChanged();

private:

  void redraw();
  void drawRuler(QPainter& painter);
  qint64 clientToSample(int x) const;
  int sampleToClient(qint64 s) const;

  class WaveEditView* m_master;
  QColor m_backColor;
  QColor m_foreColor;
  QPixmap* m_backBuff;
  QPoint m_lastMousePos;
  QPoint m_mouseDownPos;
  qint64 m_mouseDownSample;
  bool m_dragStarted;
  qint64 m_oldViewPos;
  qint64 m_oldViewLen;
};

#endif // WAVERULER_H
