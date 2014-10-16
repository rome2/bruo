#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "../documentmanager.h"
#include "../../rtaudio/RtAudio.h"

class AudioSystem
{
public:

  static void initialize(DocumentManager* docMan);
  static void finalize();
  static bool start();
  static void stop();

private:

  AudioSystem();
  AudioSystem(const AudioSystem&);
  ~AudioSystem();
  void operator = (const AudioSystem&);

  static int rt_callback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, unsigned int status, void* userData);
  static void err_callback(RtAudioError::Type type, const std::string& errorText);

  static DocumentManager* m_docMan;
  static RtAudio* m_rad;
  static SampleBuffer m_inputBuffer;
  static SampleBuffer m_outputBuffer;
  static int m_deviceID;
  static int m_bufferCount;
  static int m_apiID;
  static QString m_deviceName;
  static int m_bitDepth;
  static int m_sampleRate;
  static int m_blockSize;
  static int m_inputCount;
  static int m_outputCount;
  static bool m_error;
};

#endif // AUDIOSYSTEM_H
