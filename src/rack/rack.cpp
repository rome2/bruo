#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rack.h"
#include "document.h"

Rack::Rack(Document* doc) :
  m_doc(doc)
{
}

Rack::~Rack()
{
}

void Rack::process(const SampleBuffer& /* inputs */, SampleBuffer& outputs, int frameCount, double /* streamTime */)
{
  outputs.makeSilence();
  if (m_doc->playing())
  {
    m_doc->readSamples(m_doc->cursorPosition(), outputs, frameCount);
    m_doc->setCursorPosition(m_doc->cursorPosition() + frameCount);
    if (m_doc->cursorPosition() >= m_doc->sampleCount())
      m_doc->setPlaying(false);
  }
}
