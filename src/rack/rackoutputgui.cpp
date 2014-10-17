#include "rackoutputgui.h"
#include "controls/imagedial.h"

RackOutputGUI::RackOutputGUI(RackDevice* device, QWidget* parent) :
  RackDeviceGUI(device, parent)
{
  setGeometry(0, 0, 933, 44);

  ImageDial* dial = new ImageDial(this);
  dial->setGeometry(0, 0, 48, 48);
  dial->setValue(device->parameter(0));
  dial->setDefaultValue(0.5);
  QPixmap dialImg("/home/rollo/coding/dtedit/src/images/knob_movie.png");
  dial->setImage(dialImg);
  dial->setFrameCount(61);
  dial->setEnabled(true);
  connect(dial, SIGNAL(valueChanged(double)), this, SLOT(dialChanged(double)));
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
