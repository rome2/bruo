#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "../documentmanager.h"
#include "../../rtaudio/RtAudio.h"

class AudioSystem
{
public:

  AudioSystem(DocumentManager* docMan);
  virtual ~AudioSystem();
  void initialize();
  void finalize();
  bool probeCurrentDevice();
  bool start();
  void stop();
  void suspend();
  void resume();

private:

  static int rt_callback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, unsigned int status, void* userData);
  static void err_callback(RtAudioError::Type type, const std::string& errorText);

  DocumentManager* m_docMan;
  RtAudio* m_rad;
  SampleBuffer m_inputBuffer;
  SampleBuffer m_outputBuffer;
  int m_deviceID;
  int m_bufferCount;
  int m_apiID;
  QString m_deviceName;
  int m_bitDepth;
  int m_sampleRate;
  int m_blockSize;
  int m_inputCount;
  int m_outputCount;
  static bool m_error;
  bool m_suspended;
  QMutex m_mutex;
};

class AudioSuspender
{
public:
  AudioSuspender(AudioSystem* asys) : m_asys(asys)
  {
    if (m_asys)
      m_asys->suspend();
  }

  ~AudioSuspender()
  {
    if (m_asys)
      m_asys->resume();
    m_asys = 0;
  }

private:
  AudioSystem* m_asys;
};

#endif // AUDIOSYSTEM_H
