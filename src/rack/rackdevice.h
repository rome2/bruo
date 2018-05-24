#ifndef RACKDEVICE_H
#define RACKDEVICE_H

class RackDevice
{
public:
  RackDevice(class Rack* parent);
  virtual ~RackDevice();

  virtual void open();
  virtual void close();
  virtual void suspend();
  virtual void resume();
  bool suspended() const;

  virtual int parameterCount() const;
  virtual double parameter(const int index);
  virtual void setParameter(const int index, const double value, const bool updateGUI);
  class Rack* rack();
  const class Rack* rack() const;

  virtual double sampleRate() const;
  virtual void setSampleRate(const double rate);
  virtual int blockSize() const;
  virtual void setBlockSize(const int size);

  virtual void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);

  virtual class RackDeviceGUI* createGUI(QWidget* parent);
  virtual void guiDestroyed();

  class RackDeviceGUI* gui();
  const class RackDeviceGUI* gui() const;
  void setGui(class RackDeviceGUI* newGui);

private:
  class Rack* m_rack;
  class RackDeviceGUI* m_gui;
  double m_sampleRate;
  int m_blockSize;
  int m_suspendCounter;
};

#endif // RACKDEVICE_H
