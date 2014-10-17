#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rackdevice.h"
#include "rack.h"
#include "rackdevicegui.h"
#include "rackinput.h"
#include "rackoutput.h"
#include "document.h"

Rack::Rack(Document* doc) :
  m_doc(doc)
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

void Rack::process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime)
{
  // Start with empty buffer:
  outputs.makeSilence();

  // Process all devices:
  for (int i = 0; i <m_devices.count(); i++)
    m_devices[i]->process(inputs, outputs, frameCount, streamTime);
}

