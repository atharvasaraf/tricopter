#pragma once


#include "Servo.h"
#include  "variables.h"
Servo esc1, esc2, esc3, esc4;
int shitCounter = 0;

int motorWrite();
int motorSetup();

int motorSetup()
{
  // MOTOR SIDE
  //end points depends on the esc calibration
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
//This basically writes into esc after putting a final foolproof mechanism(Thrust limiter for the Esc)

int motorWrite()
{
  if(state.getArmStatus()!=true) return -1;//its means code is trying to run when disarmed

  if (state.thrust1 > config.max_throttle) {
    state.thrust1 = config.max_throttle;
    shitCounter++;
  }
  if (state.thrust2 > config.max_throttle) {
    state.thrust2 = config.max_throttle;
    shitCounter++;
  }
  if (state.thrust3 > config.max_throttle) {
    state.thrust3 = config.max_throttle;
    shitCounter++;
  }
  if (state.thrust4 > 1800) {
    state.thrust4 = 1800;
    shitCounter++;
  }

  //if(shitCounter>=100) dead();

  esc1.write(state.thrust1);
  esc2.write(state.thrust2);
  esc3.write(state.thrust3);
  esc4.write(state.thrust4);
  return 0;
}
