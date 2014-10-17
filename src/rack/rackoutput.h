#ifndef RACKOUTPUT_H
#define RACKOUTPUT_H

class RackOutput :
  public RackDevice
{
public:
  RackOutput(class Rack* parent);
  virtual ~RackOutput();
  virtual int parameterCount() const;
  virtual double parameter(const int index);
  virtual void setParameter(const int index, const double value);
  virtual void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);
  virtual class RackDeviceGUI* createGUI(QWidget* parent);
private:
  double m_gain;
  double m_pan;
  bool m_muted;
};

#endif // RACKOUTPUT_H
