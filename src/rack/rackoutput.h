#ifndef RACKOUTPUT_H
#define RACKOUTPUT_H

class RackOutput :
  public RackDevice
{
public:
  RackOutput(class Rack* parent);
  virtual ~RackOutput();
  virtual void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);
};

#endif // RACKOUTPUT_H
