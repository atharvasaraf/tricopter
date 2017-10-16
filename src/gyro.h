#pragma once

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include <Wire.h>      // For IIC
#include "I2Cdev.h" // for gyro sensor
#include "MPU6050_6Axis_MotionApps20.h"
#include "EnableInterrupt.h"// for gyros pin change interrupt
#include "pinDeclarations.h"
#include "dead.h"
#include "variables.h"

Quaternion q;           // [w, x, y, z]         quaternion container
MPU6050 mpu;

int initialYawFunction();
int getDmp();

/*CONNECTIONS
  arduino side            Mpu6050
  2                        Int
  A5                       SCL
  A4                       SDA
  5V                       VCC
  gnd                      GND*/

// MPU CONTROL/STATUS VARIABLES
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// ORIENTATION VARIABLES
//Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
//float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


// packet structursee for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };

//FUNCTION PROTOTYPES DECLARATION
int getDmp();
int dmpSetup();


// ===               INTERRUPT DETECTION ROUTINE                ===

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}



//===============================================================
//                          DMP SETUP
//==============================================================
int dmpSetup()
{
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  // initialize device
  //serial.println(F("Initializing I2C devices..."));
  mpu.initialize();

  // verify connection
  //serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));


  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);



  // load and configure the DMP
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(-178);
  mpu.setYGyroOffset(-67);
  mpu.setZGyroOffset(10);
  mpu.setZAccelOffset(680); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    //serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    //attachInterrupt(0, dmpDataReady, RISING);
    enableInterrupt(2,dmpDataReady,RISING);// I guess i am trying to use enable Interrupt library for this also. It works
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    //serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
    delay(1500);

    initialYawFunction();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    //serial.print(F("DMP Initialization failed (code "));
    //serial.print(devStatus);
    //serial.println(F(")"));
    dead();
  }

  //failsafe
  if (!dmpReady)dead();
  return 0;
  //===============================================================

}

/////////////////////////////////////////////////////////////////////
//                     GET DMP FUNCTION
/////////////////////////////////////////////////////////////////////

int getDmp()
{
  //return 0= peaceful execution
  //return 1= no output other reason
  // return -1= overflow

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {

    // reset so we can continue cleanly
    mpu.resetFIFO();
    //serial.println(F("\n FIFO overflow!"));
    return -1;           /////    means fifo overflow
  }

  // otherwise, check for DMP data ready interrupt (this should happen frequently)
    else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);


    // display Euler angles in degrees
   mpu.dmpGetQuaternion(&q, fifoBuffer);
   mpu.dmpGetEuler(euler, &q);
   mpu.dmpGetGravity(&gravity, &q);
   mpu.dmpGetYawPitchRoll(state.ypr, &q, &gravity);

    state.gotDMPData=true;
    return 0; ///// Zero means put peace
  }
  else {
    //serial.print("\n___");  /// 1 means u read before the value became available
    return 1;
  }
}


//The following function will find the initial yaw angle given by GYRO and set it as
// the "initalyaw".
int initialYawFunction()
  {
   // delay(1000);
   float yawAngle=0;
   delay(10);yawAngle+=state.ypr[0];
      delay(10);yawAngle+=state.ypr[0];
         delay(10);yawAngle+=state.ypr[0];
            delay(10);yawAngle+=state.ypr[0];
               delay(10);yawAngle+=state.ypr[0];
    state.initialYaw=yawAngle*0.2;
    return 0;
  }
