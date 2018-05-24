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
  m_balance(0.0),
  m_muted(false),
  m_clipped(false)
{
  m_vus = new VUMeter[2];
  for (int i = 0; i < 2; i++)
  {
    m_vus[i].setSampleRate(parent->document()->sampleRate());
    m_vus[i].setPeakMode(true);
    m_vus[i].setFalloff(300.0);
  }
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
    return m_gain.value() * 0.5;
  case 1:
    return (m_balance.value() + 1.0) * 0.5;
  case 2:
    return m_muted ? 1.0 : 0.0;
  }
  return 0.0;
}

void RackOutput::setParameter(const int index, const double value, const bool updateGUI)
{
  switch (index)
  {
  case 0:
    m_gain.setValue(value * 2.0);
    break;
  case 1:
    m_balance.setValue((value - 0.5) * 2.0);
    break;
  case 2:
    m_muted = value > 0.5;
    break;
  }
  if (updateGUI && gui())
    gui()->parameterChanged(index, value);
}

void RackOutput::setSampleRate(const double rate)
{
  RackDevice::setSampleRate(rate);
  m_gain.setSampleRate(rate);
  m_balance.setSampleRate(rate);
}

void RackOutput::process(const SampleBuffer& /*inputs*/, SampleBuffer& outputs, int frameCount, double /*streamTime*/)
{
  double* lptr = outputs.sampleBuffer(0);
  double* rptr = outputs.sampleBuffer(1);

  for (int j = 0; j < frameCount; j++)
  {
    double lval = *lptr;
    double rval = *rptr;

    double gain = m_gain.tick();
    if (gain > 1.0)
      gain += (gain - 1.0) * 8.0;
    if (m_muted)
      gain = 0.0;

    lval *= gain;
    rval *= gain;

    double bal = m_balance.tick();
    if (bal > 0.0)
      lval *= 1.0 - bal;
    else if (bal < 0.0)
      rval *= bal + 1.0;

    if (lval > 1.0 || rval > 1.0)
      m_clipped = true;

    m_vus[0].tick(lval);
    m_vus[1].tick(rval);

    *lptr++ = lval;
    *rptr++ = rval;
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
  if (channel >= 0 && channel < 2)
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
