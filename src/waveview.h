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

protected:

  virtual void paintEvent(QPaintEvent* event);

private:

  void drawRuler(QRect& waveRect, QPainter& painter);
  void drawScales(QRect& waveRect, QPainter& painter);

  Document* m_document;
  bool m_showRuler;
  bool m_showScales;
  bool m_drawHalfLine;
  bool m_drawBackGradients;
};

#endif // WAVEVIEW_H
