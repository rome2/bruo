#ifndef RACKOUTPUTGUI_H
#define RACKOUTPUTGUI_H

#include "rackdevicegui.h"

class RackOutputGUI : public RackDeviceGUI
{
  Q_OBJECT
public:

  RackOutputGUI(RackDevice* device, QWidget* parent);

private slots:

  void dialChanged(double newValue);

protected:

  virtual void paintEvent(QPaintEvent* event);
};

#endif // RACKOUTPUTGUI_H
