#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include "samplebuffer.h"
#include "math.h"

class AudioDevice
{
public:
  AudioDevice(const QString& deviceName) :
    m_deviceName(deviceName),
    m_bitDepth(0),
    m_sampleRate(0.0),
    m_blockSize(0),
    m_channelCount(0)
  {
  }

  virtual ~AudioDevice()
  {
  }

  virtual bool open(const int bitDepth, const double sampleRate, const int blockSize) = 0;
  virtual void close() = 0;
  virtual void start() = 0;
  virtual void stop() = 0;

  const QString& deviceName() const { return m_deviceName; }
  int bitDepth() const { return m_bitDepth; }
  double sampleRate() const { return m_sampleRate; }
  int blockSize() const { return m_blockSize; }
  int channelCount() const { return m_channelCount; }

protected:
  QString m_deviceName;
  unsigned int m_bitDepth;
  double m_sampleRate;
  unsigned int m_blockSize;
  unsigned int m_channelCount;
};

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

class AudioSystem
{
public:

  static void processAudio(AudioDevice* device, SampleBuffer& buffer)
  {
    //return;
    double max_phase = 6.283185307;
    double freq = 440.0;
    double step = max_phase * freq / device->sampleRate();
    static double omega = 0.0;

    double* chan1 = buffer.sampleBuffer(0);
    for (int j = 0; j < buffer.sampleCount(); j++)
    {
      omega += step;
      if (omega > max_phase)
        omega -= max_phase;
      chan1[j] = sin(omega);
    }

    for (int i = 1; i < buffer.channelCount(); i++)
    {
      double* chan = buffer.sampleBuffer(i);
      for (int j = 0; j < buffer.sampleCount(); j++)
        chan[j] = chan1[j];
    }
  }

  static bool start()
  {

#ifdef __LINUX_ALSA__
    m_device = new AlsaAudioDevice("default");
    //m_device = new AudioDevice("default");
    m_device->open(16, 44100.0, 1024);
    m_device->start();
#endif
    return true;
  }

  static void stop()
  {
    m_device->close();
    delete m_device;
    m_device = 0;
  }

private:

  AudioSystem()
  {
  }

  ~AudioSystem()
  {
    if (m_device != 0)
    {
      m_device->close();
      delete m_device;
      m_device = 0;
    }
  }

  static AudioDevice* m_device;
};

#endif // AUDIODEVICE_H
