
#include <Arduino.h>
#include "pinDeclarations.h"
#include "variables.h"
#include "receiver.h"
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

void setup()
{
  telemetrySetup();

  Serial.begin(57600);
  Serial.println(freeMemory());

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(13, HIGH);
  motorSetup();
  digitalWrite(13, 0);

  Serial.println(freeMemory());
  dmpSetup();
  receiverSetup();

  while (1)
  {
    if(state.r_input_signal[4-1]>1900 && state.r_input_signal[3-1]<1050)
    {
      delay(1000);
      if(state.r_input_signal[4-1]>1900 && state.r_input_signal[3-1]<1050)
        break;
    }
  }
  state.arm();

  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
  digitalWrite(LED_PIN, HIGH);
  delay(500);

  delay(1000);
  thrustCalc::previousMicros = thrustCalc::now = micros();
}

void loop()
{
  getDmp();
  thrustCalculator();
  motorWrite();

  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);

  if(state.r_input_signal[4-1]<1050 && state.r_input_signal[3-1]<1050)
    dead();

  teleGainReceive();
  pcLogger1();
}
