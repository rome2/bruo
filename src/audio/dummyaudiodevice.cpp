#include "bruo.h"
#include "audiodevice.h"
#include "audiosystem.h"
#include "dummyaudiodevice.h"

DummyAudioDevice::DummyAudioDevice()
{
  // Init properties:
  m_deviceName   = "Dummy";
  m_inputCount   = 0;
  m_outputCount  = 0;
  m_bitDepth     = 64;
  m_sampleRate   = 41000;
  m_blockSize    = 256;
}

DummyAudioDevice::~DummyAudioDevice()
{
  // Nothing to do here.
}

bool DummyAudioDevice::open()
{
  // Failure is not an option:
  return true;
}

void DummyAudioDevice::close()
{
  // Nothing to do here.
}

void DummyAudioDevice::start()
{
  // Nothing to do here.
}

void DummyAudioDevice::stop()
{
  // Nothing to do here.
}
