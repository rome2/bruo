#ifndef RACKOUTPUTGUI_H
#define RACKOUTPUTGUI_H

#include "rackdevicegui.h"

class RackOutputGUI : public RackDeviceGUI
{
  Q_OBJECT
public:

  RackOutputGUI(class RackOutput* device, QWidget* parent);

  virtual void idle();
  virtual void parameterChanged(const int index, const double value);

private slots:

  void volumeChanged(double newValue);
  void balanceChanged(double newValue);
  void clipClicked();

protected:

  virtual void paintEvent(QPaintEvent* event);

private:
  class VectorDial* m_volumeDial;
  class VectorDial* m_balanceDial;
  class ImageVU* m_vuLeft;
  class ImageVU* m_vuRight;
  class VectorLED* m_clip;
  QColor m_backColor; ///\> Background color.
  QColor m_borderColor; ///\> Border color.
};

#endif // RACKOUTPUTGUI_H
