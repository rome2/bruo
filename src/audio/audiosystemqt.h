#ifndef AUDIOSYSTEMQT_H
#define AUDIOSYSTEMQT_H

#include "../documentmanager.h"
#include <QAudioOutput>
#include <QIODevice>

class Generator : public QIODevice
{
    Q_OBJECT

public:
    Generator(DocumentManager* docMan, const QAudioFormat &format, int blockSize, QObject *parent);
    virtual ~Generator();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;

    int blockSize() const;
    const QAudioFormat& audioFormat() const;

private:
    void fillOutputBuffer();

    DocumentManager* m_docMan;
    QAudioFormat m_format;
    SampleBuffer m_inputBuffer;
    SampleBuffer m_outputBuffer;
    int m_blockSize;
    QByteArray m_buffer;
    int m_pos;
};

class AudioSystemQt
{
public:
  static void initialize(DocumentManager* docMan);
  static void finalize();
  static bool probeCurrentDevice();
  static bool start();
  static void stop();
  static void suspend();
  static void resume();
  static double sampleRate();
  static int blockSize();

private:

  AudioSystemQt();
  AudioSystemQt(const AudioSystemQt&);
  void operator = (const AudioSystemQt&);

  static DocumentManager* m_docMan;
  static QMutex m_mutex;
  static QAudioDeviceInfo m_device;
  static Generator* m_generator;
  static QAudioOutput* m_audioOutput;
  static QIODevice* m_output; // not owned
  static QAudioFormat m_format;
};

class AudioSuspenderQt
{
public:
  AudioSuspenderQt(AudioSystemQt* asys) : m_asys(asys)
  {
    if (m_asys)
      m_asys->suspend();
  }

  ~AudioSuspenderQt()
  {
    if (m_asys)
      m_asys->resume();
    m_asys = 0;
  }

private:
  AudioSystemQt* m_asys;
};

#endif // AUDIOSYSTEMQT_H
