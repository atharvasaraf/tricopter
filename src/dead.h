#pragma once
//Make it using servo library for simplicity sake
#include<Arduino.h>
#include "pinDeclarations.h"
#include "variables.h"

//void(* resetFunc) (void) = 0; DANGER MAX

void dead()
{
  Serial.println(F("Sorry fellas, I am dead\nBy Flight Controller Board, Aero Club,CFI,IIT Madras-600036"));
            for(short int i=0;i<2000;i++)//wait 2 sec
            {  delayMicroseconds(3000);
                 PORTD = PORTD | B01100000;// Made high
                 PORTB = PORTB | B00000110;// esc Made high
               delayMicroseconds(1000);//1ms pulse
                  PORTD &= B11011111;//pin 5 to zero
                  PORTD &= B10111111;//pin 6
                  PORTB &= B11111101;//pin 9
                  PORTB &= B11111011;//pin 10 incase some cup happened
            }
  state.disarm(); //put disarm code into this function later
  //This is the only place where it can disarm!!
  digitalWrite(LED_PIN, 1);
//  digitalWrite(arming_LED, 0);
  delay(1000);
  digitalWrite(LED_PIN, 0);
  delay(1000);

  while (1)
   {
    //if(r_input_signal(4-1)>1900 && r_input_signal(3-1)<1050)
      if(state.r_input_signal[4-1]>1900 && state.r_input_signal[3-1]<1050)
        { delay(1000);
          if(state.r_input_signal[4-1]>1900 && state.r_input_signal[3-1]<1050)
          { ;}
        }
    }
    digitalWrite(LED_PIN, 1);
    delay(1000);
    digitalWrite(LED_PIN, 0);
    delay(1000);
  }
