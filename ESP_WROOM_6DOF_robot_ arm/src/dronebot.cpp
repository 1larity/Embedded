// // --------------------------------------
// // i2c_scanner
// //
// // Version 1
// //    This program (or code that looks like it)
// //    can be found in many places.
// //    For example on the Arduino.cc forum.
// //    The original author is not know.
// // Version 2, Juni 2012, Using Arduino 1.0.1
// //     Adapted to be as simple as possible by Arduino.cc user Krodal
// // Version 3, Feb 26  2013
// //    V3 by louarnold
// // Version 4, March 3, 2013, Using Arduino 1.0.3
// //    by Arduino.cc user Krodal.
// //    Changes by louarnold removed.
// //    Scanning addresses changed from 0...127 to 1...119,
// //    according to the i2c scanner by Nick Gammon
// //    https://www.gammon.com.au/forum/?id=10896
// // Version 5, March 28, 2013
// //    As version 4, but address scans now to 127.
// //    A sensor seems to use address 120.
// // Version 6, November 27, 2015.
// //    Added waiting for the Leonardo serial communication.
// //
// //
// // This sketch tests the standard 7-bit addresses
// // Devices with higher bit address might not be seen properly.
// //
// #include <Arduino.h>
// #include <Wire.h>
//  #define I2C_SDA 32
// #define I2C_SCL 33
// // Include Adafruit PWM Library
// #include <Adafruit_PWMServoDriver.h>
// // Depending on your servo make, the pulse width min and max may vary, you 
// // want these to be as small/large as possible without hitting the hard stop
// // for max range. You'll have to tweak them as necessary to match the servos you
// // have!
// #define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
// #define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
// #define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
// #define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
// #define SERVO_FREQ 60 // Analog servos run at ~60 Hz updates

// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// // Define Potentiometer Inputs

// //int controlBase = A0;
// //int controlElbow = A1;
// //int controlWrist = A2;
// //int controlPivot = A3;
// int controlJaws = 34;

// // Define Motor Outputs on PCA9685 board

// int motorBase = 0;
// int motorElbow = 1;
// int motorWrist = 2;
// int motorPivot = 3;
// int motorJaws = 4;

// // Define Motor position variables
// int mtrDegreeBase;
// int mtrDegreeElbow;
// int mtrDegreeWrist;
// int mtrDegreePivot;
// int mtrDegreeJaws;
// void oldsetup()
// {
//   Wire.begin(I2C_SDA, I2C_SCL);
//    // Setup PWM Controller object
//   pwm.begin();
//   pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
//   pwm.setPWMFreq(SERVO_FREQ);

//   Serial.begin(9600);
//   while (!Serial);             // Leonardo: wait for serial monitor
//   Serial.println("\nI2C Scanner");
// }
 
// // Function to convert potentiometer position into servo angle
// int getDegree(int controlIn)
// {
//   int potVal,srvDegree;
  
//   // Read values from potentiometer
//   potVal = analogRead(controlIn);
  
//   // Calculate angle in degrees
//   srvDegree = map(potVal, 0, 4095, 0, 180);
  
//   // Return angle in degrees
//   return srvDegree;
  
// }

// void i2cScan() {
//     byte error, address;
//   int nDevices;
 
//   Serial.println("Scanning...");
 
//   nDevices = 0;
//   for(address = 1; address < 127; address++ )
//   {
// // The i2c_scanner uses the return value of
//     // the Write.endTransmisstion to see if
//     // a device did acknowledge to the address.
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
 
//     if (error == 0)
//     {
//       Serial.print("I2C device found at address 0x");
//       if (address<16)
//         Serial.print("0");
//       Serial.print(address,HEX);
//       Serial.println("  !");
 
//       nDevices++;
//     }
//     else if (error==4)
//     {
//       Serial.print("Unknown error at address 0x");
//       if (address<16)
//         Serial.print("0");
//       Serial.println(address,HEX);
//     }    
//   }
//   if (nDevices == 0)
//     Serial.println("No I2C devices found\n");
//   else
//     Serial.println("done\n");
// }

//  // Function to move motor to specific position
// void moveMotorDeg(int moveDegree, int motorOut)
// {
//   int pulse_wide, pulse_width;
 
//   // Convert to pulse width
//   pulse_wide = map(moveDegree, 0, 180, USMIN, USMAX);
//   pulse_width = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);
  
//   //Control Motor
//   pwm.setPWM(motorOut, 0, pulse_width);
// }
// void oldloop()
// {
//   pwm.setPWM(15, 1024, 3072);

//   //Control Jaws Motor
  
//   // Get desired position
//   mtrDegreeJaws = getDegree(controlJaws);
//   Serial.println(mtrDegreeJaws);
//   // Move motor
//   moveMotorDeg(mtrDegreeJaws,motorJaws);
// pwm.writeMicroseconds(4, 10000);
 
//   // Add short delay
//   delay(20);
 
 
// }
