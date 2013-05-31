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

protected:

  virtual void closeEvent(QCloseEvent* event);
  virtual void paintEvent(QPaintEvent* event);

private:

  void drawRuler(const QRect& rulerRect, QPainter& painter);
  void drawScales(const QRect& scaleRect, QPainter& painter);

  Document* m_document;
  bool m_showRuler;
  bool m_showScales;
};

#endif // WAVEVIEW_H
