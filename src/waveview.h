#ifndef WAVEVIEW_H
#define WAVEVIEW_H

#include "documentmanager.h"

class WaveView :
  public QWidget
{
  Q_OBJECT // Qt magic...

public:

  WaveView(Document* doc, QWidget* parent = 0);
  virtual ~WaveView();

  Document* document();
  const Document* document() const;
  void setDocument(Document* doc);
  bool drawHalfLine() const;
  void setDrawHalfLine(bool newState);
  bool drawChannelDivider() const;
  void setDrawChannelDivider(bool newState);
  bool drawBackGradients() const;
  void setDrawBackGradients(bool newState);
  double zoomV() const;
  void setZoomV(double newZoom);
  double zoomH() const;
  void setZoomH(double newZoom);
  double posV() const;
  void setPosV(double newPos);
  double posH() const;
  void setPosH(double newPos);
  double zoomVOverlap() const;
  void setZoomVOverlap(double newOverlap);

protected:

  void drawPeaks(QRect& waveRect, QPainter& painter);

private:

  Document* m_document;
  bool m_drawHalfLine;
  bool m_drawChannelDivider;
  bool m_drawBackGradients;
  double m_zoomV;
  double m_zoomVOverlap;
  double m_zoomH;
  double m_posV;
  double m_posH;
  QColor m_backColor;
  QColor m_centerColor;
  QColor m_halfColor;
  QColor m_waveColor;
  QColor m_upperColor;
  QColor m_lowerColor;
  QColor m_dividerColor;
};

#endif // WAVEVIEW_H
