#ifndef WAVEOVERVIEW_H
#define WAVEOVERVIEW_H

#include "waveview.h"
#include "waveeditview.h"

class WaveOverView :
  public WaveView
{
  Q_OBJECT // Qt magic...

public:

  WaveOverView(Document* doc, WaveEditView* slave, QWidget* parent = 0);
  virtual ~WaveOverView();

  bool drawViewOverlay() const;
  void setDrawViewOverlay(bool newState);
  const QColor& overlayColor() const;
  void setOverlayColor(const QColor& newColor);
  const QColor& overlayBorderColor() const;
  void setOverlayBorderColor(const QColor& newColor);
  int dragBorderDistance() const;
  void setDragBorderDistance(int newDist);

public slots:

  void slaveViewportChanged();

protected:

  virtual void paintEvent(QPaintEvent* event);
  virtual void resizeEvent(QResizeEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void mouseDoubleClickEvent(QMouseEvent* event);
  virtual void onViewportChanged();

private:

  void updateViewPort();
  qint64 clientToSample(int x);

  WaveEditView* m_slave;
  QRect m_slaveViewPort;
  bool m_drawOverlay;
  QColor m_overlayColor;
  QColor m_overlayBorderColor;
  int m_dragBorderDist;

  QPixmap* m_backBuff;
  QPoint m_mouseDownPos;
  qint64 m_mouseDownViewPos;
  qint64 m_mouseDownViewLen;
  bool m_dragOverlay;
  bool m_dragLeft;
  bool m_dragRight;
};

#endif // WAVEOVERVIEW_H
