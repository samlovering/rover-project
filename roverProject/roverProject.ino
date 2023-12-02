/*
* Sam Lovering
* Last Updated: 10/29/23
*
* This project acts as the microcontroller driver for a rover.
* This code in the microcontroller will be chained into a PCA9685 Servo Driver to 
* drive a couple servos. For the first prototype it will just focus on driving.
*
* Todo:
* Down power the motor, I think on full throttle it uses too much power
*
*/
//Library Declarations
#include <PS4Controller.h>
#include <Adafruit_PWMServoDriver.h>
#include <TB6612_ESP32.h>


//Global Variables for TB6612 Library
#define AIN1 13 // ESP Pin 13 to TB6612 PIN AIN1
#define AIN2 14 // ESP Pin 14 to TB6612 PIN AIN2
#define PWMA 26 // ESP PIN 26 to TB6612 PIN PWMA
#define STBY 33 // ESP PIN 33 to TB6612 PIN STBY

const int offsetA = 1;
 
//intitialling servo driver and motor driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Motor driveTrain = Motor(AIN1, AIN2, PWMA, offsetA, STBY, 5000, 8, 1);

//global variables for PWM Driver
//SERVOMIN and SERVOMAX depend on servos, make sure to test, and maybe even make SERVOMAX_1 SERVOMIN_1
#define SERVOMIN  100 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  460 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  400 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 100 (SERVOMIN * 4)
#define USMAX  1840 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 460 (SERVOMAX * 4)
#define SERVO_FREQ 50 

//Servo motor connection (value based on connection from left to right)
#define SV0 0 //Left Right
#define SV1 1 //Shift

int pwm0; //Left/Right
int shiftToggle = 0;

//Begin Code
void setup() {
  // "a0:36:bc:d7:62:4a" MAC address for DS4 Controller 
  Serial.begin(115200);
  PS4.begin("a0:36:bc:d7:62:4a");
  Serial.printf("Ready.\n");

  pwm.begin();
  //Will likely need tweaking.
  pwm.setOscillatorFrequency(26000000);
  pwm.setPWMFreq(SERVO_FREQ);
}

void loop() {
  //Steering Controls with Servo  
  if(int LStickPos = PS4.LStickX()){
    //range of operation -128 to 127
    //Dead zone was tested around range (-6 to 6)
    //Serial.printf("DEBUG: LStickPos = %d.\n", LStickPos);
    pwm0 = map(LStickPos, -128, 127, SERVOMIN, SERVOMAX);
    pwm.setPWM(SV0, 0, pwm0);
  }
  //Motor Controls
  //Forwards
  if (PS4.R2()) {
    Serial.printf("R2 Engaged, Value = %d.\n", PS4.R2Value());
    driveTrain.drive((PS4.R2Value()/2), 1);
  }
  //Backwards
  if(PS4.L2()){
    Serial.printf("L2 Engaged, Value = %d.\n", PS4.L2Value());
    driveTrain.drive(-(PS4.L2Value()/2), 1);
  }
  if(PS4.Triangle()){
    shiftToggle = !shiftToggle;
    //Serial.printf("DEBUG: Battery Level: %d.\n", PS4.Battery());
    if(shiftToggle){
      Serial.printf("shiftToggle=1, Gear 1\n");
      //PS4.setLed(255, 0, 0);
      pwm.setPWM(SV1, 0, 340);
    }
    else{
      Serial.printf("shiftToggle=0, Gear 0\n");
      //PS4.setLed(80, 255, 80);
      pwm.setPWM(SV1, 0, 280);
    }
  //charge controller over night, if working better then 
  //PS4.sendToController();
  delay(200);
 }
}
