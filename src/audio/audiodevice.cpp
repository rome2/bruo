#include "bruo.h"
#include "audiodevice.h"

#include <alsa/asoundlib.h>

AudioDevice* AudioSystem::m_device = 0;

AlsaAudioDevice::AlsaAudioDevice(const QString& deviceName) :
  AudioDevice(deviceName),
  m_alsaHandle(NULL),
  m_audioThread(0),
  m_audioThreadStopped(false),
  m_audioMuted(true)
{
}

AlsaAudioDevice::~AlsaAudioDevice()
{
  // Clear audio device:
  AlsaAudioDevice::close();
}

bool AlsaAudioDevice::open(const int bitDepth, const double sampleRate, const int blockSize)
{
//    int next;
//    char * pt;

//    char **hints;
//    /* Enumerate sound devices */
//    int verr = snd_device_name_hint(-1, "pcm", (void***)&hints);
//    if (verr != 0)
//       return false;//Error! Just return

//    char** n = hints;
//    while (*n != NULL) {

//        char *name = snd_device_name_get_hint(*n, "NAME");

//        if (name != NULL && 0 != strcmp("null", name)) {
//          printf ("Hint name: %s\n", name);
//            //Copy name to another buffer and then free it
//            free(name);
//        }
//        n++;
//    }//End of while

//    //Free hint buffer too
//    snd_device_name_free_hint((void**)hints);

//    next = -1;
//    while (1) {
//      if (snd_card_next(&next) != 0)
//        break;		// Error
//      if (next < 0)
//        break;		// No more cards
//      if (snd_card_get_name(next, &pt) == 0) {
//        qDebug() << "Name " << next << ": " << pt;
//      }
//      if (snd_card_get_longname(next, &pt) == 0) {
//        qDebug() << "Longame " << next << ": " << pt;
//      }
//    }

  // Save values:
  m_bitDepth = bitDepth;
  m_sampleRate = sampleRate;
  m_blockSize = blockSize;

  // Open playback device:
  snd_pcm_t* pcmHandle = NULL;
  QByteArray ba = m_deviceName.toLatin1();
  int err = snd_pcm_open(&pcmHandle, ba.data(), SND_PCM_STREAM_PLAYBACK, 0);
  if (err < 0)
  {
    fprintf(stderr, "cannot open audio device %s (%s)\n", ba.data(), snd_strerror(err));
    return false;
  }

  // Allocate a hardware parameters object:
  snd_pcm_hw_params_t* params = NULL;
  snd_pcm_hw_params_alloca(&params);
  if (params == NULL)
  {
    fprintf(stderr, "cannot allocate hardware parameter structure\n");
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Initalize hardware parameter structure:
  err = snd_pcm_hw_params_any(pcmHandle, params);
  if (err < 0)
  {
    fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
    snd_pcm_hw_params_free(params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Set interleaved data packing:
  err = snd_pcm_hw_params_set_access(pcmHandle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
  if (err < 0)
  {
    fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(err));
    snd_pcm_hw_params_free(params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Set sample format, 16 bit, little endian:
  err = snd_pcm_hw_params_set_format(pcmHandle, params, SND_PCM_FORMAT_S16_LE);
  if (err < 0)
  {
    fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
    snd_pcm_hw_params_free(params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Set sample rate:
  unsigned int newSampleRate = static_cast<unsigned int>(m_sampleRate);
  err = snd_pcm_hw_params_set_rate_near(pcmHandle, params, &newSampleRate, 0);
  if (err < 0)
  {
    fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
    snd_pcm_hw_params_free(params);
    snd_pcm_close(pcmHandle);
    return false;
  }
  m_sampleRate = newSampleRate;

  // Set output to stereo:
  m_channelCount = 2;
  err = snd_pcm_hw_params_set_channels(pcmHandle, params, m_channelCount);
  if (err < 0)
  {
    fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
    snd_pcm_hw_params_free(params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  snd_pcm_uframes_t alsa_blocksize = m_blockSize * 3;
  snd_pcm_hw_params_set_buffer_size_near (pcmHandle, params, &alsa_blocksize);
  snd_pcm_uframes_t alsa_period_size = m_blockSize;
  snd_pcm_hw_params_set_period_size_near (pcmHandle, params, &alsa_period_size, NULL);
  m_blockSize = alsa_period_size;

  // Apply parameters to the hardware:
  err = snd_pcm_hw_params(pcmHandle, params);
  if (err < 0)
  {
    fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
    snd_pcm_hw_params_free(params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Create software parameter structure:
  snd_pcm_sw_params_t* sw_params = NULL;
  err = snd_pcm_sw_params_malloc(&sw_params);
  if (err < 0)
  {
    fprintf(stderr, "cannot allocate software parameters structure (%s)\n", snd_strerror(err));
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Initialize software parameter structure:
  err = snd_pcm_sw_params_current(pcmHandle, sw_params);
  if (err < 0)
  {
    fprintf(stderr, "cannot initialize software parameters structure (%s)\n", snd_strerror(err));
    snd_pcm_sw_params_free(sw_params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Set buffer size:
  err = snd_pcm_sw_params_set_avail_min(pcmHandle, sw_params, m_blockSize);
  if (err < 0)
  {
    fprintf(stderr, "cannot set minimum available count (%s)\n", snd_strerror(err));
    snd_pcm_sw_params_free(sw_params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Set start mode:
  err = snd_pcm_sw_params_set_start_threshold(pcmHandle, sw_params, alsa_blocksize - m_blockSize);
  if (err < 0)
  {
    fprintf(stderr, "cannot set start mode (%s)\n", snd_strerror(err));
    snd_pcm_sw_params_free(sw_params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Apply software parameters:
  err = snd_pcm_sw_params(pcmHandle, sw_params);
  if (err < 0)
  {
    fprintf(stderr, "cannot set software parameters (%s)\n", snd_strerror(err));
    snd_pcm_sw_params_free(sw_params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  // OK, we are ready, prepare hardware:
  err = snd_pcm_prepare(pcmHandle);
  if (err < 0)
  {
    fprintf(stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
    snd_pcm_sw_params_free(sw_params);
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Store handle for later access:
  m_alsaHandle = pcmHandle;

  // Create audio polling thread:
  if (pthread_create(&m_audioThread, 0, audioFunc, this) != 0)
  {
    fprintf(stderr, "failed to create thread for audio\n");
    snd_pcm_close(pcmHandle);
    return false;
  }

  // Return success:
  return true;
}

void AlsaAudioDevice::close()
{
  // Mute output:
  m_audioMuted = true;

  // Audio thread still active?
  if (m_audioThread != 0)
  {
    // Flag thread done:
    m_audioThreadStopped = true;

    // Join the audio thread into this one:
    pthread_join(m_audioThread, NULL);
    m_audioThread = 0;
  }

  // Anything to do?
  if (m_alsaHandle != NULL)
  {
    // Discard any pending buffers:
    snd_pcm_drain(static_cast<snd_pcm_t*>(m_alsaHandle));

    // Close alsa device:
    snd_pcm_close(static_cast<snd_pcm_t*>(m_alsaHandle));
    m_alsaHandle = 0;
  }
}

void AlsaAudioDevice::start()
{
  // Unmute output:
  m_audioMuted = false;
}

void AlsaAudioDevice::stop()
{
  // Mute output:
  m_audioMuted = true;
}

void AlsaAudioDevice::audioThreadFunc()
{
  // Set running flag:
  m_audioThreadStopped = false;

  // Create buffer to fill for alsa:
  int size = m_blockSize * (m_bitDepth / 8) * m_channelCount;
  char* buffer = new char[size];
  memset(buffer, 0, size);

  // Create buffer to pass to the engine:
  SampleBuffer samples(m_channelCount, m_blockSize);

  // Get alsa handle:
  snd_pcm_t* pcmHandle = static_cast<snd_pcm_t*>(m_alsaHandle);

  // Run until stopped:
  while (!m_audioThreadStopped)
  {
    // Anything to write?
    if (m_audioMuted)
    {
      // Clear output:
      memset(buffer, 0, size);
    }
    else
    {
      // Get data:
      AudioSystem::processAudio(this, samples);

      // Convert and interlace data for output:
      signed short* ptr = reinterpret_cast<signed short*>(buffer);
      for (unsigned int i = 0; i < m_blockSize; i++)
      {
        for (unsigned int j = 0; j < m_channelCount; j++)
          *ptr++ = static_cast<signed short>(samples.sample(j, i) * 32768.0);
      }
    }

    // Wait for the driver to be ready:
    if (snd_pcm_wait(pcmHandle, 1000) < 0)
      continue;

    int rc = snd_pcm_writei(pcmHandle, buffer, m_blockSize);
    if (rc != static_cast<int>(m_blockSize))
    {
      // Buffer underrun?
      if (rc == -EPIPE)
      {
        fprintf(stderr, "xrun occurred\n");
        snd_pcm_prepare(pcmHandle);
      }

      // Other error?
      else if (rc < 0)
        fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));

      // Write incomplete
      else
        fprintf(stderr, "short write, write %d frames\n", rc);
    }
  }

  // Free hardware buffer:
  delete [] buffer;
}

void* AlsaAudioDevice::audioFunc(void* arg)
{
  // Delegate to the class function:
  static_cast<AlsaAudioDevice*>(arg)->audioThreadFunc();

  // The return value doesn't matter:
  return 0;
}
