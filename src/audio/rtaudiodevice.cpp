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
  m_rad(0),
  m_deviceID(-1),
  m_bufferCount(3),
  m_apiID(RtAudio::UNSPECIFIED)
{
}

RtAudioDevice::~RtAudioDevice()
{
  if (m_rad != 0)
    delete m_rad;
  m_rad = 0;
}

void err_callback(RtAudioError::Type type, const std::string& errorText)
{
  QString s_type("Audio system ");
  switch (type)
  {
  case RtAudioError::WARNING:
    s_type += "warning";
    break;
  case RtAudioError::DEBUG_WARNING:
    s_type += "debug warning";
    break;
  case RtAudioError::UNSPECIFIED:
    s_type += "unspecified error";
    break;
  case RtAudioError::NO_DEVICES_FOUND:
    s_type += "no devices found error";
    break;
  case RtAudioError::INVALID_DEVICE:
    s_type += "invalid device error";
    break;
  case RtAudioError::MEMORY_ERROR:
    s_type += "memory error";
    break;
  case RtAudioError::INVALID_PARAMETER:
    s_type += "invalid parameter error";
    break;
  case RtAudioError::INVALID_USE:
    s_type += "invalid use error";
    break;
  case RtAudioError::DRIVER_ERROR:
    s_type += "driver error";
    break;
  case RtAudioError::SYSTEM_ERROR:
    s_type += "system error";
    break;
  case RtAudioError::THREAD_ERROR:
    s_type += "thread error";
    break;
  default:
    s_type += "unknown error";
    break;
  }

  qDebug() << s_type << ": " << errorText.c_str();
}

bool RtAudioDevice::open(const double sampleRate, const int blockSize)
{
  // Close device (just to be sure):
  close();

  QSettings settings;

  // Get API to use:
  if (settings.contains("audiosystem/rt_api"))
    m_apiID = settings.value("audiosystem/rt_api").toInt();

  // Create audio system:
  m_rad = new RtAudio(static_cast<RtAudio::Api>(m_apiID));

  // Get device ID:
  if (settings.contains("audiosystem/rt_device_id"))
    m_deviceID = settings.value("audiosystem/rt_device_id").toInt();
  else
    m_deviceID = m_rad->getDefaultOutputDevice();

  // Get device properties:
  RtAudio::DeviceInfo info = m_rad->getDeviceInfo(m_deviceID);

  // Fill I/O parameters:
  RtAudio::StreamParameters inParams, outParams;
  outParams.deviceId     = m_deviceID;
  outParams.nChannels    = info.outputChannels;
  outParams.firstChannel = 0;
  inParams.deviceId      = m_deviceID;
  inParams.nChannels     = info.inputChannels;
  inParams.firstChannel  = 0;
  unsigned int bufferFrames = blockSize;

  // Fill options:
  RtAudio::StreamOptions options;
  if (settings.contains("audiosystem/rt_buffer_cnt"))
    m_bufferCount = settings.value("audiosystem/rt_buffer_cnt").toInt();
  options.numberOfBuffers = m_bufferCount;
  options.flags = RTAUDIO_NONINTERLEAVED;
  if (settings.contains("audiosystem/rt_realtime") && settings.value("audiosystem/rt_realtime").toBool())
    options.flags |= RTAUDIO_SCHEDULE_REALTIME;
  if (settings.contains("audiosystem/rt_exclusive") && settings.value("audiosystem/rt_exclusive").toBool())
    options.flags |= RTAUDIO_HOG_DEVICE;
  if (settings.contains("audiosystem/rt_min_latency") && settings.value("audiosystem/rt_min_latency").toBool())
    options.flags |= RTAUDIO_MINIMIZE_LATENCY;
  options.streamName = "bruo";

  try
  {
    m_rad->openStream(&outParams, /*&inParams*/0, RTAUDIO_FLOAT64, static_cast<unsigned int>(sampleRate), &bufferFrames, &rt_callback, static_cast<void*>(this), &options, &err_callback);
  }
   catch (RtAudioError& e)
  {
    qDebug() << e.getMessage().c_str();
  }

  // Safe properties:
  m_deviceName  = info.name.c_str();
  m_inputCount  = 0;//inParams.nChannels;
  m_outputCount = outParams.nChannels;
  m_sampleRate  = sampleRate;
  m_blockSize   = bufferFrames;
  m_bitDepth    = 64;
  m_bufferCount = options.numberOfBuffers;

  // Create buffers to pass to the engine:
  m_inputBuffer.createBuffers(m_inputCount, m_blockSize);
  m_outputBuffer.createBuffers(m_outputCount, m_blockSize);

  // Return success:
  return true;
}

void RtAudioDevice::close()
{
  if (m_rad == 0)
    return;

  if (m_rad->isStreamOpen())
    m_rad->closeStream();

  delete m_rad;
  m_rad = 0;
}

void RtAudioDevice::start()
{
  if (m_rad == 0)
    return;

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
  if (m_rad == 0)
    return;

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

void RtAudioDevice::callback(const double* inBuffer, double* outBuffer, unsigned int frameCount)
{
  // Copy input data:
  for (unsigned int i = 0; i < m_inputCount; i++, inBuffer += frameCount)
    memcpy(m_inputBuffer.sampleBuffer(i), inBuffer, frameCount * sizeof(double));

  // Process data:
  AudioSystem::processAudio(m_inputBuffer, m_outputBuffer, frameCount);

  // Copy output data:
  for (unsigned int j = 0; j < m_outputCount; j++, outBuffer += frameCount)
    memcpy(outBuffer, m_outputBuffer.sampleBuffer(j), frameCount * sizeof(double));
}
