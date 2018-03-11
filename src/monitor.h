#pragma once
#include <Arduino.h>
#include "variables.h"

namespace monitor{  unsigned short counter=0;   }

int pcLogger1()
{
  using namespace monitor;
  if(counter<100){counter++;return 0;}

  Serial.print(state.fmean     ); Serial.print("     ");
  Serial.print(state.pitchPid  ); Serial.print("     ");
  Serial.print(state.rollPid   ); Serial.print("     ");
  Serial.print(state.thrust1   ); Serial.print("     ");
  Serial.print(state.thrust2   ); Serial.print("     ");
  Serial.print(state.thrust3   ); Serial.print("     ");
  Serial.print(state.thrust4   ); Serial.print("     ");

  Serial.print(state.r_input_signal[0]);   Serial.print("  ");
  Serial.print(state.r_input_signal[1]);   Serial.print("  ");
  Serial.print(state.r_input_signal[2]);   Serial.print("  ");
  Serial.println(state.r_input_signal[3]); Serial.print("  ");

  counter=0;
  return 0;
}

int pcLogger()
{
  Serial.println( state.yawE);
  return 0;
}

int plotter()
{
  Serial.print(state.ypr[0]*57.2958 );  Serial.print(" ");
  Serial.print(state.ypr[1]*57.2958 );  Serial.print(" ");
  Serial.print(state.ypr[2]*57.2958 );  Serial.println();
  return 0;
}

int showGains()
{
  using namespace monitor;
  if(counter<10){counter++; return 0;}

  Serial.print(config.kp_pitch);
  Serial.print(" ");
  Serial.print(config.kp_roll);
  Serial.print(" ");
  Serial.print(config.kp_yaw);
  Serial.print(" ");
  Serial.print(config.ki_pitch);
  Serial.print(" ");
  Serial.print(config.ki_roll);
  Serial.print(" ");
  Serial.print(config.ki_yaw);
  Serial.print(" ");
  Serial.print(config.kd_pitch);
  Serial.print(" ");
  Serial.print(config.kd_roll);
  Serial.print(" ");
  Serial.println(config.kd_yaw);
  counter=0;

  return 0;
}
