#pragma once 

#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>
#include "variables.h"

#define ARDUINOPIN 3
namespace ppm
{
  volatile long oldTime = {0};
  volatile int marker= 5;
}

void receiverISR() //Interrupt service routine
{
  using namespace ppm;
  long curTime =  micros();
  if (curTime - oldTime > 5000)
  {
    marker = 0;
    oldTime = curTime;
    return;
  }

  if (marker >= 0 && marker <= 5)
  {
    state.r_input_signal[marker] = curTime - oldTime;
    marker++;
    oldTime = curTime;
    return;
  }
}

void ppmReceiverSetup()
{
  enableInterrupt(ARDUINOPIN, receiverISR, FALLING);
}
