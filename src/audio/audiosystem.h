#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "../documentmanager.h"

class AudioSystem
{
public:

  static void initialize(DocumentManager* docMan);
  static void finalize();
  static void processAudio(const SampleBuffer& inputBuffer, SampleBuffer& outputBuffer, const int sampleFrames);
  static bool start();
  static void stop();

private:

  AudioSystem();
  AudioSystem(const AudioSystem&);
  ~AudioSystem();
  void operator = (const AudioSystem&);

  static AudioDevice* m_device;
  static DocumentManager* m_docMan;
};

#endif // AUDIOSYSTEM_H
