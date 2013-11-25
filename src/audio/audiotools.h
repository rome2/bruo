#ifndef AUDIOTOOLS_H
#define AUDIOTOOLS_H

inline double AmpToDB(double val)
{
  if (val <= 0.0)
    return -INFINITY;
  return 20.0 * log10(val);
}

inline double DBToAmp(double val)
{
  if (val <= -INFINITY)
    return 0.0;
  return pow(10, val) / 20.0;
}

#endif // AUDIOTOOLS_H
