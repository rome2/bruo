#ifndef WAVEVIEW_H
#define WAVEVIEW_H

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
    numChannels(0),
    sampleRate(0),
    timeStamp(0),
    sourceSize(0),
    numMipmaps(0),
    mipmaps(0)
  {
    // Nothing to do here.
  }

  virtual ~PeakData()
  {
    // Free peaks:
    if (mipmaps != 0)
      delete [] mipmaps;
    mipmaps = 0;

    // Reset members:
    numChannels = 0;
    sampleRate = 0;
    timeStamp = 0;
    sourceSize = 0;
    numMipmaps = 0;
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
    peakStream >> ui8;  numChannels = ui8;
    peakStream >> ui8;  numMipmaps  = ui8;
    peakStream >> ui32; sampleRate  = ui32;
    peakStream >> ui32; timeStamp   = ui32;
    peakStream >> ui32; sourceSize  = ui32;

    // Sanity check:
    if (numChannels == 0 || numMipmaps == 0)
      return false;

    // Check size:
    expectedSize += numMipmaps * 8;
    if (fileSize < expectedSize)
      return false;

    // Create mipmap header:
    mipmaps = new MipmapLevel[numMipmaps];
    if (mipmaps == 0)
      return false;

    // Load mipmap header:
    for (int i = 0; i < numMipmaps; i++)
    {
      // Load properties:
      peakStream >> ui32;
      mipmaps[i].setDivisionFactor(static_cast<int>(ui32));
      peakStream >> ui32;
      mipmaps[i].setSampleCount(static_cast<int>(ui32));
      mipmaps[i].setChannelCount(numChannels);

      // Update expected file size:
      if (magic[3] == 'N')
        expectedSize += mipmaps[i].sampleCount() * numChannels * 4;
      else
        expectedSize += mipmaps[i].sampleCount() * numChannels * 2;
    }

    // Check size:
    if (fileSize < expectedSize)
      return false;

    // Read all mipmaps:
    for (int i = 0; i < numMipmaps; i++)
    {
      // Create mipmap data:
      mipmaps[i].createSamples();

      // Read all samples:
      for (int j = 0; j < mipmaps[i].sampleCount(); j++)
      {
        // Deinterleave samples:
        for (int k = 0; k < numChannels; k++)
        {
          // New version?
          if (magic[3] == 'N')
          {
            // Read min and max values:
            peakStream >> mipmaps[i].samples()[k][j].maxVal;
            peakStream >> mipmaps[i].samples()[k][j].minVal;
          }

          // No, v1.0:
          else
          {
            // We only have one value, use it for both min and max:
            qint16 val;
            peakStream >> val;
            mipmaps[i].samples()[k][j].maxVal = val >= 0 ? val : -val;
            mipmaps[i].samples()[k][j].minVal = val <= 0 ? val : -val;
          }
        }
      }
    }

    // Return success:
    return true;
  }

  int getChannelCount() const
  {
    // Return current channel count:
    return numChannels;
  }

  bool getValid() const
  {
    return numChannels > 0 && sampleRate > 0 && numMipmaps > 0 && mipmaps != 0;
  }

  const MipmapLevel* getMipmaps() const
  {
    return mipmaps;
  }

private:
  //////////////////////////////////////////////////////////////////////////////
  // Member:
  int          numChannels; ///> Number of channels of source file (usually 1 or 2)
  int          sampleRate;  ///> Samplerate of source file.
  int          timeStamp;   ///> Last modified time of source file (i.e. from stat().st_mtime) Note that REAPER will allow for small variations (a few seconds), as well as within a few seconds of one hour off (to allow for DST changes etc)
  int          sourceSize;  ///> File size of source (i.e. from stat() .st_size). Note that these last two members are purely for making sure that an updated file is detected--you can completely ignore them on read if rebuilding peaks is not an option.
  QString      sourceFile;  ///> The source file name.
  int          numMipmaps;  ///> Number of mipmaps. As of REAPER 2.x, the maximum mipmaps supported is 4.
  MipmapLevel* mipmaps;     ///> Actual peak data as mipmaps.
};

class WaveView :
  public QWidget
{
public:

  WaveView(QWidget* parent = 0) :
    QWidget(parent),
    showRuler(true),
    showScales(true)
  {
  }

  bool readPeakData(const QString& fileName)
  {
    return peakData.readReapeaks(fileName);
  }

protected:

  bool showRuler;
  bool showScales;

  void drawRuler(const QRect& rulerRect, QPainter& painter)
  {
    // Clear background:
    painter.fillRect(rulerRect, QColor(192, 192,192));

    // Draw divider:
    painter.setPen(QColor(0, 0, 0));
    painter.drawLine(rulerRect.left(), rulerRect.bottom(), rulerRect.right(), rulerRect.bottom());
  }

  void drawScales(const QRect& scaleRect, QPainter& painter)
  {
    // Clear background:
    painter.fillRect(scaleRect, QColor(160, 160,160));

    // Draw divider:
    painter.setPen(QColor(0, 0, 0));
    painter.drawLine(scaleRect.right(), scaleRect.top(), scaleRect.right(), scaleRect.bottom());
  }

  virtual void paintEvent(QPaintEvent* /* event */)
  {
    // Are we empty?
    if (!peakData.getValid())
    {
      // Fill red:
      QPainter painter(this);
      painter.fillRect(0, 0, width(), height(), Qt::white);

      // Done:
      return;
    }

    QColor backColor(255, 255, 255);
    QColor centerColor(192, 192, 192);
    QColor waveColor(127, 127, 127);

    // Create painter:
    QPainter painter(this);

    QRect waveArea(0, 0, width(), height());

    // Ruler visible?
    QRect rulerRect(0, 0, 0, 0);
    if (showRuler)
    {
      // Set ruler dimensions:
      rulerRect.setCoords(0, 0, width(), 20);

      // Draw the ruler:
      drawRuler(rulerRect, painter);

      // Update wave drawing area:
      waveArea.setTop(rulerRect.bottom() + 1);
    }

    // Scales visible?
    QRect scaleRect(0, 0, 0, 0);
    if (showScales)
    {
      // Set scale area dimensions:
      if (rulerRect.height() > 0)
        scaleRect.setCoords(0, rulerRect.bottom() + 1, 30, height());
      else
        scaleRect.setCoords(0, rulerRect.bottom(), 30, height());

      // Draw the scales:
      drawScales(scaleRect, painter);

      // Update wave drawing area:
      waveArea.setLeft(scaleRect.right() + 1);
    }

    // Clear wave background:
    painter.fillRect(waveArea, backColor);

    // Get height of a single channel:
    double channelHeight = (double)waveArea.height() / peakData.getChannelCount();

    // Draw channels:
    for (int channel = 0; channel < peakData.getChannelCount(); channel++)
    {
      const PeakSample* samples = peakData.getMipmaps()[1].samples()[channel];
      int cnt = peakData.getMipmaps()[1].sampleCount();

      // Create target rect:
      QRect destRect(waveArea.left(), channel * channelHeight + waveArea.top(), width(), (channel + 1) * channelHeight + waveArea.top());

      // Draw center line:
      painter.setPen(centerColor);
      int y = destRect.top() + channelHeight / 2;
      painter.drawLine(destRect.left(), y, destRect.right(), y);

      // Draw peaks:
      painter.setPen(waveColor);
      for (int i = 0; i < cnt && i < destRect.width(); i++)
      {
        // Convert span to -1..1:
        double minVal = (double)samples[i].minVal / 32768.0;
        double maxVal = (double)samples[i].maxVal / 32768.0;

        // Move into window (windows is top down so reverse range):
        double ymax = (0.5 * channelHeight) + (minVal * channelHeight * 0.5);
        double ymin = (0.5 * channelHeight) + (maxVal * channelHeight * 0.5);

        // Draw the value:
        int x = i + destRect.left();
        painter.drawLine(x, (int)ymax + destRect.top(), x, (int)ymin + destRect.top());
      }
    }
  }

private:
  PeakData peakData;
};


#endif // WAVEVIEW_H
