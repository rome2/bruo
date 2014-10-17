#include "bruo.h"
#include "audiosystem.h"
#include "settings/loggingsystem.h"

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

DocumentManager* AudioSystem::m_docMan;
RtAudio* AudioSystem::m_rad = 0;
SampleBuffer AudioSystem::m_inputBuffer;
SampleBuffer AudioSystem::m_outputBuffer;
int AudioSystem::m_deviceID = -1;
int AudioSystem::m_bufferCount = 3;
int AudioSystem::m_apiID = RtAudio::UNSPECIFIED;
QString AudioSystem::m_deviceName("");
int AudioSystem::m_bitDepth = 64;
int AudioSystem::m_sampleRate = 41000;
int AudioSystem::m_blockSize = 512;
int AudioSystem::m_inputCount = 2;
int AudioSystem::m_outputCount = 2;
bool AudioSystem::m_error = false;

void AudioSystem::initialize(DocumentManager* docMan)
{
  // Save document manager:
  m_docMan = docMan;
}

void AudioSystem::finalize()
{
}

bool AudioSystem::start()
{
  // Close device (just to be sure):
  stop();

  // Reset error flag:
  m_error = false;

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

  // Get samplerate and buffer size:
  if (settings.contains("audiosystem/sample_rate"))
    m_sampleRate = settings.value("audiosystem/sample_rate").toInt();
  if (settings.contains("audiosystem/buffer_size"))
    m_blockSize = settings.value("audiosystem/buffer_size").toInt();

  try
  {
    unsigned int b = m_blockSize;
    m_rad->openStream(&outParams, /*&inParams*/0, RTAUDIO_FLOAT64, m_sampleRate, &b, &rt_callback, 0, &options, &err_callback);
    m_blockSize = b;
  }
  catch (RtAudioError& e)
  {
    QString es(QString(e.getMessage().c_str()));
    LoggingSystem::logMessageAsync(es);
    delete m_rad;
    m_rad = 0;
    m_error = true;
    return false;
  }

  // Safe properties:
  m_deviceName  = info.name.c_str();
  m_inputCount  = 0;//inParams.nChannels;
  m_outputCount = outParams.nChannels;
  m_bitDepth    = 64;
  m_bufferCount = options.numberOfBuffers;

  // Create buffers to pass to the engine:
  m_inputBuffer.createBuffers(m_inputCount,   m_blockSize);
  m_outputBuffer.createBuffers(m_outputCount, m_blockSize);

  try
  {
    if (!m_error)
      m_rad->startStream();
  }
  catch (RtAudioError& e)
  {
    QString es(QString(e.getMessage().c_str()));
    LoggingSystem::logMessageAsync(es);
    delete m_rad;
    m_rad = 0;
    m_error = true;
    return false;
  }

  // Return success:
  return true;
}

void AudioSystem::stop()
{
  if (m_rad == 0)
    return;

  try
  {
    if (!m_error)
    {
      m_rad->stopStream();
      if (m_rad->isStreamOpen())
        m_rad->closeStream();
    }
  }
  catch (RtAudioError& e)
  {
    QString es(QString(e.getMessage().c_str()));
    LoggingSystem::logMessageAsync(es);
    m_error = true;
  }

  delete m_rad;
  m_rad = 0;
}

AudioSystem::AudioSystem()
{
  // Nothing to do here.
}

AudioSystem::AudioSystem(const AudioSystem&)
{
  // Nothing to do here.
}

AudioSystem::~AudioSystem()
{
  // Nothing to do here.
}

void AudioSystem::operator = (const AudioSystem&)
{
  // Nothing to do here.
}

void AudioSystem::err_callback(RtAudioError::Type type, const std::string& errorText)
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
    m_error = true;
    break;
  case RtAudioError::NO_DEVICES_FOUND:
    s_type += "no devices found error";
    m_error = true;
    break;
  case RtAudioError::INVALID_DEVICE:
    s_type += "invalid device error";
    m_error = true;
    break;
  case RtAudioError::MEMORY_ERROR:
    s_type += "memory error";
    m_error = true;
    break;
  case RtAudioError::INVALID_PARAMETER:
    s_type += "invalid parameter error";
    m_error = true;
    break;
  case RtAudioError::INVALID_USE:
    s_type += "invalid use error";
    m_error = true;
    break;
  case RtAudioError::DRIVER_ERROR:
    s_type += "driver error";
    m_error = true;
    break;
  case RtAudioError::SYSTEM_ERROR:
    s_type += "system error";
    m_error = true;
    break;
  case RtAudioError::THREAD_ERROR:
    s_type += "thread error";
    m_error = true;
    break;
  default:
    s_type += "unknown error";
    m_error = true;
    break;
  }
  s_type += ": ";
  s_type += errorText.c_str();

  // Log message:
  LoggingSystem::logMessageAsync(s_type);
}

int AudioSystem::rt_callback(void* outBuffer, void* inBuffer, unsigned int frameCount, double streamTime, unsigned int /* status */, void* /* userData */)
{
  // Error checking:
  if (m_error)
    return 2;

  // Get current document:
  if (m_docMan == 0)
    return 0;
  Document* doc = m_docMan->activeDocument();
  if (doc == 0)
    return 0;

  // Copy input data:
  double* in = static_cast<double*>(inBuffer);
  for (int i = 0; i < m_inputCount; i++, in += frameCount)
    memcpy(m_inputBuffer.sampleBuffer(i), in, frameCount * sizeof(double));

  // Get samples:
  doc->rack().process(m_inputBuffer, m_outputBuffer, frameCount, streamTime);

  // Copy output data:
  double* out = static_cast<double*>(outBuffer);
  for (int j = 0; j < m_outputCount; j++, out += frameCount)
    memcpy(out, m_outputBuffer.sampleBuffer(j), frameCount * sizeof(double));

  // Return success:
  return 0;
}
