#include "rackdevicegui.h"

RackDeviceGUI::RackDeviceGUI(RackDevice* device, QWidget* parent) :
  QWidget(parent),
  m_device(device),
  m_aspectRatio(1.0),
  m_widgetPinner(this)
{
  QSizePolicy p = sizePolicy();
  p.setHeightForWidth(true);
  setSizePolicy(p);
}

RackDevice* RackDeviceGUI::device()
{
  return m_device;
}

const RackDevice* RackDeviceGUI::device() const
{
  return m_device;
}

void RackDeviceGUI::idle()
{
}

void RackDeviceGUI::parameterChanged(const int /* index */, const double /* value */)
{
}

int RackDeviceGUI::heightForWidth(int w) const
{
  return static_cast<int>(m_aspectRatio * w);
}

QSize RackDeviceGUI::sizeHint() const
{
  return QSize(3000, heightForWidth(3000));
}

double RackDeviceGUI::aspectRatio()
{
  return m_aspectRatio;
}

void RackDeviceGUI::setAspectRatio(double ratio)
{
  m_aspectRatio = ratio;
}

void RackDeviceGUI::addPinnedChild(QWidget* child)
{
  m_widgetPinner.addWidget(child);
}

void RackDeviceGUI::resizeEvent(QResizeEvent* /* event */)
{
  m_widgetPinner.resizeWidgets();
}
