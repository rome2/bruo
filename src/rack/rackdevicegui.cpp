#include "rackdevicegui.h"

RackDeviceGUI::RackDeviceGUI(RackDevice* device, QWidget* parent) :
  QWidget(parent),
  m_device(device)
{
}

RackDevice* RackDeviceGUI::device()
{
  return m_device;
}

const RackDevice* RackDeviceGUI::device() const
{
  return m_device;
}
