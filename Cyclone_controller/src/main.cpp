#include <Arduino.h>
 #include <Servo.h>//Using servo library to control ESC

Servo esc; //Creating a servo class with name as esc
//inputs
int potPin = A0;
//outputs
int controlPin = 2;
void setup()

{
pinMode(controlPin, OUTPUT); 
esc.attach(3); //Specify the esc signal pin,Here as D8

esc.writeMicroseconds(1000); //initialize the signal to 1000

Serial.begin(9600);

}

void loop()

{

int val; //Creating a variable val

val= analogRead(potPin); //Read input from analog pin a0 and store in val
Serial.print("pot :");
Serial.println(val);
//val= map(val, 0, 1023,1000,2000); //mapping val to minimum and maximum(Change if needed) esc.writeMicroseconds(val); //using val as the signal to esc
analogWrite(controlPin, val/4);
//esc.writeMicroseconds(val); //using val as the signal to es
}