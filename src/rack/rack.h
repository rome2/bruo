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
  bool suspended() const;
  void suspend();
  void resume();

  virtual double sampleRate() const;
  virtual void setSampleRate(const double rate);
  virtual int blockSize() const;
  virtual void setBlockSize(const int size);

  void process(const SampleBuffer& inputs, SampleBuffer& outputs, int frameCount, double streamTime);

private:
  class Document* m_doc;
  QList<class RackDevice*> m_devices;
  bool m_suspended;
  double m_sampleRate;
  int m_blockSize;
};

#endif // RACK_H
