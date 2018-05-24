#include "../bruo.h"
#include "../audio/samplebuffer.h"
#include "rack.h"
#include "rackdevice.h"
#include "rackoutput.h"
#include "rackoutputgui.h"
#include "../controls/vectordial.h"
#include "../controls/imagevu.h"
#include "../controls/vectorled.h"

RackOutputGUI::RackOutputGUI(RackOutput* device, QWidget* parent) :
  RackDeviceGUI(device, parent),
  m_backColor(106, 115, 123),
  m_borderColor(140, 149, 157)
{
  setGeometry(0, 0, 933, 60);

  m_volumeDial = new VectorDial(this);
  m_volumeDial->setGeometry(5, 5, 50, 50);
  m_volumeDial->setValue(device->parameter(0));
  m_volumeDial->setDefaultValue(0.5);
  m_volumeDial->setEnabled(true);
  connect(m_volumeDial, SIGNAL(valueChanged(double)), this, SLOT(volumeChanged(double)));

  m_balanceDial = new VectorDial(this);
  m_balanceDial->setGeometry(60, 5, 50, 50);
  m_balanceDial->setValue(device->parameter(1));
  m_balanceDial->setDefaultValue(0.5);
  m_balanceDial->setEnabled(true);
  m_balanceDial->setArcBase(0.5);
  connect(m_balanceDial, SIGNAL(valueChanged(double)), this, SLOT(balanceChanged(double)));

  m_vuLeft = new ImageVU(this);
  m_vuLeft->image().load(":/images/rack/rackoutput/meterbackl.png");
  m_vuLeft->setGeometry(120, 5, 200, 25);
  m_vuRight = new ImageVU(this);
  m_vuRight->image().load(":/images/rack/rackoutput/meterbackr.png");
  m_vuRight->setGeometry(120, 30, 200, 25);

  m_clip = new VectorLED(this);
  m_clip->setGeometry(400, 4, 22, 22);
  m_clip->setBackColor(m_backColor);
  connect(m_clip, SIGNAL(clicked()), this, SLOT(clipClicked()));

  setAspectRatio(static_cast<double>(height()) / width());
  addPinnedChild(m_volumeDial);
  addPinnedChild(m_balanceDial);
  addPinnedChild(m_vuLeft);
  addPinnedChild(m_vuRight);
  addPinnedChild(m_clip);
}

void RackOutputGUI::idle()
{
  RackDeviceGUI::idle();

  RackOutput* dev = static_cast<RackOutput*>(device());
  m_vuLeft->setValue(dev->getVU(0));
  m_vuRight->setValue(dev->getVU(1));
  m_clip->setValue(dev->clipped());
}

void RackOutputGUI::parameterChanged(const int index, const double value)
{
  bool b;
  switch (index)
  {
  case 0:
    b = m_volumeDial->blockSignals(true);
    m_volumeDial->setValue(value);
    m_volumeDial->blockSignals(b);
    break;
  case 1:
    b = m_balanceDial->blockSignals(true);
    m_balanceDial->setValue(value);
    m_balanceDial->blockSignals(b);
    break;
  }
}

void RackOutputGUI::paintEvent(QPaintEvent* /* event */)
{
  QPainter painter(this);
  painter.fillRect(rect(), m_backColor);
  painter.setPen(m_borderColor);
  painter.drawRect(0, 0, width() - 1, height() - 1);
}

void RackOutputGUI::volumeChanged(double newValue)
{
  device()->setParameter(0, newValue, false);
}

void RackOutputGUI::balanceChanged(double newValue)
{
  device()->setParameter(1, newValue, false);
}

void RackOutputGUI::clipClicked()
{
  static_cast<RackOutput*>(device())->resetClip();
}
