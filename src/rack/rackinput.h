#ifndef RACKINPUT_H
#define RACKINPUT_H

class RackInput :
  public RackDevice
{
public:
  RackInput(class Rack* parent);
  virtual ~RackInput();
  virtual void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);
};

#endif // RACKINPUT_H
