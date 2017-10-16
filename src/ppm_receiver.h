#pragma once //include guard

#define EI_ARDUINO_INTERRUPTED_PIN  //Needed for enable interrupt lib I guess
#include <EnableInterrupt.h>
#include "variables.h"

#define ARDUINOPIN 3  //Put the pin number here
namespace ppm
{
//USE ONLY VOLATILE TYPE FOR ISR handling to avoid compiler optimizations and cups because of it
volatile long oldTime = {0};    //Temporary variables for calculations
volatile int marker= 5;
}

void receiverISR() //Interrupt service routine
{
        using namespace ppm;
        long curTime =  micros();
        if (curTime - oldTime > 5000)
        {
                //Start of ppm signal detected!
                marker = 0;
                oldTime = curTime;
                return;
        }

        if (marker >= 0 && marker <= 5) {
                //read the signal
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

// void loop() {
//   for(int i=0;i<6;i++)
//   {
//    Serial.print(state.r_input_signal[i]);
//    Serial.print(" ");
//   }
//   Serial.println("");
//   delay(100);
//   Serial.println("heartbeat");
// }
