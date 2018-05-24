////////////////////////////////////////////////////////////////////////////////
// (c) 2017 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    smoothparameter.cpp
///\ingroup bruo
///\brief   Parameter smoothing helper class implementation.
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
#include <cmath>
#include "smoothparameter.h"

////////////////////////////////////////////////////////////////////////////////

SmoothParameter::SmoothParameter(const double initialValue) :
  m_sampleRate(44100.0),
  m_smoothedValue(initialValue),
  m_rawValue(initialValue),
  m_coeff(1.0),
  m_time(0.02)
{
  // Init state:
  updateCoefficient();
}

////////////////////////////////////////////////////////////////////////////////

double SmoothParameter::tick()
{
  // Filter value:
  m_smoothedValue += m_coeff * (m_rawValue - m_smoothedValue);

  // Return filtered value:
  return m_smoothedValue;
}

////////////////////////////////////////////////////////////////////////////////

double SmoothParameter::sampleRate() const
{
  // Return our sample rate:
  return m_sampleRate;
}

////////////////////////////////////////////////////////////////////////////////

void SmoothParameter::setSampleRate(const double newRate)
{
  // Update the internal rate:
  m_sampleRate = newRate;

  // Update state:
  updateCoefficient();
}

////////////////////////////////////////////////////////////////////////////////

double SmoothParameter::time() const
{
  // Return current time:
  return m_time;
}

////////////////////////////////////////////////////////////////////////////////

void SmoothParameter::setTime(const double newTime)
{
  // Update the internal time:
  m_time = newTime;

  // Update state:
  updateCoefficient();
}

////////////////////////////////////////////////////////////////////////////////

double SmoothParameter::value() const
{
  // Return current value:
  return m_rawValue;
}

////////////////////////////////////////////////////////////////////////////////

void SmoothParameter::setValue(const double newValue)
{
  // Set new value:
  m_rawValue = newValue;
}

////////////////////////////////////////////////////////////////////////////////

void SmoothParameter::updateCoefficient()
{
  // Recalculate coefficient:
  m_coeff = 1.0 - exp(-1.0 / (m_time * m_sampleRate));
}

///////////////////////////////// End of File //////////////////////////////////
