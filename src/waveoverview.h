#ifndef WAVEOVERVIEW_H
#define WAVEOVERVIEW_H

#include "waveview.h"

class WaveOverView :
  public WaveView
{
  Q_OBJECT // Qt magic...

public:

  WaveOverView(Document* doc, QWidget* parent = 0);
  virtual ~WaveOverView();

protected:

  virtual void paintEvent(QPaintEvent* event);
};

#endif // WAVEOVERVIEW_H
