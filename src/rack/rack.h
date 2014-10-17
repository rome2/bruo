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

  void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);

private:
  class Document* m_doc;
  QList<class RackDevice*> m_devices;
};

#endif // RACK_H
