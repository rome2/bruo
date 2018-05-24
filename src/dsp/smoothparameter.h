////////////////////////////////////////////////////////////////////////////////
// (c) 2017 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    smoothparameter.h
///\ingroup bruo
///\brief   Parameter smoothing helper class.
///\author  Rolf Meyerhoff (rm@matrix44.de)
///\version 1.0
/// This file is part of the bruo audio editor.
////////////////////////////////////////////////////////////////////////////////
///\par License:
/// This program is free software: you can redistribute it and/or modify it
/// under the terms of the GNU General Public License as published by the Free
/// Software Foundation, either version 2 of the License, or (at your option)
/// any later version.
///\par
/// This program is distributed in the hope that it will be useful, but WITHOUT
/// ANY WARRANTY; without even  the implied warranty of MERCHANTABILITY or
/// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
/// more details.
///\par
/// You should have received a copy of the GNU General Public License along with
/// this program; see the file COPYING. If not, see http://www.gnu.org/licenses/
/// or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
/// Floor, Boston, MA 02110-1301, USA.
////////////////////////////////////////////////////////////////////////////////
#ifndef __SMOOTHPARAMETER_H_INCLUDED__
#define __SMOOTHPARAMETER_H_INCLUDED__

////////////////////////////////////////////////////////////////////////////////
///\class SmoothParameter smoothparameter.h
///\brief Parameter smoothing class.
///
/// Helper class to avoid zipper noise while changing parameters in a effect or
/// synth plugin. Internally it uses a simple one pole low pass filter.
///
/// Usage: Add one SmoothParameter for each parameter into your class. Set the
///        desired value via setValue() and read the smoothed values in the
///        process function through the interpolator.
///
/// // In constructor:
/// SmoothParameter smoothGain(1.0);
///
/// [...]
///
/// // Set value from GUI:
/// smoothGain.setValue(0.5);
///
/// [...]
///
/// // Process samples:
/// for (int i = 0; i < sampleCount; i++)
/// {
///   // No more zipper noise:
///   double gain = smoothGain.tick();
///   out1[i] = in1[i] * gain;
///   out2[i] = in2[i] * gain;
/// }
///
////////////////////////////////////////////////////////////////////////////////
class SmoothParameter
{
public:
  //////////////////////////////////////////////////////////////////////////////
  // SmoothParameter::SmoothParameter()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this widget.
  ///\param   [in] initialValue: Initial value of the parameter to smooth.
  ///\remarks It is usually a good idea to provide a realistic initial value to
  ///         avoid smoothing at the very first tick() calls.
  //////////////////////////////////////////////////////////////////////////////
  SmoothParameter(const double initialValue);

  //////////////////////////////////////////////////////////////////////////////
  // SmoothParameter::tick()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Calc the smoothed parameter value.
  ///\return  The current smoothed parameter.
  ///\remarks Only call this once per sample as the smoothing is done on a per
  ///         sample basis. So if you need the value more than once then store
  ///         it in a temporary variable.
  //////////////////////////////////////////////////////////////////////////////
  double tick();

  //////////////////////////////////////////////////////////////////////////////
  // SmoothParameter::sampleRate()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the SampleRate property.
  ///\return  The current sample rate of this object.
  ///\remarks This must be set to the current sample rate of the project or else
  ///         the parameter smoothing will not work as expected.
  //////////////////////////////////////////////////////////////////////////////
  double sampleRate() const;

  //////////////////////////////////////////////////////////////////////////////
  // SmoothParameter::setSampleRate()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the SampleRate property.
  ///\param   [in] newRate: The new sample rate.
  ///\remarks This must be set to the current sample rate of the project or else
  ///         the parameter smoothing will not work as expected.
  //////////////////////////////////////////////////////////////////////////////
  void setSampleRate(const double newRate);

  //////////////////////////////////////////////////////////////////////////////
  // SmoothParameter::time()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Time property.
  ///\return  The current smoothing time in seconds.
  ///\remarks Reasonable values are between 0.01 and 0.03 to avoid zipper noises
  ///         while parameters change.
  //////////////////////////////////////////////////////////////////////////////
  double time() const;

  //////////////////////////////////////////////////////////////////////////////
  // SmoothParameter::setTime()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Time property.
  ///\param   [in] newTime: New smoothing time in seconds.
  ///\remarks Reasonable values are between 0.01 and 0.03 to avoid zipper noises
  ///         while parameters change.
  //////////////////////////////////////////////////////////////////////////////
  void setTime(const double newTime);

  //////////////////////////////////////////////////////////////////////////////
  // SmoothParameter::value()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Value property.
  ///\return  Returns the current raw parameter value.
  ///\remarks This is the real value as it is set by the DAW and as it stored to
  ///         disk. To get the smoothed value use tick().
  //////////////////////////////////////////////////////////////////////////////
  double value() const;

  //////////////////////////////////////////////////////////////////////////////
  // SmoothParameter::setValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Value property.
  ///\param   [in] newVal: New target value of this parameter.
  ///\remarks This is the real value as it is set by the DAW and as it stored to
  ///         disk. The smoothed values from tick() will reach this value
  ///         eventually.
  //////////////////////////////////////////////////////////////////////////////
  void setValue(const double newValue);

private:

  //////////////////////////////////////////////////////////////////////////////
  // SmoothParameter::updateCoefficient()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Update the internal filter state.
  ///\remarks This is called after a sample rate or time change.
  //////////////////////////////////////////////////////////////////////////////
  void updateCoefficient();

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  double m_sampleRate;    ///\> Current sample rate.
  double m_smoothedValue; ///\> Current smoothed value.
  double m_rawValue;      ///\> Current real/target value.
  double m_coeff;         ///\> Current filter coefficient.
  double m_time;          ///\> Smoothing time for the filter in s.
};

#endif // #ifndef __SMOOTHPARAMETER_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
