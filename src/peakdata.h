#ifndef PEAKDATA_H
#define PEAKDATA_H

#include "bruo.h"
#include "audio/samplebuffer.h"

////////////////////////////////////////////////////////////////////////////////
// struct PeakSample
////////////////////////////////////////////////////////////////////////////////
///\brief One audio peak data sample.
struct PeakSample
{
  float maxVal; ///> Maximum value of this sample interval.
  float minVal; ///> Minimum value of this sample interval.
};

////////////////////////////////////////////////////////////////////////////////
///\brief One channel of audio peak data.
typedef PeakSample* PeakChannel;

////////////////////////////////////////////////////////////////////////////////
// class MipmapLevel
////////////////////////////////////////////////////////////////////////////////
///\brief One mipmap level for audio peak data.
class MipmapLevel
{
 public:

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::MipmapLevel()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this mipmap level.
  ///\remarks Initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  MipmapLevel() :
    m_cursor(0),
    m_block(0),
    m_divisionFactor(0),
    m_numSamples(0),
    m_numChannels(0),
    m_data(0),
    m_samples(0)
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::~MipmapLevel()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this mipmap level.
  ///\remarks Frees the members.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~MipmapLevel()
  {
    // Free buffers:
    if (m_data != 0)
      delete [] m_data;
    if (m_samples != 0)
      delete [] m_samples;
    m_data    = 0;
    m_samples = 0;

    // Reset members:
    m_cursor         = 0;
    m_block          = 0;
    m_divisionFactor = 0;
    m_numSamples     = 0;
    m_numChannels    = 0;
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::createSamples()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Create the internal buffers.
  ///\return  Returns true if successful or false on failure.
  ///\remarks This will clear any previously allocated buffers. The ChannelCount
  ///         and SampleCount properties must have been set to reasonable valuesSo o
  ///         before calling this function.
  //////////////////////////////////////////////////////////////////////////////
  bool createSamples()
  {
    // Environment check:
    if (m_numChannels <= 0 || m_numSamples <= 0)
      return false;

    // Free old buffers (if any):
    if (m_data != 0)
      delete [] m_data;
    if (m_samples != 0)
      delete [] m_samples;
    m_data    = 0;
    m_samples = 0;

    // Create sample buffer:
    m_data = new PeakSample[m_numChannels * m_numSamples];
    if (m_data == 0)
      return false;

    // Fill with zeroes:
    memset(m_data, 0, m_numChannels * m_numSamples * sizeof(PeakSample));

    // Create one pointer per channel:
    m_samples = new PeakChannel[m_numChannels];
    if (m_samples == 0)
      return false;

    // Set channel pointers into the buffer:
    for (int i = 0; i < m_numChannels; i++)
      m_samples[i] = m_data + i * m_numSamples;

    // Reset write cursors:
    m_cursor = 0;
    m_block  = 0;

    // Return success:
    return true;
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::divisionFactor()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor of the division factor property.
  ///\return  The current division factor.
  ///\remarks This value represents the number of samples represented by each
  ///         peak value.
  //////////////////////////////////////////////////////////////////////////////
  int divisionFactor() const
  {
    // Return current factor:
    return m_divisionFactor;
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::setDivisionFactor()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor of the division factor property.
  ///\param   [in] newFactor: The new division factor.
  ///\remarks This value represents the number of samples represented by each
  ///         peak value.
  //////////////////////////////////////////////////////////////////////////////
  void setDivisionFactor(const int newFactor)
  {
    // Parameter check:
    if (newFactor <= 0)
      return;

    // Update member:
    m_divisionFactor = newFactor;
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::sampleCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor of the sample count property.
  ///\return  The current number of samples.
  ///\remarks This value represents the number of samples in this mipmap.
  //////////////////////////////////////////////////////////////////////////////
  int sampleCount() const
  {
    // Return current sample count:
    return m_numSamples;
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::setSampleCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor of the sample count property.
  ///\param   [in] newCount: The new number of samples.
  ///\remarks This value represents the number of samples in this mipmap level.
  ///         Setting this value will not allocate any samples. You'll have to
  ///         use createSamples() above to create the actual buffers.
  //////////////////////////////////////////////////////////////////////////////
  void setSampleCount(const int newCount)
  {
    // Parameter check:
    if (newCount <= 0)
      return;

    // Update member:
    m_numSamples = newCount;
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::channelCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor of the channel count property.
  ///\return  The current number of channels.
  ///\remarks This value represents the number of channels of the audio data.
  //////////////////////////////////////////////////////////////////////////////
  int channelCount() const
  {
    // Return current channel count:
    return m_numChannels;
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::setChannelCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor of the ChannelCount property.
  ///\param   [in] newCount: The new number of channels.
  ///\remarks This value represents the number of channels of the audio data.
  ///         Setting this value will not update the sample buffers. You'll have
  ///         to use createSamples() above to update the actual buffers.
  //////////////////////////////////////////////////////////////////////////////
  void setChannelCount(const int newCount)
  {
    // Parameter check:
    if (newCount <= 0)
      return;

    // Update member:
    m_numChannels = newCount;
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::samples()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor of the samples property, const version.
  ///\return  The current samples, one buffer per channel.
  ///\remarks This value represents the actual peak data.
  //////////////////////////////////////////////////////////////////////////////
  const PeakChannel* samples() const
  {
    // Return hannel buffers:
    return m_samples;
  }

  //////////////////////////////////////////////////////////////////////////////
  // MipmapLevel::samples()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor of the samples property.
  ///\return  The current samples, one buffer per channel.
  ///\remarks This value represents the actual peak data.
  //////////////////////////////////////////////////////////////////////////////
  PeakChannel* samples()
  {
    // Return channel buffers:
    return m_samples;
  }

  void addSamples(int count, SampleBuffer& buffer)
  {
    return;
    for (int i = 0; i < count; i++)
    {
      for (int j = 0; j < m_numChannels; j++)
      {
        float sample = static_cast<float>(buffer.sample(j, i));

        // Init on first round:
        if (m_cursor == 0)
        {
          m_samples[j][m_block].minVal = sample;
          m_samples[j][m_block].maxVal = sample;
        }

        // Update value:
        if (sample < m_samples[j][m_block].minVal)
          m_samples[j][m_block].minVal = sample;
        if (sample > m_samples[j][m_block].maxVal)
          m_samples[j][m_block].maxVal = sample;
      }

      // Advance cursor:
      m_cursor++;
      if (m_cursor >= m_divisionFactor)
      {
        m_block++;
        if (m_block >= m_numSamples)
          m_block = m_numSamples - 1;
        m_cursor = 0;
      }
    }
  }

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  int          m_cursor;
  int          m_block;
  int          m_divisionFactor; ///> Samples represented by each peak value.
  int          m_numSamples;     ///> Number of samples in the buffer.
  int          m_numChannels;    ///> Number of channels.
  PeakSample*  m_data;           ///> The raw sample data.
  PeakChannel* m_samples;        ///> Data sorted by channel.
};

class PeakData
{
 public:

  PeakData() :
    m_numChannels(0),
    m_numSamples(0),
    m_sampleRate(0),
    m_numMipmaps(0),
    m_mipmaps(0)
  {
    // Nothing to do here.
  }

  virtual ~PeakData()
  {
    // Free peaks:
    if (m_mipmaps != 0)
      delete [] m_mipmaps;
    m_mipmaps = 0;

    // Reset members:
    m_numChannels = 0;
    m_numSamples  = 0,
    m_sampleRate  = 0;
    m_numMipmaps  = 0;
  }

  int channelCount() const
  {
    // Return current channel count:
    return m_numChannels;
  }

  double sampleRate() const
  {
    return m_sampleRate;
  }

  qint64 sampleCount() const
  {
    return m_numSamples;
  }

  bool valid() const
  {
    return m_numChannels > 0 && m_sampleRate > 0 && m_numMipmaps > 0 && m_mipmaps != 0 && m_numSamples != 0;
  }

  const MipmapLevel* mipmaps() const
  {
    return m_mipmaps;
  }

  MipmapLevel* mipmaps()
  {
    return m_mipmaps;
  }

  int mipmapCount() const
  {
    return m_numMipmaps;
  }

  void allocateMipMaps(int numMipMaps, int numChannels, double sampleRate, int sampleCount)
  {
    // Free peaks:
    if (m_mipmaps != 0)
      delete [] m_mipmaps;
    m_mipmaps = 0;

    // Reset members:
    m_numChannels = 0;
    m_numSamples  = 0;
    m_sampleRate  = 0;
    m_numMipmaps  = 0;

    // Allocate mipmaps:
    m_mipmaps = new MipmapLevel[numMipMaps];
    for (int i = 0; i < numMipMaps; i++)
    {
      // Get division factor:
      int factor = 0;
      switch (i)
      {
      case 0:
        factor = static_cast<int>(sampleRate / 300);
        break;
      case 1:
        factor = static_cast<int>(sampleRate / 20);
        break;
      case 2:
        factor = static_cast<int>(sampleRate);
        break;
      default:
        factor = static_cast<int>(sampleRate * i * 20);
        break;
      }

      // Get sample count:
      int count = sampleCount / factor;
      if (count < (sampleCount * factor))
        count++;

      // Update mipmap:
      m_mipmaps[i].setChannelCount(numChannels);
      m_mipmaps[i].setDivisionFactor(factor);
      m_mipmaps[i].setSampleCount(count);
      m_mipmaps[i].createSamples();
    }

    // Update members:
    m_numMipmaps  = numMipMaps;
    m_numChannels = numChannels;
    m_numSamples  = sampleCount;
    m_sampleRate  = sampleRate;
  }

private:
  //////////////////////////////////////////////////////////////////////////////
  // Member:
  int          m_numChannels; ///> Number of channels of the source file.
  qint64       m_numSamples;  ///> Number of sample frames of the source file.
  double       m_sampleRate;  ///> Samplerate of the source file.
  int          m_numMipmaps;  ///> Number of mipmaps.
  MipmapLevel* m_mipmaps;     ///> Actual peak data as mipmaps.
};

#endif // PEAKDATA_H
