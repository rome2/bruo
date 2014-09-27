#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include "samplebuffer.h"

class AudioDevice
{
public:
  AudioDevice();
  virtual ~AudioDevice();

  virtual bool open(const double sampleRate, const int blockSize) = 0;
  virtual void close() = 0;
  virtual void start() = 0;
  virtual void stop() = 0;

  const QString& deviceName() const;
  int bitDepth() const;
  double sampleRate() const;
  int blockSize() const;
  int inputCount() const;
  int outputCount() const;

protected:
  QString m_deviceName;
  unsigned int m_bitDepth;
  double m_sampleRate;
  unsigned int m_blockSize;
  unsigned int m_inputCount;
  unsigned int m_outputCount;
};

#endif // AUDIODEVICE_H
