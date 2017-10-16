//This is called as include guard:)
#pragma once

/* THIS HEADER CONTAINS DECLARATION OF GLOBAL VARIABLES AND CONSTANTS SO AS TO GENERALISE STUFF
 * NOTE:
 * This file should not be changed unneccessarily because all the previous stuff will get lost in the eeprom.
   Read the atmega on computer using this code and write the gains somewhere and then start middling with this code
 */

//----------------------------CONFIGURATIONS STORED----------------------------
class configClass  //This class will contain all the eeprom writable configuration data
{
//This class will be used for storing all the configurable parameters together and it will be saved to eeprom
public:

//TX CALIBRATIO
int CH1limits[2]={1000,2000};
int CH2limits[2]={1000,2000};
int CH3limits[2]={1000,2000};
int CH4limits[2]={1000,2000};


//TX sensitivity
float pitchSens =0.06;
float rollSens  =0.06;
float yawSens =70;

//TRIM settings
float pitchTrim =0;
float rollTrim  =0;
float yawTrim =0;

//Motor limits
int lowerCalibrationLimit=1000, upperCalibrationLimit=2000;
int shutDownAvoider=1070, max_throttle=1900;

//GAINS                         //for 11.75
float kp_pitch=1.5;             //1.5
float ki_pitch=0.07;           //0.07
float kd_pitch=0.45;            //0.45
float kp_roll=1.3;              //1.3
float ki_roll=0.12;            //0.12
float kd_roll=0.55;             //0.55
float kp_yaw=1.5;               //1.2
float ki_yaw=0.0;              //0
float kd_yaw=0;                 //0

//FUNCTIONS
int writeEepromConfig(); //def in configurationFunctions.h
int readEepromConfig();  //def in configurationFunctions.h
} config,  tempConfig;//All the arrived configuration is checked in code and sent to config object. Config object has the executable settings


//-----------------------------------STATES---------------------------------
//The following class will be used to store main state and control inputs as per
// the requirement for telemetry and control loop

class stateClass
{

private:
//If armStatus is false then Dont ever run the motorSetup
//If armStatus is true dont run calibration functions and eeprom functions. That can lead to disaster.
//DONT make the armstatus false by using the code. //Only the Transmitter should have the power to arm and disarm.
bool armStatus=false;

public:
//variables:
float ypr[3]={0.0};
float initialYaw=0.0,yawInputIntegral=0.0;
bool gotDMPData=false;
volatile int r_input_signal[6]={0};
float pitchE=0.0, rollE=0.0, yawE=0.0;
float oldRollE=0,oldPitchE=0,oldYawE=0;

float pitchPid=0.0, rollPid=0.0, yawPid=0.0;
float fmean=0.0;
int thrust1 =0, thrust2 =0, thrust3 =0, thrust4 =0;
float theta=0.0;
public:
//Functions
int arm()             { armStatus=true;  return 0;  }
int disarm()          { armStatus=false; return 0;  }
bool getArmStatus()   { return armStatus;  }
} state;


class verificationParams
{
public:
  const float kp_p_lim[2]={0.0, 5.0};
  const float ki_p_lim[2]={0.0, 2.0};
  const float kd_p_lim[2]={0.0, 2.0};
  const float kp_y_lim[2]={0.0, 10.0};
  const float ki_y_lim[2]={0.0, 2.0};
  const float kd_y_lim[2]={0.0, 2.0};
  const float kp_r_lim[2]={0.0, 5.0};
  const float ki_r_lim[2]={0.0, 2.0};
  const float kd_r_lim[2]={0.0, 2.0};
}VP;
