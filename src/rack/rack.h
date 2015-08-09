#ifndef RACK_H
#define RACK_H

class Rack
{
public:
  Rack(class Document* doc);
  virtual ~Rack();

  class Document* document();
  const class Document* document() const;
  QList<class RackDevice*>& devices();
  const QList<class RackDevice*>& devices() const;
  bool activated() const;
  void activate(bool state = true);

  void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);

private:
  class Document* m_doc;
  QList<class RackDevice*> m_devices;
  bool m_activated;
};

#endif // RACK_H
