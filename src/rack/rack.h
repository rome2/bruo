#ifndef RACK_H
#define RACK_H

class RackDevice
{
public:
  RackDevice(class Rack* parent) : m_rack(parent) { }
  virtual ~RackDevice() { }

  virtual int parameterCount() const { return 0; }
  virtual double parameter(const int index) { return 0.0; }
  virtual void setParameter(const int index, const double value) {}
  virtual void setParameterAutomated(const int index, const double value) {}

  class Rack* rack() { return m_rack; }
  const class Rack* rack() const { return m_rack; }

  void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime) { }

private:
  class Rack* m_rack;
};

class Rack
{
public:
  Rack(class Document* doc);
  virtual ~Rack();

  class Document* document();
  const class Document* document() const;

  void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);

private:
  class Document* m_doc;
  QList<RackDevice*> m_devices;
};

#endif // RACK_H
