#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rack.h"
#include "rackdevice.h"
#include "rackoutput.h"
#include "rackoutputgui.h"

RackOutput::RackOutput(class Rack* parent) :
  RackDevice(parent),
  m_gain(1.0),
  m_pan(0.5),
  m_muted(false)
{
}

RackOutput::~RackOutput()
{
}

int RackOutput::parameterCount() const
{
  return 3;
}

double RackOutput::parameter(const int index)
{
  switch (index)
  {
  case 0:
    return m_gain * 0.5;
  case 1:
    return m_pan;
  case 2:
    return m_muted ? 1.0 : 0.0;
  }
  return 0.0;
}

void RackOutput::setParameter(const int index, const double value)
{
  switch (index)
  {
  case 0:
    m_gain = value * 2.0;
    break;
  case 1:
    m_pan = value;
    break;
  case 2:
    m_muted = value > 0.5;
    break;
  }
}

void RackOutput::process(const SampleBuffer& /*inputs*/, SampleBuffer& outputs, int frameCount, double /*streamTime*/)
{
  for (int i = 0; i < outputs.channelCount(); i++)
  {
    double* ptr = outputs.sampleBuffer(i);
    for (int j = 0; j < frameCount; j++, ptr++)
      *ptr *= m_gain;
  }
}

RackDeviceGUI* RackOutput::createGUI(QWidget* parent)
{
  if (gui() != 0)
    return 0;
  setGui(new RackOutputGUI(this, parent));
  return gui();
}
