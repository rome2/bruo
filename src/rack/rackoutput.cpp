#include "bruo.h"
#include "document.h"
#include "../audio/samplebuffer.h"
#include "rack.h"
#include "rackdevice.h"
#include "rackoutput.h"
#include "rackoutputgui.h"
#include "dsp/vumeter.h"

RackOutput::RackOutput(class Rack* parent) :
  RackDevice(parent),
  m_gain(1.0),
  m_pan(0.5),
  m_muted(false),
  m_clipped(false)
{
  m_vus = new VUMeter[parent->document()->channelCount()];
  for (int i = 0; i < parent->document()->channelCount(); i++)
    m_vus[i].setSampleRate(parent->document()->sampleRate());
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
    {
      double val = *ptr;

      val *= m_muted ? 0.0 : m_gain;

      if (val > 1.0)
        m_clipped = true;

      m_vus[i].tick(val);

      *ptr = val;
    }
  }
}

RackDeviceGUI* RackOutput::createGUI(QWidget* parent)
{
  if (gui() != 0)
    return 0;
  setGui(new RackOutputGUI(this, parent));
  return gui();
}

double RackOutput::getVU(int channel) const
{
  if (channel >= 0 && channel < rack()->document()->channelCount())
    return m_vus[channel].vu();
  return 0.0;
}

bool RackOutput::clipped() const
{
  return m_clipped;
}

void RackOutput::resetClip()
{
  m_clipped = false;
}
