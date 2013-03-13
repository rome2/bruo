#ifndef PEAKDATA_H
#define PEAKDATA_H

#include "bruo.h"

////////////////////////////////////////////////////////////////////////////////
// struct PeakSample
////////////////////////////////////////////////////////////////////////////////
///\brief One audio peak data sample.
struct PeakSample
{
  qint16 maxVal; ///> Maximum value of this sample interval.
  qint16 minVal; ///> Minimum value of this sample interval.
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
  ///         and SampleCount properties must have been set to reasonable values
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

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
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
    m_sampleRate(0),
    m_timeStamp(0),
    m_sourceSize(0),
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
    m_sampleRate = 0;
    m_timeStamp = 0;
    m_sourceSize = 0;
    m_numMipmaps = 0;
  }

  bool readReapeaks(QString fileName)
  {
    // Parameter check:
    if (fileName.isNull() || fileName.isEmpty())
      return false;

    // Open the file:
    QFile peakFile(fileName);
    if (!peakFile.open(QIODevice::ReadOnly))
      return false;

    // Check size:
    size_t fileSize = peakFile.size();
    size_t expectedSize = 18;
    if (fileSize < expectedSize)
      return false;

    // Attach a data stream:
    QDataStream peakStream(&peakFile);
    peakStream.setByteOrder(QDataStream::LittleEndian);

    // Read and check header's magic value:
    quint8 magic[4];
    peakStream >> magic[0];
    peakStream >> magic[1];
    peakStream >> magic[2];
    peakStream >> magic[3];
    if (magic[0] != 'R' || magic[1] != 'P' || magic[2] != 'K' || (magic[3] != 'M' && magic[3] != 'N'))
      return false;

    // Read rest of the header:
    quint32 ui32;
    quint8 ui8;
    peakStream >> ui8;  m_numChannels = ui8;
    peakStream >> ui8;  m_numMipmaps  = ui8;
    peakStream >> ui32; m_sampleRate  = ui32;
    peakStream >> ui32; m_timeStamp   = ui32;
    peakStream >> ui32; m_sourceSize  = ui32;

    // Sanity check:
    if (m_numChannels == 0 || m_numMipmaps == 0)
      return false;

    // Check size:
    expectedSize += m_numMipmaps * 8;
    if (fileSize < expectedSize)
      return false;

    // Create mipmap header:
    m_mipmaps = new MipmapLevel[m_numMipmaps];
    if (m_mipmaps == 0)
      return false;

    // Load mipmap header:
    for (int i = 0; i < m_numMipmaps; i++)
    {
      // Load properties:
      peakStream >> ui32;
      m_mipmaps[i].setDivisionFactor(static_cast<int>(ui32));
      peakStream >> ui32;
      m_mipmaps[i].setSampleCount(static_cast<int>(ui32));
      m_mipmaps[i].setChannelCount(m_numChannels);

      // Update expected file size:
      if (magic[3] == 'N')
        expectedSize += m_mipmaps[i].sampleCount() * m_numChannels * 4;
      else
        expectedSize += m_mipmaps[i].sampleCount() * m_numChannels * 2;
    }

    // Check size:
    if (fileSize < expectedSize)
      return false;

    // Read all mipmaps:
    for (int i = 0; i < m_numMipmaps; i++)
    {
      // Create mipmap data:
      m_mipmaps[i].createSamples();

      // Read all samples:
      for (int j = 0; j < m_mipmaps[i].sampleCount(); j++)
      {
        // Deinterleave samples:
        for (int k = 0; k < m_numChannels; k++)
        {
          // New version?
          if (magic[3] == 'N')
          {
            // Read min and max values:
            peakStream >> m_mipmaps[i].samples()[k][j].maxVal;
            peakStream >> m_mipmaps[i].samples()[k][j].minVal;
          }

          // No, v1.0:
          else
          {
            // We only have one value, use it for both min and max:
            qint16 val;
            peakStream >> val;
            m_mipmaps[i].samples()[k][j].maxVal = val >= 0 ? val : -val;
            m_mipmaps[i].samples()[k][j].minVal = val <= 0 ? val : -val;
          }
        }
      }
    }

    // Return success:
    return true;
  }

  int channelCount() const
  {
    // Return current channel count:
    return m_numChannels;
  }

  int sampleRate() const
  {
    return m_sampleRate;
  }

  bool valid() const
  {
    return m_numChannels > 0 && m_sampleRate > 0 && m_numMipmaps > 0 && m_mipmaps != 0;
  }

  const MipmapLevel* mipmaps() const
  {
    return m_mipmaps;
  }

private:
  //////////////////////////////////////////////////////////////////////////////
  // Member:
  int          m_numChannels; ///> Number of channels of source file (usually 1 or 2)
  int          m_sampleRate;  ///> Samplerate of source file.
  int          m_timeStamp;   ///> Last modified time of source file (i.e. from stat().st_mtime) Note that REAPER will allow for small variations (a few seconds), as well as within a few seconds of one hour off (to allow for DST changes etc)
  int          m_sourceSize;  ///> File size of source (i.e. from stat() .st_size). Note that these last two members are purely for making sure that an updated file is detected--you can completely ignore them on read if rebuilding peaks is not an option.
  QString      m_sourceFile;  ///> The source file name.
  int          m_numMipmaps;  ///> Number of mipmaps. As of REAPER 2.x, the maximum mipmaps supported is 4.
  MipmapLevel* m_mipmaps;     ///> Actual peak data as mipmaps.
};

#endif // PEAKDATA_H
