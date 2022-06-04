//command implementations
#ifndef botfunctions_h
#define botfunctions_h
//ToF sensor support
#include "Adafruit_VL53L0X.h"
//compass support
#include <QMC5883LCompass.h>
// Motor A
extern int enA;
extern int in1;
extern int in2;

// Motor B
extern int enB;
extern int in3;
extern int in4;

extern int minSpeed;
extern int maxSpeed;

void ISR_countA();
void ISR_countB();
void stop();
void seek(VL53L0X_RangingMeasurementData_t measure);
void reverse(int distance);
void spinLeft(int distance);
void spinRight(int distance);
void forwards(int distance);
void hokycoky();
void linedance();
void range(VL53L0X_RangingMeasurementData_t measure, int leftUsound, int rightUsound, QMC5883LCompass compass);
void rangeRepeat(VL53L0X_RangingMeasurementData_t measure, int leftUsound, int rightUsound, QMC5883LCompass compass);
void help();
void MoveForward(int steps, int mspeed);
void MoveReverse(int steps, int mspeed);
void SpinRight(int steps, int mspeed);
void SpinLeft(int steps, int mspeed);
void faceNorth(QMC5883LCompass compass);
#endif