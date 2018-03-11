#pragma once

#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>
#include "variables.h"

volatile int r_previous_time=0;
volatile int r_lastIntPin=0;
volatile bool r_currentState=0;
void riseHandler();
void fallHandler();

int interruptPinMap(int interrupPin)
{
  switch(interrupPin)
  {
    case 14: return 0;
    case 15: return 1;
    case 16: return 2;
    case 17: return 3;
    case 4: return 3;
    case 3: return 3;
  }
  return 0;
}

void interruptHandler()
{
  r_currentState=digitalRead(arduinoInterruptedPin);

  if(r_currentState==1)
  {
    r_previous_time=micros();
    r_lastIntPin=arduinoInterruptedPin;
  }
  else if(r_lastIntPin!=arduinoInterruptedPin)
  {
    r_previous_time=0;
    r_lastIntPin=arduinoInterruptedPin;
  }
  else
  {
    state.r_input_signal[interruptPinMap(arduinoInterruptedPin) ]=micros()-r_previous_time;
    r_lastIntPin=arduinoInterruptedPin;
    r_currentState=0;
  }

}


void receiverSetup()
{
  enableInterrupt(A0,interruptHandler,CHANGE);
  enableInterrupt(A1,interruptHandler,CHANGE);
  enableInterrupt(A2,interruptHandler,CHANGE);
  enableInterrupt(A3,interruptHandler,CHANGE);
  enableInterrupt(4,interruptHandler,CHANGE);
  enableInterrupt(3,interruptHandler,CHANGE);
}
