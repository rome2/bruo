#ifndef VUMETER_H
#define VUMETER_H

class VUMeter
{
public:
  VUMeter();

  double vu() const;

  double falloff() const;
  void setFalloff(double newVal);

  double sampleRate() const;
  void setSampleRate(double newVal);

  bool peakMode() const;
  void setPeakMode(bool newVal);

  double tick(double input);

  void reset();

private:

  void updateCoeff();

  double m_vu;
  double m_falloff;
  double m_coeff;
  double m_sampleRate;
  bool m_peakMode;
};

#endif // VUMETER_H
