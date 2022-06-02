
/*
  PCA9685 PWM Servo Driver Example
  pca9685-servomotor-demo.ino
  Demonstrates use of 16 channel I2C PWM driver board with 4 servo motors
  Uses Adafruit PWM library
  Uses 4 potentiometers for input

  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/

// Include Wire Library for I2C Communications
#include <Wire.h>
// Inlude motor control functions
#include "motorControl.h"



//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Define Potentiometer Inputs

int controlA = A0;
int controlB = A1;
int controlC = A2;


// Define Motor Outputs on PCA9685 board

int motorA = 0;
int motorB = 4;
int motorC = 8;

int potValA;
int potValB;
int potValC;


void setup() 
{
  Serial.begin(9600);
  motorInit();

}




void loop() {
//potControl();
//cycleControl();
cycleControl2();
//potControlSynched();

feedBack();
}

void feedBack(){
  analogReference(DEFAULT);
  potValA = analogRead(controlA);
  potValB = analogRead(controlB);
    analogReference(EXTERNAL);
  potValC = analogRead(controlC);
   
  Serial.print(", PotvalA: ");
  Serial.print(potValA);
  Serial.print(", PotvalB: ");
  Serial.print(potValB);
  Serial.print(", PotvalC: ");
  Serial.print(potValC);
  Serial.println(".");
}

//Sweep all servos
void cycleControl(){
int angle;

  for (int i = 0; i < 180; i=i+10){
    angle=i;
  feedBack();
  //Control Motor A
  moveMotorDegrees(angle, motorA);
  
  //Control Motor B
  moveMotorDegrees(angle , motorB);
  delay(100);
}

  //Control Motor C
  moveMotorDegrees(angle, motorC);
  
 for (int i = 180; i > 0; i=i-10){
    angle=i;
  feedBack();
  //Control Motor A
  moveMotorDegrees(angle, motorA);
  
  //Control Motor B
  moveMotorDegrees(angle , motorB);
    
  //Control Motor C
  moveMotorDegrees(angle, motorC);
  delay(100);
 // analogReference(DEFAULT);
  }
}

//Sweep all servos
void cycleControl2(){
int angle;

    angle=180;
  feedBack();
  //Control Motor A
  moveMotorDegrees(angle, motorA);
  
  //Control Motor B
  moveMotorDegrees(angle , motorB);
  delay(800);

  //Control Motor C
  moveMotorDegrees(angle, motorC);
  
 
    angle=0;
  feedBack();
  //Control Motor A
  moveMotorDegrees(angle, motorA);
  
  //Control Motor B
  moveMotorDegrees(angle , motorB);
    
  //Control Motor C
  moveMotorDegrees(angle, motorC);
  delay(800);
 // analogReference(DEFAULT);
 }

//Control servos independently using 3 potentiometers
void potControl() {

  //Control Motor A
  moveMotor(controlA, motorA);
  
  //Control Motor B
  moveMotor(controlB, motorB);
    
  //Control Motor C
  moveMotor(controlC, motorC);
   feedBack();
}
//Control servos synched using 1 potentiometers
void potControlSynched() {
  //Control Motor A
  moveMotor(controlA, motorA);
  
  //Control Motor B
  moveMotor(controlA, motorB);
    
  //Control Motor C
  moveMotor(controlA, motorC);
  feedBack();
}
