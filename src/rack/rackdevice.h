#ifndef RACKDEVICE_H
#define RACKDEVICE_H

class RackDevice
{
public:
  RackDevice(class Rack* parent);
  virtual ~RackDevice();

  virtual int parameterCount() const;
  virtual double parameter(const int index);
  virtual void setParameter(const int index, const double value);
  virtual void setParameterAutomated(const int index, const double value);
  class Rack* rack();
  const class Rack* rack() const;

  virtual void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);

  virtual class RackDeviceGUI* createGUI(QWidget* parent);
  virtual void guiDestroyed();

  class RackDeviceGUI* gui();
  const class RackDeviceGUI* gui() const;
  void setGui(class RackDeviceGUI* newGui);

private:
  class Rack* m_rack;
  class RackDeviceGUI* m_gui;
};

#endif // RACKDEVICE_H
