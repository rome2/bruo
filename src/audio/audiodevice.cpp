#include "bruo.h"
#include "audiodevice.h"
#include "audiosystem.h"

AudioDevice::AudioDevice() :
  m_deviceName("Unknown"),
  m_bitDepth(0),
  m_sampleRate(0.0),
  m_blockSize(0),
  m_inputCount(0),
  m_outputCount(0)
{
}

AudioDevice::~AudioDevice()
{
}

const QString& AudioDevice::deviceName() const
{
  return m_deviceName;
}

int AudioDevice::bitDepth() const
{
  return m_bitDepth;
}

double AudioDevice::sampleRate() const
{
  return m_sampleRate;
}

int AudioDevice::blockSize() const
{
  return m_blockSize;
}

int AudioDevice::inputCount() const
{
  return m_inputCount;
}

int AudioDevice::outputCount() const
{
  return m_outputCount;
}
