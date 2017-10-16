#pragma once

#include<Arduino.h>
#include"variables.h"

namespace thrustCalc{
                      unsigned long now = 0, previousMicros = 0;
                      float roll_d=0,pitch_d=0,yaw_d=0;
                      float roll_i=0,pitch_i=0,yaw_i=0;
                      float oldPitch_d,oldRoll_d,oldYaw_d;
                    }

/////////////////////////////////////////////////////////////////////////////
//                              THRUST CALCULATOR
/////////////////////////////////////////////////////////////////////////////
int thrustCalculator()
{
using namespace thrustCalc;
now = micros();
state.fmean=state.r_input_signal[3-1] ;//got through interrupt function.EXtra variable
  //This calculates the Thrust to be given to motors in terms of Pulse width in microsecs
if(micros()-previousMicros<4000) return 0; //otherwise continue so that the code will not have too mcuh noise

state.pitchE=-state.ypr[1]*57.2958;// in deg
state.yawE=state.ypr[0]*57.2958-state.initialYaw*57.2958;
state.rollE=-state.ypr[2]*57.2958;
state.pitchE+=(state.r_input_signal[2-1]-1500)*0.06;//30 deg// this is pitch error
state.rollE+=(state.r_input_signal[1-1]-1500)*0.06;//30 deg

  {// YAW PART
    //yaw-=(r_input_signal[4-1]-1500)*0.06;//30 deg
    if ((state.r_input_signal[4-1]-1500<-50)||(state.r_input_signal[4-1]-1500>50))//check!!!!!
    state.yawInputIntegral+=((now-previousMicros)/1000000.0)*(state.r_input_signal[4-1]-1500)/1000*70;   //60deg in 3 sec
    if(state.yawInputIntegral>180.00 ){Serial.println("Yaw input grows out of 360 deg bound");   state.yawInputIntegral-=360.00;}
    else if(state.yawInputIntegral<-180.00){Serial.println("Yaw input grows out of 360 deg bound");   state.yawInputIntegral+=360.00;}
    state.yawE=state.yawE-state.yawInputIntegral;      //became yaw error now
    if(state.yawE>180.00) state.yawE-=360.00;
    else if(state.yawE<-180.00) state.yawE+=360.00;
  }

if(state.gotDMPData==true)//So that zero wont come as result
        { pitch_d=(state.pitchE-state.oldPitchE)/(float)(now-previousMicros)*1000000.0;
          yaw_d=(state.yawE-state.oldYawE)/(float)(now-previousMicros)*1000000.0;
          roll_d=(state.rollE-state.oldRollE)/(float)(now-previousMicros)*1000000.0;
          state.gotDMPData=false;}

pitch_i+=(state.pitchE)*(now-previousMicros)/1000000.0;
roll_i+=(state.rollE)*(now-previousMicros)/1000000.0;

yaw_i-=(state.yawE)*(now-previousMicros)/1000000.0;
state.oldPitchE=state.pitchE;
state.oldYawE=state.yawE;
state.oldRollE=state.rollE;
previousMicros=now;

state.pitchPid = (state.pitchE)*config.kp_pitch  +  pitch_d*config.kd_pitch +  pitch_i*config.ki_pitch;
state.rollPid = (state.rollE)*config.kp_roll  +  roll_d*config.kd_roll  +  roll_i*config.ki_roll;
//temporary changes:
state.yawPid = (state.yawE)*config.kp_yaw  +  yaw_d*config.kd_yaw;
//state.yawPid=state.r_input_signal[4-1];//comment out this one
  state.thrust1=state.fmean+state.rollPid+state.pitchPid;
  state.thrust2=state.fmean-state.rollPid+state.pitchPid;
  //state.thrust3=state.fmean+state.rollPid-state.pitchPid;
  state.thrust3=(state.fmean-state.pitchPid)+0.6*abs(state.yawPid);
  state.thrust4=1500-state.yawPid;

  if (state.fmean < config.lowerCalibrationLimit + 25) {   state.thrust1 = 0; state.thrust2 = 0; state.thrust3 = 0;state.thrust4 = 0; //to stop the rotors below some throttle input
                                                  pitch_d=0; yaw_d=0;roll_d=0;
                                                  pitch_i=0;  roll_i=0; yaw_i=0;
                                                 // yawInputIntegral=0;
                                             }
  else {
    if (state.thrust1 < config.shutDownAvoider) {
      state.thrust1 = config.shutDownAvoider;
      // This is to prevent Motors from stopping when the quad is stabilizing. If it stops it will take more time to gain back speed. So let this be a sort of minimum speed.
    }
    if (state.thrust2 < config.shutDownAvoider) {
      state.thrust2 = config.shutDownAvoider;
    }
    if (state.thrust3 < config.shutDownAvoider) {
      state.thrust3 = config.shutDownAvoider;
    }
    if (state.thrust4 < 1200) {
      state.thrust4 = 1200;
    }
  }
  return 0;
}
