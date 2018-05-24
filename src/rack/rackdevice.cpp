#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rackdevice.h"

RackDevice::RackDevice(class Rack* parent) :
  m_rack(parent),
  m_gui(0),
  m_sampleRate(44100.0),
  m_blockSize(4096),
  m_suspendCounter(1)
{
}

RackDevice::~RackDevice()
{
}

void RackDevice::open()
{
}

void RackDevice::close()
{
}

void RackDevice::suspend()
{
  m_suspendCounter++;
}

void RackDevice::resume()
{
  m_suspendCounter--;
  if (m_suspendCounter < 0)
    m_suspendCounter = 0;
}

bool RackDevice::suspended() const
{
  return m_suspendCounter > 0;
}

int RackDevice::parameterCount() const
{
  return 0;
}

double RackDevice::parameter(const int /*index*/)
{
  return 0.0;
}

void RackDevice::setParameter(const int /*index*/, const double /*value*/, const bool /* updateGUI */)
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

double RackDevice::sampleRate() const
{
  return m_sampleRate;
}

void RackDevice::setSampleRate(const double rate)
{
  m_sampleRate = rate;
}

int RackDevice::blockSize() const
{
  return m_blockSize;
}

void RackDevice::setBlockSize(const int size)
{
  m_blockSize = size;
}

void RackDevice::process(const SampleBuffer& /*inputs*/, SampleBuffer& /*outputs*/, int /*frameCount*/, double /*streamTime*/)
{
}

RackDeviceGUI* RackDevice::createGUI(QWidget* /*parent*/)
{
  return 0;
}

void RackDevice::guiDestroyed()
{
  m_gui = 0;
}

RackDeviceGUI* RackDevice::gui()
{
  return m_gui;
}

const RackDeviceGUI* RackDevice::gui() const
{
  return m_gui;
}

void RackDevice::setGui(RackDeviceGUI* newGui)
{
  m_gui = newGui;
}
