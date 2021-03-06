#ifndef WAVEEDITVIEW_H
#define WAVEEDITVIEW_H

#include "waveview.h"
#include "wavescales.h"
#include "waveruler.h"

class WaveEditView :
  public WaveView
{
  Q_OBJECT // Qt magic...

public:

  WaveEditView(Document* doc, QWidget* parent);
  virtual ~WaveEditView();

  bool showRuler() const;
  void setShowRuler(bool newState);
  bool showScales() const;
  void setShowScales(bool newState);
  bool showScrollBars() const;
  void setShowScrollBars(bool newState);

  void zoomAt(qint64 samplePos, double factor);

protected:

  virtual void paintEvent(QPaintEvent* event);
  virtual void resizeEvent(QResizeEvent* event);
  virtual void wheelEvent(QWheelEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void mouseDoubleClickEvent(QMouseEvent* event);
  virtual void keyPressEvent(QKeyEvent* event);
  virtual void focusOutEvent(QFocusEvent* event);

  virtual void onViewportChanged();

public slots:

  void btnPlusHPressed();
  void btnMinusHPressed();
  void btnPlusVPressed();
  void btnMinusVPressed();
  void btnNullPressed();

private slots:
  void scrollHChanged(int value);
  void scrollVChanged(int value);
  void dragTimerTick();

private:

  typedef enum DragMode
  {
    None = 0,
    ContentDrag = 1,
    Selecting = 2
  } DragMode;

  void updateScrollbars();
  void updateWaveRect();
  void drawCorner(QPainter& painter);
  void drawRuler(QPainter& painter);
  qint64 clientToSample(int x);
  int sampleToClient(qint64 s);
  int clientToChannel(int y);
  void updateCursor(const QPoint& pt);

  WaveScales* m_scales;
  WaveRuler* m_ruler;
  QScrollBar* m_scrollH;
  QScrollBar* m_scrollV;
  QPushButton* m_btnPlusH;
  QPushButton* m_btnPlusV;
  QPushButton* m_btnMinusH;
  QPushButton* m_btnMinusV;
  QPushButton* m_btnNull;
  QTimer* m_dragTimer;

  bool m_showRuler;
  bool m_showScales;
  bool m_showScrollBars;
  int m_buttonSize;
  int m_rulerHeight;
  int m_scalesWidth;
  QColor m_rulerBackColor;
  QColor m_cornerColor;
  QColor m_cornerBackColor;
  int m_dragBorderDist;
  int m_scrollOverlap;

  QPixmap* m_backBuff;
  QRect m_waveArea;
  bool m_scrollbarsLocked;
  DragMode m_draggingMode;
  QPoint m_lastMousePos;
  QPoint m_mouseDownPos;
  qint64 m_mouseDownSample;
  bool m_extendingSelection;
  bool m_dragStarted;
};

#endif // WAVEEDITVIEW_H
