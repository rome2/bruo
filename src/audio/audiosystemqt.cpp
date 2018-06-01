#include "bruo.h"
#include "audiosystemqt.h"
#include "settings/loggingsystem.h"

Generator::Generator(DocumentManager* docMan, const QAudioFormat& format, int blockSize, QObject* parent) :
  QIODevice(parent),
  m_docMan(docMan),
  m_format(format),
  m_blockSize(blockSize),
  m_pos(0)
{
  m_inputBuffer.createBuffers(2, m_blockSize);
  m_outputBuffer.createBuffers(2, m_blockSize);
  qint64 length = (format.channelCount() * (format.sampleSize() / 8)) * m_blockSize;
  m_buffer.resize(length);
  m_pos = length + 1;
}

Generator::~Generator()
{
}

void Generator::start()
{
  open(QIODevice::ReadOnly);
}

void Generator::stop()
{
  close();
}

void Generator::fillOutputBuffer()
{
  // Get current document:
  if (m_docMan == 0)
    return;
  Document* doc = m_docMan->activeDocument();
  if (doc == 0)
    return;

  // Init input data:
  m_inputBuffer.makeSilence();
  m_outputBuffer.makeSilence();

  // Get samples:
  doc->rack().process(m_inputBuffer, m_outputBuffer, m_blockSize, 0.0);

  // Copy output data:
  unsigned char* ptr = reinterpret_cast<unsigned char*>(m_buffer.data());
  for (int i = 0; i < m_blockSize; i++)
  {
    for (int j = 0; j < m_format.channelCount(); j++)
    {
      double x = m_outputBuffer.sample(qMin(doc->channelCount(), j), i);
      if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::Float)
      {
        float value = static_cast<float>(x);
        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
          qToLittleEndian<quint32>(*reinterpret_cast<quint32*>(&value), ptr);
        else
          qToBigEndian<quint32>(*reinterpret_cast<quint32*>(&value), ptr);
        ptr += 4;
      }
      else if (m_format.sampleSize() == 64 && m_format.sampleType() == QAudioFormat::Float)
      {
        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
          qToLittleEndian<quint64>(*reinterpret_cast<quint64*>(&x), ptr);
        else
          qToBigEndian<quint64>(*reinterpret_cast<quint64*>(&x), ptr);
        ptr += 8;
      }
      else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt)
      {
        const quint8 value = static_cast<quint8>((1.0 + x) / 2 * 255);
        *reinterpret_cast<quint8*>(ptr) = value;
        ptr++;
      }
      else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt)
      {
        const qint8 value = static_cast<qint8>(x * 127);
        *reinterpret_cast<quint8*>(ptr) = value;
        ptr++;
      }
      else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt)
      {
        quint16 value = static_cast<quint16>((1.0 + x) / 2 * 65535);
        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
          qToLittleEndian<quint16>(value, ptr);
        else
          qToBigEndian<quint16>(value, ptr);
        ptr += 2;
      }
      else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt)
      {
        qint16 value = static_cast<qint16>(x * 32767);
        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
          qToLittleEndian<qint16>(value, ptr);
        else
          qToBigEndian<qint16>(value, ptr);
        ptr += 2;
      }
      else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::UnSignedInt)
      {
        quint32 value = static_cast<quint32>((1.0 + x) * 0.5 * (1 << 31));
        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
          qToLittleEndian<quint32>(value, ptr);
        else
          qToBigEndian<quint32>(value, ptr);
        ptr += 4;
      }
      else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::SignedInt)
      {
        qint32 value = static_cast<qint32>(x * (1 << 31));
        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
          qToLittleEndian<qint32>(value, ptr);
        else
          qToBigEndian<qint32>(value, ptr);
        ptr += 4;
      }
    }
  }
}

qint64 Generator::readData(char* data, qint64 len)
{
  if (m_buffer.isEmpty())
    return 0;

  qint64 total = 0;
  while (len - total > 0)
  {
    if (m_pos >= m_buffer.size())
    {
      fillOutputBuffer();
      m_pos = 0;
    }

    const qint64 chunk = qMin((qint64)(m_buffer.size() - m_pos), len - total);
    memcpy(data + total, m_buffer.constData() + m_pos, chunk);
    m_pos += chunk;
    total += chunk;
  }
  return total;
}

qint64 Generator::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

qint64 Generator::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}

int Generator::blockSize() const
{
  return m_blockSize;
}

const QAudioFormat& Generator::audioFormat() const
{
  return m_format;
}

DocumentManager* AudioSystemQt::m_docMan = 0;
QMutex AudioSystemQt::m_mutex;
QAudioDeviceInfo AudioSystemQt::m_device;
Generator* AudioSystemQt::m_generator = 0;
QAudioOutput* AudioSystemQt::m_audioOutput = 0;
QIODevice* AudioSystemQt::m_output = 0; // not owned
QAudioFormat AudioSystemQt::m_format;

void AudioSystemQt::initialize(DocumentManager* docMan)
{
  // Save document manager:
  m_docMan = docMan;

  m_format.setSampleRate(44100);
  m_format.setChannelCount(2);
  m_format.setSampleSize(32);
  m_format.setCodec("audio/pcm");
  m_format.setByteOrder(QAudioFormat::LittleEndian);
  m_format.setSampleType(QAudioFormat::SignedInt);

  m_device = QAudioDeviceInfo::defaultOutputDevice();
  QAudioDeviceInfo info(m_device);
  if (!info.isFormatSupported(m_format)) {
    qWarning() << "Default audio format not supported - trying to use nearest";
    m_format = info.nearestFormat(m_format);
  }
  qInfo() << "Using audio format: ";
  qInfo() << " Codec:        " << m_format.codec();
  qInfo() << " Channels:     " << m_format.channelCount();
  qInfo() << " Samplerate:   " <<  m_format.sampleRate() << "Hz";
  qInfo() << " Sample format:" << (m_format.sampleType() == QAudioFormat::Float ? "float" : (m_format.sampleType() == QAudioFormat::UnSignedInt ? "unsigned int" : "int"));
  qInfo() << " Bit depth:    " << m_format.sampleSize() << "bit" << (m_format.byteOrder() == QAudioFormat::BigEndian ? "big endian" : "little endian");

  if (m_generator)
    delete m_generator;
  m_generator = new Generator(m_docMan, m_format, 4096, 0);
}

void AudioSystemQt::finalize()
{
  stop();
  if (m_generator)
    delete m_generator;
  m_generator = 0;
  m_device = QAudioDeviceInfo();
}

bool AudioSystemQt::probeCurrentDevice()
{
  // Lock access:
  QMutexLocker locker(&m_mutex);

  try
  {
    return true;
  }
  catch (...)
  {
  }
  return false;
}

bool AudioSystemQt::start()
{
  // Close device (just to be sure):
  stop();

  // Lock access:
  QMutexLocker locker(&m_mutex);

  try
  {
    m_audioOutput = new QAudioOutput(m_device, m_format, 0);
    qint64 length = (m_format.channelCount() * (m_format.sampleSize() / 8)) * m_generator->blockSize();
    m_audioOutput->setBufferSize(length);
    m_generator->start();
    m_audioOutput->start(m_generator);
  }
  catch (...)
  {
    return false;
  }

  // Return success:
  return true;
}

void AudioSystemQt::stop()
{
  // Lock access:
  QMutexLocker locker(&m_mutex);

  try
  {
    if (m_generator)
      m_generator->stop();
    if (m_audioOutput)
    {
      m_audioOutput->stop();
      delete m_audioOutput;
      m_audioOutput = 0;
    }
  }
  catch (...)
  {
  }
}

void AudioSystemQt::suspend()
{
  // Lock access:
  QMutexLocker locker(&m_mutex);

  try
  {
    if (m_audioOutput->state() == QAudio::SuspendedState || m_audioOutput->state() == QAudio::StoppedState)
      return;
    m_audioOutput->suspend();
  }
  catch (...)
  {
  }
}

void AudioSystemQt::resume()
{
  // Lock access:
  QMutexLocker locker(&m_mutex);

  try
  {
    if (m_audioOutput->state() == QAudio::SuspendedState || m_audioOutput->state() == QAudio::StoppedState)
      m_audioOutput->resume();
  }
  catch (...)
  {
  }
}

double AudioSystemQt::sampleRate()
{
  if (m_generator)
    return m_generator->audioFormat().sampleRate();
  return 0.0;
}

int AudioSystemQt::blockSize()
{
  if (m_generator)
    return m_generator->blockSize();
  return 0;
}

AudioSystemQt::AudioSystemQt()
{
}

AudioSystemQt::AudioSystemQt(const AudioSystemQt&)
{
}

void AudioSystemQt::operator = (const AudioSystemQt&)
{
}

