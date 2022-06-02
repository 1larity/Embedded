#include <Arduino.h>
#include "Encoder.h"
//output pins
int REDPIN = 9;
int GREENPIN = 10;
int BLUEPIN = 11;
int MOTOR = 5;

//indicator LEDs
int REDINDPIN = 6;
int GREENINDPIN = 7;
int BLUEINDPIN = 8;

//input pins
int CLK = 3; // Pin 7 to clk on encoder
int DT = 2;  // Pin 6 to DT on encoder
int SW = 4;  //Pin 5 to SW on encoder

//encoder globals
Encoder knob(2, 3); //encoder instance

//LED timing
long currentMillis;      //time based delay, current time
long previousMillis = 0; //time based delay, last time called
long interval = 1;       //LED update frequency in milliseconds
int updown = 10;         //Direction of iterator increment
int i = 1;               //main iterator
int cycledelay=100;      //main loop delay

//light effect helpers
int currentPattern = 1;    //index of current light pattern
unsigned int rgbColour[3]; //tuple of colour values
int RED = 0;               //red colour enum
int GREEN = 1;             //green colour enum
int BLUE = 2;              //blue colour enum

//UI stuff
int MAXPATTERNS=6;         //number of light patterns

void setup()
{
  Serial.begin(9600);
  // pinMode(MOTOR, OUTPUT);
  //Main LED
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  //indicator LED
  pinMode(REDINDPIN, OUTPUT);
  pinMode(GREENINDPIN, OUTPUT);
  pinMode(BLUEINDPIN, OUTPUT);
  //encoder
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
}

//************set main LED pins
void setColourRgb(unsigned int red, unsigned int green, unsigned int blue)
{
  analogWrite(REDPIN, red);
  analogWrite(GREENPIN, green);
  analogWrite(BLUEPIN, blue);
}
//**********************light patterns****************
//**colour sweep from blue to green and back
void cyanRamp()
{
  if (updown)
  {
    rgbColour[BLUE] = 256 - i;
    rgbColour[GREEN] = i;
    pinMode(MOTOR, i);
  }
  else
  {
    rgbColour[BLUE] = i;
    rgbColour[GREEN] = 256 - i;
  }
  i = i + updown;
  if (i > 254 || i < 2)
  {
    updown = -updown;
  }
  analogWrite(MOTOR, i);
  setColourRgb(rgbColour[RED], rgbColour[GREEN], rgbColour[BLUE]);
}

//**colour sweep from blue to red and back
void purpleRamp()
{
  //Which direction is the incrementor going in?
  if (updown)
  {
    //fade blue
    rgbColour[BLUE] = 256 - i;
    //brighten red
    rgbColour[RED] = i;
    //  pinMode(MOTOR, i);
  }
  else
  {
    rgbColour[BLUE] = i;
    rgbColour[RED] = 256 - i;
  }
  i = i + updown;
  if (i > 254 || i < 2)
  {
    updown = -updown;
  }
  //  analogWrite(MOTOR, i);
  setColourRgb(rgbColour[RED], rgbColour[GREEN], rgbColour[BLUE]);
}

//**colour sweep from red to green and back
void orangeRamp()
{
  if (updown)
  {
    rgbColour[RED] = 256 - i;
    rgbColour[GREEN] = i;
    pinMode(MOTOR, i);
  }
  else
  {
    rgbColour[RED] = i;
    rgbColour[GREEN] = 256 - i;
  }
  i = i + updown;
  if (i > 254 || i < 2)
  {
    updown = -updown;
  }
  // analogWrite(MOTOR, i);
  setColourRgb(rgbColour[RED], rgbColour[GREEN], rgbColour[BLUE]);
}

//flicker effect
void flicker()
{
  rgbColour[RED] = random(120) + 135;
  rgbColour[GREEN] = random(120) + 135;
  rgbColour[BLUE] = random(120) + 135;
  setColourRgb(rgbColour[RED], rgbColour[GREEN], rgbColour[BLUE]);
  cycledelay=random(100);
}

//****************colourcycle*****************
void colourcycle()
{ // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour++)
  {
    int incColour = decColour == 2 ? 0 : decColour + 1;
    rgbColour[decColour] = i;
    rgbColour[incColour] = 255 - i;
    i = i + updown;
    if (i >= 255 || i <= 1)
    {
      rgbColour[RED] = 255;
      rgbColour[BLUE] = 0;
      rgbColour[GREEN] = 0;
      updown = -updown;
    }
    // analogWrite(MOTOR, i);
    setColourRgb(rgbColour[RED], rgbColour[GREEN], rgbColour[BLUE]);
  }
}
//****************Encoder input*******************
void readEncoder()
{

  int pos = knob.readAndReset();
  if (pos != 0)
  {
    if (pos < 0)
    {
      currentPattern--;
    }
    else
    {
      currentPattern++;
    }
    if (currentPattern > MAXPATTERNS)
    {
      currentPattern = 1;
    }

    if (currentPattern < 1)
    {
      currentPattern = MAXPATTERNS;
    }
    //setting has changed, reset Main LEDs
    setColourRgb(0, 0, 0);
    cycledelay=100;
    if (currentPattern == 3)
    {

      rgbColour[RED] = 255;
      rgbColour[GREEN] = 0;
      rgbColour[BLUE] = 0;
    }
    else
    {
      rgbColour[RED] = 0;
      rgbColour[GREEN] = 0;
      rgbColour[BLUE] = 0;
    }

    i = 1;
    updown = 1;
    delay(cycledelay);
  }
}

//****************pattern selectors***************
//**light pattern selector
void selectLightPattern(int currentPattern, boolean onphase)
{
  switch (currentPattern)
  {
  case 1:
  {
    cyanRamp();
    analogWrite(REDINDPIN, 0);
    analogWrite(GREENINDPIN, 255);
    analogWrite(BLUEINDPIN, 255);
  }
  break;
  case 2:
  {
    purpleRamp();
    analogWrite(REDINDPIN, 255);
    analogWrite(GREENINDPIN, 0);
    analogWrite(BLUEINDPIN, 255);
  }
  break;
  case 3:
  {
    orangeRamp();
    analogWrite(REDINDPIN, 255);
    analogWrite(GREENINDPIN, 255);
    analogWrite(BLUEINDPIN, 0);
  }
  case 4:
  {
    //Colourcycle
    colourcycle();
    analogWrite(REDINDPIN, 255);
    analogWrite(BLUEINDPIN, 255);
    analogWrite(REDINDPIN, 255);
  }
  break;
  case 5:
  {
    //off
    setColourRgb(0, 0, 0);
    analogWrite(REDINDPIN, 0);
    analogWrite(BLUEINDPIN, 0);
    analogWrite(REDINDPIN, 0);
  }
   break;
  case 6:
  {
    //flicker
    flicker();
    // setColourRgb(0, 0, 0);
    // analogWrite(REDINDPIN, 0);
    // analogWrite(BLUEINDPIN, 0);
    // analogWrite(REDINDPIN, 0);
  }
  }
  Serial.print(" pattern:");
  Serial.println(currentPattern);
}

//*******************LED Update
void update()
{
  readEncoder();
  currentMillis = millis();
  // Put timed operations in here:-
  if (currentMillis - previousMillis > interval)
  {
    selectLightPattern(currentPattern, true); // Execute the currently selected light pattern
    // save the last time you incremented i
    previousMillis = currentMillis;
    //do time triggered operations
    Serial.print("value:");
    Serial.print(i);
    Serial.print("cycle:");
    Serial.print(updown);
  }
}
//*******************main************************************
void loop()
{
  update();
}
