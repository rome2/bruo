#include "bruo.h"
#include "document.h"
#include "../audio/samplebuffer.h"
#include "rack.h"
#include "rackdevice.h"
#include "rackinput.h"

RackInput::RackInput(class Rack* parent) :
  RackDevice(parent)\
{
}

RackInput::~RackInput()
{
}

void RackInput::process(const SampleBuffer& /*inputs*/, SampleBuffer& outputs, int frameCount, double /*streamTime*/)
{
  Document* doc = rack()->document();
  if (!doc || !doc->playing())
    return;

  doc->readSamples(doc->cursorPosition(), outputs, frameCount);

  doc->setCursorPosition(doc->cursorPosition() + frameCount);
  if (doc->cursorPosition() >= doc->sampleCount())
  {
    if (doc->looping())
      doc->setCursorPosition(0);
    else
      rack()->document()->setPlaying(false);
  }

  if (rack()->document()->channelCount() < outputs.channelCount())
  {
    memcpy(outputs.sampleBuffer(1), outputs.sampleBuffer(0), outputs.sampleCount() * sizeof(double));
  }
}
