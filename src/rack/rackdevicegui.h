#ifndef RACKDEVICEGUI_H
#define RACKDEVICEGUI_H

#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rackdevice.h"

class RackDeviceGUI : public QWidget
{
  Q_OBJECT

public:
  RackDeviceGUI(RackDevice* device, QWidget* parent);

  RackDevice* device();
  const RackDevice* device() const;

  virtual void idle();

signals:

public slots:

private:

  RackDevice* m_device;
};

#endif // RACKDEVICEGUI_H
