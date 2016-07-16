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
  if (rack()->document() != 0 && rack()->document()->playing())
  {
    rack()->document()->readSamples(rack()->document()->cursorPosition(), outputs, frameCount);
    rack()->document()->setCursorPosition(rack()->document()->cursorPosition() + frameCount);
    if (rack()->document()->cursorPosition() >= rack()->document()->sampleCount())
      rack()->document()->setPlaying(false);
  }
}
