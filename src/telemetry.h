#pragma once

#include <Arduino.h>
#include "variables.h"

void telemetrySetup()
{
  Serial.begin(57600);
  Serial.setTimeout(2);
}

namespace telemetry
{
  float gainArray[9]  =  {0.0};
}

int teleGainReceive()
{
  using namespace telemetry;

  if (Serial.available() > 1)
  {

    if (Serial.read() == '#')
    {
    //Start byte detected
      while (!Serial.available()) ;
    if (Serial.read()==49)
    {
      //write command detected
        for (int i  =  0; i < 9; i++)
        {
          while (!Serial.available()) ;
          gainArray[i]  =  Serial.parseFloat();
        }

        tempConfig.kp_pitch  =  gainArray[0];
        tempConfig.ki_pitch  =  gainArray[1];
        tempConfig.kd_pitch  = gainArray[2];
        tempConfig.kp_yaw    = gainArray[3];
        tempConfig.ki_yaw    = gainArray[4];
        tempConfig.kd_yaw    = gainArray[5];
        tempConfig.kp_roll   = gainArray[6];
        tempConfig.kd_roll   = gainArray[7];
        tempConfig.ki_roll   = gainArray[8];
        updateConfiguration(tempConfig);

        return 0;
      }
    }
  }
  return 1;
}
