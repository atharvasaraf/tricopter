#pragma once
#include <EEPROM.h>
#include "variables.h"
#include "receiver.h"

//This header is meant to contain all additional features for FCB ranging from telecommanding functions to configuration changing functions like autotrim/autotune/calibration of gyro. Config function name
//should be a character. default value for that variable should be zero
//Try to use namespaces for future works :)
char functionCode='0';
int autotrim();
int radioCalibration();

//The following function will execute the needed functionality based on
//predetermined function code. call using execFunction(&X) where x='a'...
//call by reference is only used to have control over the variable inside function
int execFunction(char *code)
{
        switch(*code) {     case 'a':   autotrim(); break;
                    case 'b':   radioCalibration(); break;   }
        return 0;
}

//The following namespace is just to abstract few unnecessary variables from
//outside world.
namespace configSpace
{
float tempTrimCH1=0.0,tempTrimCH2=0.0,tempTrimCH4=0.0;//variables required for autotrim
const int noOfDataTaken=2000;
 int counterTrim=0;
}

int autotrim()// need to set some flag for its execution
{
        using namespace configSpace;
        if(counterTrim>=noOfDataTaken)//Corresponds to ~10Seconds
        {
                tempConfig.pitchTrim=(tempTrimCH2/(float)noOfDataTaken)*2000.0;
                tempConfig.rollTrim=(tempTrimCH1/(float)noOfDataTaken)*2000.0;
                tempConfig.yawTrim=(tempTrimCH4/(float)noOfDataTaken)*2000.0;
                counterTrim=0;
                functionCode=0;
                return 0;
                //Done autotrim NEXT STOP THIS CODE FROM EXECUTION BY CHANGING FUNCTIONCODE
        }
        tempTrimCH1+= state.r_input_signal[1-1]/2000.0;
        tempTrimCH2+= state.r_input_signal[2-1]/2000.0;
        tempTrimCH4+= state.r_input_signal[4-1]/2000.0;
        counterTrim++;
}

int radioCalibration()
{
        //have to find the TX end points
        //need to use one more channel !!
/*
      while(      )
          {

            delay(20);// no need to execute the program that fast
          }
 */
        return 0;
}


int verifyConfiguration(configClass obj)
{
        //This is to verify the whole configuration class object
        if(obj.kp_pitch>  VP.kp_p_lim[1] ||obj.kp_pitch<VP.kp_p_lim[0]) return 1;
        if(obj.ki_pitch>  VP.ki_p_lim[1] ||obj.ki_pitch<VP.ki_p_lim[0]) return 2;
        if(obj.kd_pitch>  VP.kd_p_lim[1] ||obj.kd_pitch<VP.kd_p_lim[0]) return 3;
        if(obj.kp_yaw>  VP.kp_y_lim[1] ||obj.kp_yaw<VP.kp_y_lim[0]) return 4;
        if(obj.ki_yaw>  VP.ki_y_lim[1] ||obj.ki_yaw<VP.ki_y_lim[0]) return 5;
        if(obj.kd_yaw>  VP.kd_y_lim[1] ||obj.kd_yaw<VP.kd_y_lim[0]) return 6;
        if(obj.kp_roll>  VP.kp_r_lim[1] ||obj.kp_roll<VP.kp_r_lim[0]) return 7;
        if(obj.ki_roll>  VP.ki_r_lim[1] ||obj.ki_roll<VP.ki_r_lim[0]) return 8;
        if(obj.kd_roll>  VP.kd_r_lim[1] ||obj.kd_roll<VP.kd_r_lim[0]) return 9;
        return 0; //passed verification
}
int updateConfiguration(configClass obj)
{       //To copy tempConfig into config obj
        //if(verifyConfiguration(obj)==0){  config = tempConfig;  } bypass
        config=tempConfig;
        //else return -1;

        return 0;// successful update
}

//CLASS configClass definitions
#define eepromStByt 50
int configClass::writeEepromConfig()
{
        char buf[sizeof config];
        memcpy(buf, &config, sizeof config);
        //Suppose the buffer is of 80bytes then to fully upload it to eeprom it will take 277 ms. This is too high. So if we try writing to eeprom then its gonna crash.
        //Better we can upload the gains when the bot is on ground, verify it and then push it into eeprom. Write gains when the bot is on ground when throttle is 0 using a blocking function
        if(sizeof config>512-eepromStByt-1) return -1; //Too much on arduino. better pack this shitCounter

        for(int i=0; i<sizeof config; i++)
        {
                EEPROM.update(eepromStByt+i,buf[i]);
        }
        return 0; //use 0 for success.-1 for cupmax cases
}
int configClass::readEepromConfig()
{
        char buf[sizeof config];
        for(int i=0; i<sizeof config; i++)
                buf[i]=EEPROM.read(eepromStByt+i);
        memcpy(&config,buf,sizeof config);
        return 0;
}
