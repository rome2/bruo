#ifndef RACKOUTPUTGUI_H
#define RACKOUTPUTGUI_H

#include "rackdevicegui.h"

class RackOutputGUI : public RackDeviceGUI
{
  Q_OBJECT
public:

  RackOutputGUI(class RackOutput* device, QWidget* parent);

  virtual void idle();

private slots:

  void dialChanged(double newValue);
  void clipClicked();

protected:

  virtual void paintEvent(QPaintEvent* event);

private:
  class ImageVU* m_vuLeft;
  class ImageVU* m_vuRight;
  class ImageLED* m_clip;
};

#endif // RACKOUTPUTGUI_H
