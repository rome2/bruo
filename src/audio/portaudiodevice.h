#ifndef PORTAUDIODEVICE_H
#define PORTAUDIODEVICE_H

#include <portaudio.h>

class PortAudioDevice : public AudioDevice
{
public:
  PortAudioDevice(const QString& deviceName);
  virtual ~PortAudioDevice();
  virtual bool open(const int bitDepth, const double sampleRate, const int blockSize);
  virtual void close();
  virtual void start();
  virtual void stop();

private:
  static int paCallback(void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, PaTimestamp timeInfo, void* userData);
  void callback(float* inBuffer, float* outBuffer, int frameCount);

  PaStream* m_stream;
  SampleBuffer m_buffer;
};

#endif // PORTAUDIODEVICE_H
