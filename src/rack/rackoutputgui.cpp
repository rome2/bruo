#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rack.h"
#include "rackdevice.h"
#include "rackoutput.h"
#include "rackoutputgui.h"
#include "controls/imagedial.h"
#include "controls/imagevu.h"
#include "controls/imageled.h"

RackOutputGUI::RackOutputGUI(RackOutput* device, QWidget* parent) :
  RackDeviceGUI(device, parent)
{
  setGeometry(0, 0, 933, 44);

  ImageDial* dial = new ImageDial(this);
  dial->setGeometry(0, 0, 48, 48);
  dial->setValue(device->parameter(0));
  dial->setDefaultValue(0.5);
  dial->image().load("/home/rollo/coding/dtedit/src/images/knob_movie.png");
  dial->setFrameCount(61);
  dial->setEnabled(true);
  connect(dial, SIGNAL(valueChanged(double)), this, SLOT(dialChanged(double)));

  m_vuLeft = new ImageVU(this);
  m_vuLeft->image().load(":/images/rack/rackoutput/meterbackl.png");
  m_vuLeft->setGeometry(500, 5, 161, 9);

  m_vuRight = new ImageVU(this);
  m_vuRight->image().load(":/images/rack/rackoutput/meterbackr.png");
  m_vuRight->setGeometry(500, 20, 161, 9);

  m_clip = new ImageLED(this);
  m_clip->setGeometry(300, 4, 22, 22);
  m_clip->image().load(":/images/rack/rackoutput/clip.png");
  connect(m_clip, SIGNAL(clicked()), this, SLOT(clipClicked()));
}

void RackOutputGUI::idle()
{
  RackDeviceGUI::idle();

  RackOutput* dev = static_cast<RackOutput*>(device());
  m_vuLeft->setValue(dev->getVU(0));
  m_vuRight->setValue(dev->getVU(1));
  m_clip->setValue(dev->clipped());
}

void RackOutputGUI::paintEvent(QPaintEvent* /* event */)
{
  QPainter painter(this);
  painter.fillRect(rect(), QColor(0xFF, 0, 0));
}

void RackOutputGUI::dialChanged(double newValue)
{
  device()->setParameter(0, newValue);
}

void RackOutputGUI::clipClicked()
{
  static_cast<RackOutput*>(device())->resetClip();
}
