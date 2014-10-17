#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rack.h"
#include "rackdevice.h"
#include "rackoutput.h"

RackOutput::RackOutput(class Rack* parent) :
  RackDevice(parent)
{
}

RackOutput::~RackOutput()
{
}

void RackOutput::process(const SampleBuffer& /*inputs*/, SampleBuffer& /*outputs*/, int /*frameCount*/, double /*streamTime*/)
{
}
