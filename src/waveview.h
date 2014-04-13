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
  double posV() const;
  void setPosV(double newPos);
  double zoomVOverlap() const;
  void setZoomVOverlap(double newOverlap);
  qint64 viewLength() const;
  void setViewLength(qint64 newLength);
  qint64 viewPosition() const;
  void setViewPosition(qint64 newPos);
  void setViewport(qint64 newPos, qint64 newLength);
  const QColor& backColor() const;
  void setBackColor(const QColor& newColor);
  const QColor& centerColor() const;
  void setCenterColor(const QColor& newColor);
  const QColor& halfColor() const;
  void setHalfColor(const QColor& newColor);
  const QColor& waveColor() const;
  void setWaveColor(const QColor& newColor);
  const QColor& upperColor() const;
  void setUpperColor(const QColor& newColor);
  const QColor& lowerColor() const;
  void setLowerColor(const QColor& newColor);
  const QColor& dividerColor() const;
  void setDividerColor(const QColor& newColor);
  const QColor& selectionBackColor() const;
  void setSelectionBackColor(const QColor& newColor);
  const QColor& selectionBorderColor() const;
  void setSelectionBorderColor(const QColor& newColor);

signals:

  void viewportChanged();

protected:

  virtual void onViewportChanged();

  void drawPeaks(QRect& waveRect, QPainter& painter);
  qint64 clientToSample(const QRect& rc, const int x) const;
  int sampleToClient(const QRect& rc, qint64 s) const;
  void emitViewportChanged();

private:

  Document* m_document;
  bool m_drawHalfLine;
  bool m_drawChannelDivider;
  bool m_drawBackGradients;
  double m_zoomV;
  double m_zoomVOverlap;
  double m_posV;
  qint64 m_viewLength;
  qint64 m_viewPosition;
  QColor m_backColor;
  QColor m_centerColor;
  QColor m_halfColor;
  QColor m_waveColor;
  QColor m_upperColor;
  QColor m_lowerColor;
  QColor m_dividerColor;
  QColor m_selectionBackColor;
  QColor m_selectionBorderColor;
};

#endif // WAVEVIEW_H
