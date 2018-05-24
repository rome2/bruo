#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rackdevice.h"
#include "rack.h"
#include "rackdevicegui.h"
#include "rackinput.h"
#include "rackoutput.h"
#include "document.h"

Rack::Rack(Document* doc) :
  m_doc(doc),
  m_suspended(false),
  m_sampleRate(44100),
  m_blockSize(4096)
{
  // Add input and output:
  m_devices.append(new RackInput(this));
  m_devices.append(new RackOutput(this));
}

Rack::~Rack()
{
  // Clear rack:
  for (int i = 0; i <m_devices.count(); i++)
    delete m_devices[i];
  m_devices.clear();
}

Document* Rack::document()
{
  return m_doc;
}

const Document* Rack::document() const
{
  return m_doc;
}

QList<RackDevice*>& Rack::devices()
{
  return m_devices;
}

const QList<RackDevice*>& Rack::devices() const
{
  return m_devices;
}

bool Rack::suspended() const
{
  return m_suspended;
}

void Rack::suspend()
{
  if (m_suspended)
    return;
  m_suspended = true;

  // Process all devices:
  for (int i = 0; i < m_devices.count(); i++)
    m_devices[i]->suspend();
}

void Rack::resume()
{
  if (!m_suspended)
    return;
  m_suspended = false;

  // Process all devices:
  for (int i = 0; i < m_devices.count(); i++)
    m_devices[i]->resume();
}

double Rack::sampleRate() const
{
  return m_sampleRate;
}

void Rack::setSampleRate(const double rate)
{
  if (m_sampleRate == rate)
    return;

  m_sampleRate = rate;

  // Process all devices:
  for (int i = 0; i < m_devices.count(); i++)
    m_devices[i]->setSampleRate(rate);
}

int Rack::blockSize() const
{
  return m_blockSize;
}

void Rack::setBlockSize(const int size)
{
  if (m_blockSize == size)
    return;

  m_blockSize = size;

  // Process all devices:
  for (int i = 0; i < m_devices.count(); i++)
    m_devices[i]->setBlockSize(size);
}

void Rack::process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime)
{
  // Start with empty buffer:
  outputs.makeSilence();

  // Disabled?
  if (m_suspended)
    return;

  // Process all devices:
  for (int i = 0; i < m_devices.count(); i++)
    m_devices[i]->process(inputs, outputs, frameCount, streamTime);
}

