#include "bruo.h"
#include "audiodevice.h"
#include "audiosystem.h"
#include "dummyaudiodevice.h"

DummyAudioDevice::DummyAudioDevice() :
  AudioDevice("Dummy")
{
  // Nothing to do here.
}

DummyAudioDevice::~DummyAudioDevice()
{
  // Nothing to do here.
}

bool DummyAudioDevice::open(const int bitDepth, const double sampleRate, const int blockSize)
{
  // Update properties:
  m_channelCount = 2;
  m_bitDepth     = bitDepth;
  m_sampleRate   = sampleRate;
  m_blockSize    = blockSize;

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
