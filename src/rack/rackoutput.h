#ifndef RACKOUTPUT_H
#define RACKOUTPUT_H

#include "../dsp/smoothparameter.h"

class RackOutput :
  public RackDevice
{
public:
  RackOutput(class Rack* parent);
  virtual ~RackOutput();
  virtual int parameterCount() const;
  virtual double parameter(const int index);
  virtual void setParameter(const int index, const double value, const bool updateGUI);
  virtual void setSampleRate(const double rate);
  virtual void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);
  virtual class RackDeviceGUI* createGUI(QWidget* parent);

  double getVU(int channel) const;
  bool clipped() const;
  void resetClip();

private:
  SmoothParameter m_gain;
  SmoothParameter m_balance;
  bool m_muted;
  class VUMeter* m_vus;
  bool m_clipped;
};

#endif // RACKOUTPUT_H
