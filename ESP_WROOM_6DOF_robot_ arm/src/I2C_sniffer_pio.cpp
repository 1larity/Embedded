/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  100 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  150 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  500 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2500 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~60 Hz updates

// our servo # counter
uint8_t servonum = 0;

//input pot pins
int controlWaist = 25;
int controlBack = 32;
int controlSholder = 33;
int controlElbow = 35;
int controlWrist = 34;
//int controlJaws = 34;

// // Define Motor Outputs on PCA9685 board
int motorWaist = 8;
int motorBack = 11;
int motorSholder = 12;
int motorElbow = 13;
int motorWrist = 14;
int motorJaws = 15;

// // Define Motor position variables
// int mtrDegreeBase;
// int mtrDegreeElbow;
int mtrDegreeWrist;
// int mtrDegreePivot;
int mtrDegreeJaws;

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  adcAttachPin(controlWaist);
  adcAttachPin(controlBack);
  adcAttachPin(controlSholder);
  adcAttachPin(controlElbow);
  adcAttachPin(controlWrist);
  pwm.begin();
  // In theory the internal oscillator is 25MHz but it really isn't
  // that precise. You can 'calibrate' by tweaking this number till
  // you get the frequency you're expecting!
  pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~60 Hz updates

  delay(10);
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}
// Function to convert potentiometer position into servo angle
int getDegree(int controlIn)
{
  int potVal,srvDegree;
  
  // Read values from potentiometer
  potVal = analogRead(controlIn);
  
  // Calculate angle in degrees
  srvDegree = map(potVal, 0, 4095, 0, 180);
  
  // Return angle in degrees
  return srvDegree;
  
}

 // Function to move motor to specific position
void moveMotorDeg(int moveDegree, int motorOut)
{
  uint16_t pulse_wide, pulse_width;
 
  // Convert to pulse width
  pulse_wide = map(moveDegree, 0, 180, USMIN, USMAX);
  pulse_width = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);
  
  //Control Motor
  pwm.writeMicroseconds(motorOut, pulse_wide);

  //pwm.setPWM(motorOut, 0, pulse_width);
}

void loop() {


  
 // Move motor
  moveMotorDeg(getDegree(controlWrist),motorWrist);
  moveMotorDeg(getDegree(controlElbow),motorElbow);
  moveMotorDeg(getDegree(controlSholder),motorSholder);
  moveMotorDeg(getDegree(controlBack),motorBack);
   moveMotorDeg(getDegree(controlWaist),motorWaist);
  //debug
  Serial.print("Wrist ");
  Serial.print(getDegree(controlWrist));
  Serial.print("Elbow ");
  Serial.print(getDegree(controlElbow));
  Serial.print("Sholder ");
  Serial.print(getDegree(controlSholder));
  Serial.print("Back ");
  Serial.print(getDegree(controlBack));
  Serial.print("Waist ");
  Serial.print(getDegree(controlWaist));
  Serial.println(".");
}