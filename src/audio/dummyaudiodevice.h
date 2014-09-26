#ifndef __DUMMYAUDIODEVICE_H_INCLUDED__
#define __DUMMYAUDIODEVICE_H_INCLUDED__

class DummyAudioDevice :
  public AudioDevice
{
public:
  DummyAudioDevice();
  virtual ~DummyAudioDevice();
  virtual bool open(const int bitDepth, const double sampleRate, const int blockSize);
  virtual void close();
  virtual void start();
  virtual void stop();
};

#endif // #ifndef __DUMMYAUDIODEVICE_H_INCLUDED__
