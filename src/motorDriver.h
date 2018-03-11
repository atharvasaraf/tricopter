#pragma once

#include "Servo.h"
#include "variables.h"

Servo esc1, esc2, esc3, esc4;
int modCounter = 0;
int motorWrite();
int motorSetup();

int motorSetup()
{

  esc1.attach(5, config.lowerCalibrationLimit, config.upperCalibrationLimit);
  esc2.attach(6, config.lowerCalibrationLimit, config.upperCalibrationLimit);
  esc3.attach(9, config.lowerCalibrationLimit, config.upperCalibrationLimit);
  esc4.attach(10, config.lowerCalibrationLimit, config.upperCalibrationLimit);
  esc1.write(config.lowerCalibrationLimit);
  esc2.write(config.lowerCalibrationLimit);
  esc3.write(config.lowerCalibrationLimit);
  esc4.write(config.lowerCalibrationLimit);

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
//                                  MotorWrite
////////////////////////////////////////////////////////////////////////////
int motorWrite()
{
  if(state.getArmStatus()!=true)
    return -1;

  if(state.thrust1 > config.max_throttle)
  {
    state.thrust1 = config.max_throttle;
    modCounter++;
  }

  if (state.thrust2 > config.max_throttle)
  {
    state.thrust2 = config.max_throttle;
    modCounter++;
  }

  if (state.thrust3 > config.max_throttle)
  {
    state.thrust3 = config.max_throttle;
    modCounter++;
  }

  if (state.thrust4 > 1800)
  {
    state.thrust4 = 1800;
    modCounter++;
  }

  esc1.write(state.thrust1);
  esc2.write(state.thrust2);
  esc3.write(state.thrust3);
  esc4.write(state.thrust4);
  return 0;
}
