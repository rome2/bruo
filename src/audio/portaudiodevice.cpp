#include "bruo.h"
#include "audiodevice.h"
#include "audiosystem.h"
#include "portaudiodevice.h"

class AudioConfig
{
public:
  QString m_name;
  QString m_api;
  int m_deviceIndex;
  int m_flags;
  int m_channels;
  int m_bitDepth;
  double m_sampleRate;
  double m_blockSize;
  int m_blockCount;
};

PortAudioDevice::PortAudioDevice(const QString& deviceName) :
  AudioDevice(deviceName),
  m_stream(0)
{
}

PortAudioDevice::~PortAudioDevice()
{
  // Clear audio device:
  PortAudioDevice::close();
}

int PortAudioDevice::paCallback(void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, PaTimestamp /*timeInfo */, void* userData)
{
  // Cast data passed through stream to our device:
  PortAudioDevice* device = (PortAudioDevice*)userData;

  // Process audio:
  device->callback(static_cast<float*>(inputBuffer), static_cast<float*>(outputBuffer), static_cast<int>(framesPerBuffer));

  // Return success:
  return 0;
}

void PortAudioDevice::callback(float* /* inBuffer */, float* outBuffer, int frameCount)
{
  // Get data:
  AudioSystem::processAudio(this, m_buffer);

  // Copy and interlace data:
  for (int i = 0; i < frameCount; i++)
  {
    for (unsigned int j = 0; j < m_channelCount; j++)
      *outBuffer++ = (float)m_buffer.sample(j, i);
  }
}

bool PortAudioDevice::open(const int bitDepth, const double sampleRate, const int blockSize)
{
  // Still open?
  if (m_stream != 0)
    close();

  // Open an audio I/O stream:
  PaError err = Pa_OpenDefaultStream(&m_stream, 0, 2, paFloat32, sampleRate, blockSize, 3, paCallback, this);
  if (err != paNoError)
    qDebug() << "Port audio start stream failure: " << Pa_GetErrorText(err);

  // Save config:
  m_channelCount = 2;
  m_bitDepth     = bitDepth;
  m_sampleRate   = sampleRate;
  m_blockSize    = blockSize;

  // Create buffer to pass to the engine:
  m_buffer.createBuffers(m_channelCount, m_blockSize);

  // Return success:
  return true;
}

void PortAudioDevice::close()
{
  if (m_stream == 0)
    return;

  PaError err = Pa_CloseStream(m_stream);
  if (err != paNoError)
    qDebug() << "Port audio start stream failure: " << Pa_GetErrorText(err);
  m_stream = 0;
}

void PortAudioDevice::start()
{
  if (m_stream == 0)
    return;

  PaError err = Pa_StartStream(m_stream);
  if (err != paNoError)
    qDebug() << "Port audio start stream failure: " << Pa_GetErrorText(err);
}

void PortAudioDevice::stop()
{
  if (m_stream == 0)
    return;

  PaError err = Pa_StopStream(m_stream);
  if (err != paNoError)
    qDebug() << "Port audio stop stream failure: " << Pa_GetErrorText(err);
}
