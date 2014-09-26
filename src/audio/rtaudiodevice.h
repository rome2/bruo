#ifndef RTAUDIODEVICE_H
#define RTAUDIODEVICE_H

class RtAudioDevice :
  public AudioDevice
{
public:
  RtAudioDevice();
  virtual ~RtAudioDevice();
  virtual bool open(const int bitDepth, const double sampleRate, const int blockSize);
  virtual void close();
  virtual void start();
  virtual void stop();

private:
  static int rt_callback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, unsigned int status, void* userData);
  void callback(const double* inBuffer, double* outBuffer, unsigned int frameCount);

  class RtAudio* m_rad;
  SampleBuffer m_buffer;
};

#endif // RTAUDIODEVICE_H
