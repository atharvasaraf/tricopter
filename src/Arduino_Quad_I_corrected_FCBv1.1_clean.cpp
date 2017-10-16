
//RULES:
//Dont make global variables unnecessarily
//Use Namespaces when needed
//Make sure that the temporary changes made for testing are marked appropriately

//This is for Arduino based Basic stabilizer
//The code is modularised

//---------------------LIBRARY------------------------------------------------------------------------
#include <Arduino.h>
#include "pinDeclarations.h"
#include "variables.h"
#include "receiver.h" //Custom library to acquire data from receiver
#include "dead.h"
#include "gyro.h"
#include "thrustCalc_euler.h"
#include "motorDriver.h"
#include "monitor.h"
#include "configurationFunctions.h"
#include "telemetry.h"
#include <MemoryFree.h>
void setup();
void loop();

bool blinkState = true;

/* DOES NOT WORK. SO REVERTED BACK TO .INO FROM .CPP
ITS ALREADY DEFINED IN ARDUINO.H
BUT FIND OUT THE REAL CAUSE OF PROBLEM LATER
int main()  { setup();
              while(1)  loop();
              return 0;
            }
*/
// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
void setup()
{
  telemetrySetup();
            Serial.begin(57600);
            Serial.println(freeMemory());
  pinMode(LED_PIN, OUTPUT);               //Serial.println(F("setup started\n"));
            digitalWrite(13, HIGH);
  motorSetup();
            digitalWrite(13, 0);
            Serial.println(freeMemory());
  dmpSetup();                             //Serial.println(F("dmp setup done"));
  receiverSetup();                        //Serial.println(F("rx setup done"));
                                          //pcLogger();
  while (1)//Arming check
      {if(state.r_input_signal[4-1]>1900 && state.r_input_signal[3-1]<1050)
          {  delay(1000);  if(state.r_input_signal[4-1]>1900 && state.r_input_signal[3-1]<1050)  break; } }
  state.arm(); //only place where it can arm!!
  //Blink leds
  digitalWrite(LED_PIN, HIGH);delay(500);digitalWrite(LED_PIN, LOW);delay(500);digitalWrite(LED_PIN, HIGH);delay(500);
  delay(1000);
  thrustCalc::previousMicros = thrustCalc::now = micros();
                                          //Serial.println("Setup Over!");

}

////////////////////////////////////////////////////////////////////////
//                       LOOP STUFF BEGINS
////////////////////////////////////////////////////////////////////////
void loop()
{ getDmp();
  thrustCalculator();//this changes previousMicros time
  motorWrite();
  blinkState = !blinkState;// blink LED to indicate activity
  digitalWrite(LED_PIN, blinkState);
  if(state.r_input_signal[4-1]<1050 && state.r_input_signal[3-1]<1050) dead();
  teleGainReceive();
  pcLogger1();
}
