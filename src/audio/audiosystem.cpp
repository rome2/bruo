#include "bruo.h"
#include "audiodevice.h"
#include "rtaudiodevice.h"
#include "audiosystem.h"

AudioDevice* AudioSystem::m_device = 0;
DocumentManager* AudioSystem::m_docMan;

void AudioSystem::initialize(DocumentManager* docMan)
{
  // Save document manager:
  m_docMan = docMan;

  // Init port audio:
//  PaError err = Pa_Initialize();
//  if (err != paNoError)
//    qDebug() << "Port audio initialization failure: " << Pa_GetErrorText(err);
}

void AudioSystem::finalize()
{
  // Free port audio:
//  PaError err = Pa_Terminate();
//  if (err != paNoError)
//    qDebug() << "Port audio termination failure: " << Pa_GetErrorText(err);
}

void AudioSystem::processAudio(const SampleBuffer& /* inputBuffer */, SampleBuffer& outputBuffer, const int sampleFrames)
{
  // Get current document:
  if (m_docMan == 0)
    return;
  Document* doc = m_docMan->activeDocument();
  if (doc == 0)
    return;

  outputBuffer.makeSilence();
  if (doc->playing())
  {
    doc->readSamples(doc->cursorPosition(), outputBuffer, sampleFrames);
    doc->setCursorPosition(doc->cursorPosition() + sampleFrames);
    if (doc->cursorPosition() >= doc->sampleCount())
      doc->setPlaying(false);
  }
}

bool AudioSystem::start()
{
  // Already got a device?
  if (m_device != 0)
    return false;

  m_device = new RtAudioDevice();
  if (m_device->open())
    m_device->start();

  return true;
}

void AudioSystem::stop()
{
  // Got a device?
  if (m_device == 0)
    return;

  // Close and free the device:
  m_device->close();
  delete m_device;
  m_device = 0;
}

AudioSystem::AudioSystem()
{
  // Nothing to do here.
}

AudioSystem::AudioSystem(const AudioSystem&)
{
  // Nothing to do here.
}

AudioSystem::~AudioSystem()
{
  // Still got a device?
  if (m_device != 0)
  {
    // Close and free the device:
    m_device->close();
    delete m_device;
    m_device = 0;
  }
}

void AudioSystem::operator = (const AudioSystem&)
{
  // Nothing to do here.
}
