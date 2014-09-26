#include "bruo.h"
#include "audiodevice.h"
#include "audiosystem.h"
#include "rtaudiodevice.h"
#include "../../rtaudio/RtAudio.h"

#ifdef __WINDOWS_ASIO__
#undef DEFINE_GUID
#define DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) EXTERN_C const GUID DECLSPEC_SELECTANY name = { l,w1,w2,{ b1,b2,b3,b4,b5,b6,b7,b8 } }
DEFINE_GUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT,0x3,0,0x10,0x80,0,0,0xaa,0,0x38,0x9b,0x71);
DEFINE_GUID(KSDATAFORMAT_SUBTYPE_PCM,0x1,0,0x10,0x80,0,0,0xaa,0,0x38,0x9b,0x71);
#include "../../rtaudio/include/asio.cpp"
#include "../../rtaudio/include/asiodrivers.cpp"
#include "../../rtaudio/include/asiolist.cpp"
#include "../../rtaudio/include/iasiothiscallresolver.cpp"
#endif

RtAudioDevice::RtAudioDevice() :
  AudioDevice("Dummy"),
  m_rad(0)
{
  m_rad = new RtAudio();
}

RtAudioDevice::~RtAudioDevice()
{
  delete m_rad;
}

bool RtAudioDevice::open(const int bitDepth, const double sampleRateD, const int blockSize)
{
  RtAudio::StreamParameters parameters;
  parameters.deviceId = m_rad->getDefaultOutputDevice();
  parameters.nChannels = 2;
  parameters.firstChannel = 0;
  unsigned int sampleRate = (int)sampleRateD;
  unsigned int bufferFrames = blockSize; // 256 sample frames
  try
  {
    m_rad->openStream(&parameters, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &rt_callback, static_cast<void*>(this));
  }
   catch (RtAudioError& e)
  {
    qDebug() << e.getMessage().c_str();
  }

  m_channelCount = 2;
  m_bitDepth     = bitDepth;
  m_sampleRate   = sampleRate;
  m_blockSize    = bufferFrames;

  // Create buffer to pass to the engine:
  m_buffer.createBuffers(m_channelCount, m_blockSize);

  // Return success:
  return true;
}

void RtAudioDevice::close()
{
  if (m_rad->isStreamOpen())
    m_rad->closeStream();
}

void RtAudioDevice::start()
{
  try
  {
    m_rad->startStream();
  }
  catch (RtAudioError& e)
  {
    qDebug() << e.getMessage().c_str();
  }
}

void RtAudioDevice::stop()
{
  try
  {
    m_rad->stopStream();
  }
  catch (RtAudioError& e)
  {
    qDebug() << e.getMessage().c_str();
  }
}

int RtAudioDevice::rt_callback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double /*streamTime */, RtAudioStreamStatus /* status */, void* userData)
{
  // Cast data passed through stream to our device:
  RtAudioDevice* device = (RtAudioDevice*)userData;

  // Process audio:
  device->callback(static_cast<const double*>(inputBuffer), static_cast<double*>(outputBuffer), nBufferFrames);

  // Return success:
  return 0;
}

void RtAudioDevice::callback(const double* /* inBuffer */, double* outBuffer, unsigned int frameCount)
{
  // Get data:
  AudioSystem::processAudio(this, m_buffer);

  // Copy and interlace data:
  for (unsigned int i = 0; i < frameCount; i++)
  {
    for (unsigned int j = 0; j < m_channelCount; j++)
      *outBuffer++ = m_buffer.sample(j, i);
  }
}
