
// command implementation

// Analog write for ESP32
#include <WebSerial.h>
#include <analogWrite.h>
#include "utils.h"
// ToF sensor support
#include "Adafruit_VL53L0X.h"
// compass support
#include <QMC5883LCompass.h>
// Motor A
int enA = 33; // 10;
int in1 = 27; // 9;
int in2 = 25; // 8;

// Motor B
int enB = 26; // 5;
int in3 = 32; // 7;
int in4 = 12; // 6;

int minSpeed = 170;
int maxSpeed = 255;
// Integers for pulse counters
int lastCounter_A = 0;
int lastCounter_B = 0;
volatile int counter_A = 0;
volatile int counter_B = 0;

// Motor A pulse count ISR
void ISR_countA()
{
  counter_A++; // increment Motor A counter value
}

// Motor B pulse count ISR
void ISR_countB()
{
  counter_B++; // increment Motor B counter value
}

// get all sensors and print
void getSensors(VL53L0X_RangingMeasurementData_t measure, int leftUsound, int rightUsound, QMC5883LCompass compass)
{
  WebSerial.print("Laser ");
  WebSerial.print(measure.RangeMilliMeter);
  WebSerial.print("mm left ultrasound ");
  WebSerial.print(leftUsound);
  WebSerial.print("mm, right ultrasound ");
  WebSerial.print(rightUsound);
  WebSerial.println("mm.");
  int x, y, z;
  x = compass.getX();
  y = compass.getY();
  z = compass.getZ();
  WebSerial.print("X: ");
  WebSerial.print(x);
  WebSerial.print(" Y: ");
  WebSerial.print(y);
  WebSerial.print(" Z: ");
  WebSerial.print(z);

  int azimuth = compass.getAzimuth();
  WebSerial.print(" AZ: ");
  WebSerial.print(azimuth);
  char compassArray[3];
  compass.getDirection(compassArray, azimuth);
  WebSerial.print(" dir1: ");
  WebSerial.print(char(compassArray[0]));
  WebSerial.print(" dir2: ");
  WebSerial.print(compassArray[1]);
  WebSerial.print(" dir2: ");
  WebSerial.print(compassArray[2]);
  WebSerial.println();
}

void stop()
{
  // Stop when done
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}
// Function to Move Forward
void MoveForward(int steps, int mspeed)
{
  counter_A = 0; //  reset counter A to zero
  counter_B = 0; //  reset counter B to zero
  // Set Motor A forward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  // Set Motor B forward
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 255);
  analogWrite(enA, 255);
  delay(10);
  // Go forward until step value is reached
  while (steps > counter_A && steps > counter_B)
  {
    if (steps > counter_A)
    {
      analogWrite(enA, mspeed);
    }
    else
    {
      analogWrite(enA, 0);
    }
    if (steps > counter_B)
    {
      analogWrite(enB, mspeed);
    }
    else
    {
      analogWrite(enB, 0);
    }
  }
}
// Function to Move in Reverse
void MoveReverse(int steps, int mspeed)
{
  counter_A = 0; //  reset counter A to zero
  counter_B = 0; //  reset counter B to zero
  // Set Motor A reverse
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Set Motor B reverse
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 255);
  analogWrite(enA, 255);
  delay(10);
  // Go in reverse until step value is reached
  while (steps > counter_A && steps > counter_B)
  {
    if (steps > counter_A)
    {
      analogWrite(enA, mspeed);
    }
    else
    {
      analogWrite(enA, 0);
    }
    if (steps > counter_B)
    {
      analogWrite(enB, mspeed);
    }
    else
    {
      analogWrite(enB, 0);
    }
  }
}

// Function to Spin Right
void SpinRight(int steps, int mspeed)
{
  counter_A = 0; //  reset counter A to zero
  counter_B = 0; //  reset counter B to zero

  // Set Motor A reverse
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Set Motor B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  // Go until step value is reached
  while (steps > counter_A && steps > counter_B)
  {

    if (steps > counter_A)
    {
      analogWrite(enA, mspeed);
    }
    else
    {
      analogWrite(enA, 0);
    }
    if (steps > counter_B)
    {
      analogWrite(enB, mspeed);
    }
    else
    {
      analogWrite(enB, 0);
    }
  }
}

// Function to Spin Left
void SpinLeft(int steps, int mspeed)
{
  counter_A = 0; //  reset counter A to zero
  counter_B = 0; //  reset counter B to zero
  // Set Motor A forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // Go until step value is reached
  while (steps > counter_A && steps > counter_B)
  {
    if (steps > counter_A)
    {
      analogWrite(enA, mspeed);
    }
    else
    {
      analogWrite(enA, 0);
    }
    if (steps > counter_B)
    {
      analogWrite(enB, mspeed);
    }
    else
    {
      analogWrite(enB, 0);
    }
  }
}

// Seek, move FOrwards until minimum range of laser is hit, bot should decelerate as range is approached
void seek(VL53L0X_RangingMeasurementData_t measure)
{
  if (measure.RangeMilliMeter <= 50)
  {
    stop();
  }
  if ((measure.RangeMilliMeter >= 51) && (measure.RangeMilliMeter <= 100))
  {
    MoveForward(CMtoSteps(1), 50);
  }
  if ((measure.RangeMilliMeter >= 101) && (measure.RangeMilliMeter <= 200))
  {
    MoveForward(CMtoSteps(1), 100);
  }
  if ((measure.RangeMilliMeter >= 201) && (measure.RangeMilliMeter < 400))
  {
    MoveForward(CMtoSteps(1), 200);
  }
  if ((measure.RangeMilliMeter >= 400))
  {
    MoveForward(CMtoSteps(1), 255);
  }
}
// move backwards (20cm)
void reverse(int distance)
{
  MoveReverse(CMtoSteps(distance), minSpeed);
  stop();
}
// spinleft
void spinLeft(int distance)
{
  SpinLeft(CMtoSteps(distance), minSpeed);
  stop();
}
// spinright
void spinRight(int distance)
{
  SpinRight(CMtoSteps(distance), minSpeed);
  stop();
}
// move forwards 20 cm
void forwards(int distance)
{
  MoveForward(CMtoSteps(distance), minSpeed);
  stop();
}
// hokycoky
void hokycoky()
{
  for (int i = 0; i < 100; i++)
  {
    WebSerial.println("forwards");
    MoveForward(CMtoSteps(2), maxSpeed);
    MoveForward(CMtoSteps(40), minSpeed);
    delay(1000);
    WebSerial.println("back");
    MoveReverse(CMtoSteps(2), maxSpeed);
    MoveReverse(CMtoSteps(40), minSpeed);
    delay(1000);
    WebSerial.println("right");
    SpinRight(CMtoSteps(2), maxSpeed);
    SpinRight(CMtoSteps(40), minSpeed);
    delay(1000);
    WebSerial.println("left");
    SpinLeft(CMtoSteps(2), maxSpeed);
    SpinLeft(CMtoSteps(40), minSpeed);
    delay(1000);
  }
  stop();
  // WebSerial.println("Done");
}
// linedance
void linedance()
{
  for (int i = 0; i < 100; i++)
  {
    WebSerial.println("forwards");
    MoveForward(CMtoSteps(40), minSpeed);
    delay(1000);
    WebSerial.println("back");
    MoveReverse(CMtoSteps(40), minSpeed);
    delay(1000);
  }
  stop();

  WebSerial.println("Done");
}
// take 1 ranging measurement
void range(VL53L0X_RangingMeasurementData_t measure, int leftUsound, int rightUsound, QMC5883LCompass compass)
{
  getSensors(measure, leftUsound, rightUsound, compass);
}
// take range 1/s for 30s
void rangeRepeat(VL53L0X_RangingMeasurementData_t measure, int leftUsound, int rightUsound, QMC5883LCompass compass)
{
  for (int i = 0; i < 30; i++)
  {
    getSensors(measure, leftUsound, rightUsound, compass);
    delay(1000);
  }
}
// help
void help()
{

  WebSerial.println("stop: halt robot.");
  WebSerial.println("back N: go back N cemtimeters.");
  WebSerial.println("left N: turn left.");
  WebSerial.println("right N: turn right.");
  WebSerial.println("hokycoky: do a little dance.");
  WebSerial.println("linedance: do a different dance");
  WebSerial.println("range: take one range measurement.");
  WebSerial.println("rangerepeat: take measurements 1 per second for 30 seconds.");
  WebSerial.println("NYI");
  WebSerial.println("NYI");
}

void faceNorth(QMC5883LCompass compass)
{
  compass.read();
  int azimuth = compass.getAzimuth();
  while (azimuth > 4 && azimuth < 356)
  {
    compass.read();
    azimuth = compass.getAzimuth();
    WebSerial.print(" AZ: ");
    WebSerial.println(azimuth);
    while (azimuth > 4 && azimuth < 180)
    {
      compass.read();
      azimuth = compass.getAzimuth();
      WebSerial.print(" AZ: ");
      WebSerial.println(azimuth);
      SpinLeft(1, minSpeed);
      stop();
      delay(150);
    }
    while (azimuth < 356 && azimuth >= 180)
    {
      compass.read();
      azimuth = compass.getAzimuth();
      WebSerial.print(" AZ: ");
      WebSerial.println(azimuth);
      SpinRight(1, minSpeed);
      stop();
      delay(150);
    }
  }
}