#ifndef ALSAAUDIODEVICE_H
#define ALSAAUDIODEVICE_H

#ifdef __LINUX_ALSA__

#include <pthread.h>

class AlsaAudioDevice : public AudioDevice
{
public:
  AlsaAudioDevice(const QString& deviceName);
  virtual ~AlsaAudioDevice();
  virtual bool open(const int bitDepth, const double sampleRate, const int blockSize);
  virtual void close();
  virtual void start();
  virtual void stop();

private:
  static void* audioFunc(void* arg);
  void audioThreadFunc();
  void* m_alsaHandle;
  pthread_t m_audioThread;
  bool m_audioThreadStopped;
  bool m_audioMuted;
};

#endif

#endif // ALSAAUDIODEVICE_H
