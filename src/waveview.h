#ifndef WAVEVIEW_H
#define WAVEVIEW_H

#include "documentmanager.h"

class WaveView :
  public QWidget
{
  Q_OBJECT // Qt magic...

public:

  WaveView(Document* doc, QWidget* parent = 0);
  Document* document();
  const Document* document() const;
  void setDocument(Document* doc);
  bool showRuler() const;
  void setShowRuler(bool newState);
  bool showScales() const;
  void setShowScales(bool newState);
  bool drawHalfLine() const;
  void setDrawHalfLine(bool newState);
  bool drawBackGradients() const;
  void setDrawBackGradients(bool newState);
  bool showScrollBars() const;
  void setShowScrollBars(bool newState);

protected:

  virtual void paintEvent(QPaintEvent* event);
  virtual void resizeEvent(QResizeEvent* event);

private slots:

  void btnPlusHPressed();
  void btnMinusHPressed();
  void btnPlusVPressed();
  void btnMinusVPressed();
  void btnNullPressed();
  void scrollHChanged(int value);
  void scrollVChanged(int value);

private:

  void updateScrollbars();
  void drawRuler(QRect& waveRect, QPainter& painter);
  void drawScales(QRect& waveRect, QPainter& painter);

  QScrollBar* m_scrollH;
  QScrollBar* m_scrollV;
  QPushButton* m_btnPlusH;
  QPushButton* m_btnPlusV;
  QPushButton* m_btnMinusH;
  QPushButton* m_btnMinusV;
  QPushButton* m_btnNull;
  Document* m_document;
  bool m_showRuler;
  bool m_showScales;
  bool m_scalesDB;
  bool m_drawHalfLine;
  bool m_drawBackGradients;
  bool m_showScrollBars;
  int m_buttonSize;
  int m_rulerHeight;
  int m_scalesWidth;
  double m_zoomV;
  double m_zoomH;
  double m_posV;
  double m_posH;
};

#endif // WAVEVIEW_H
