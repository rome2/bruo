#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include "samplebuffer.h"

class AudioDevice
{
public:
  AudioDevice();
  virtual ~AudioDevice();

  virtual bool open() = 0;
  virtual void close() = 0;
  virtual void start() = 0;
  virtual void stop() = 0;

  const QString& deviceName() const;
  int bitDepth() const;
  int sampleRate() const;
  int blockSize() const;
  int inputCount() const;
  int outputCount() const;

protected:
  QString m_deviceName;
  int m_bitDepth;
  int m_sampleRate;
  int m_blockSize;
  int m_inputCount;
  int m_outputCount;
};

#endif // AUDIODEVICE_H
