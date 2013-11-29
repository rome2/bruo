#ifndef WAVEEDITVIEW_H
#define WAVEEDITVIEW_H

#include "waveview.h"

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
  bool m_showRuler;
  bool m_showScales;
  bool m_scalesDB;
  bool m_showScrollBars;
  int m_buttonSize;
  int m_rulerHeight;
  int m_scalesWidth;
};

#endif // WAVEEDITVIEW_H
