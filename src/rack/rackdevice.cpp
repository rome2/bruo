#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rackdevice.h"

RackDevice::RackDevice(class Rack* parent) :
  m_rack(parent)
{
}

RackDevice::~RackDevice()
{
}

int RackDevice::parameterCount() const
{
  return 0;
}

double RackDevice::parameter(const int index)
{
  return 0.0;
}

void RackDevice::setParameter(const int /*index*/, const double /*value*/)
{
}

void RackDevice::setParameterAutomated(const int /*index*/, const double /*value*/)
{
}

Rack* RackDevice::rack()
{
  return m_rack;
}

const Rack* RackDevice::rack() const
{
  return m_rack;
}

void RackDevice::process(const SampleBuffer& /*inputs*/, SampleBuffer& /*outputs*/, int /*frameCount*/, double /*streamTime*/)
{
}
