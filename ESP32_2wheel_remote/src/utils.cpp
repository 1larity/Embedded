#include <Arduino.h>
String commandString="";
String parameterString="";

// Constant for steps in disk
const float stepcount = 20.00; // 20 Slots in disk, change if different

// Constant for wheel diameter
const float wheeldiameter = 66.10; // Wheel diameter in millimeters, change if different

// Function to convert from centimeters to steps
int CMtoSteps(float cm)
{

  int result;                                        // Final calculation result
  float circumference = (wheeldiameter * 3.14) / 10; // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;         // CM per Step

  float f_result = cm / cm_step; // Calculate result as a float
  result = (int)f_result;        // Convert to an integer (note this is NOT rounded)

  return result; // End and return result
}

void splitString(String str, String delimiter)
{
    int start = 0;  
    int end = str.indexOf(delimiter);
    while (end != -1) {
        commandString = str.substring(start, end - start) ;
        start = end + delimiter.length();
        end = str.indexOf(delimiter, start);
    }
   parameterString = str.substring(start, end - start);
}