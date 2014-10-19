#include "bruo.h"
#include "vumeter.h"

VUMeter::VUMeter() :
  m_vu(0.0),
  m_falloff(300.0),
  m_sampleRate(44100.0),
  m_peakMode(false)
{
  // Init filter:
  updateCoeff();
}

double VUMeter::vu() const
{
  // Return current value:
  return m_vu;
}

double VUMeter::falloff() const
{
  // Return current value:
  return m_falloff;
}

void VUMeter::setFalloff(double newVal)
{
  // Anything to do?
  if (m_falloff == newVal)
    return;

  // Update time:
  m_falloff = newVal;

  // Update filter:
  updateCoeff();
}

double VUMeter::sampleRate() const
{
  // Return current sample rate:
  return m_sampleRate;
}

void VUMeter::setSampleRate(double newVal)
{
  // Anything to do?
  if (m_sampleRate == newVal)
    return;

  // Update sample rate:
  m_sampleRate = newVal;

  // Update filter:
  updateCoeff();
}

bool VUMeter::peakMode() const
{
  // Return current mode:
  return m_peakMode;
}

void VUMeter::setPeakMode(bool newVal)
{
  // Update mode:
  m_peakMode = newVal;
}

double VUMeter::tick(double input)
{
  double newVU = m_peakMode ? abs(input) : input * input;

  // Decaying? Filter value:
  if (newVU < m_vu)
    m_vu = newVU + m_coeff * (m_vu - newVU);

  // No, attack stage. Set average to peak:
  else
    m_vu = newVU;

  // This is a passive device so always return the original value:
  return input;
}

void VUMeter::reset()
{
  // Reset vu:
  m_vu = 0.0;
}

void VUMeter::updateCoeff()
{
  // Update filter coefficient (0.01 = -20 dB):
  m_coeff = exp(log(0.01) / (m_falloff * m_sampleRate * 0.001));
}
