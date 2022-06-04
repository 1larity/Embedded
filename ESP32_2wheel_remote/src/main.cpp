/*
  Robot Car with Speed Sensor
*/
#include <Arduino.h>
// ToF sensor support
#include "Adafruit_VL53L0X.h"
// Ultrasonic sensor setup
#include <HCSR04.h>
// Analog write for ESP32
#include <analogWrite.h>
// Wifi support
#include <WiFi.h>
// TCP support
#include <AsyncTCP.h>
// web based serial console
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
// ESP wifi OTA firmware updater
#include <ArduinoOTA.h>
// compass support
#include <QMC5883LCompass.h>
// I2C support
#include <Wire.h>
// Wifi credentials
#include <secrets.h>
#include "botfunctions.h"
#include "operationSelector.h"
#include "utils.h"

// create webserver
AsyncWebServer server(80);
// create ToF object
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
// compass object
QMC5883LCompass compass;

// Pins
const int MOTOR_A = 23; // Motor 2 Interrupt Pin - INT 1 - Right Motor
const int MOTOR_B = 17; // Motor 1 Interrupt Pin - INT 0 - Left Motor
// int enA = 33; // 10;
// int in1 = 27; // 9;
// int in2 = 25; // 8;

// // Motor B
// int enB = 26; // 5;
// int in3 = 32; // 7;
// int in4 = 12; // 6;
// ultrasonic pins
byte triggerPin = 5;
byte echoCount = 2;

// setup ultrasonic sensors
byte *echoPins = new byte[echoCount]{15, 2};

// General purpose globals
int botCommand = 0;

void command(String command)
{
  String delimiter = " ";

  if (command.indexOf(delimiter) != -1)
  {
    splitString(command, delimiter);
  }
  else
  {
    commandString = command;
  }

  if (commandString == "seek")
  {
    botCommand = 1;
    WebSerial.println("Seek started.");
  }
  else if (commandString == "stop")
  {
    botCommand = 0;
    WebSerial.println("Stopping.");
  }
  else if (commandString == "back")
  {
    botCommand = 2;
    WebSerial.println("Reversing.");
  }
  else if (commandString == "left")
  {
    botCommand = 3;
    WebSerial.println("Turn left.");
  }
  else if (commandString == "right")
  {
    botCommand = 4;
    WebSerial.println("Turn right.");
  }
  else if (commandString == "forward")
  {
    botCommand = 5;
    WebSerial.println("Go forward.");
  }
  else if (commandString == "hokycoky")
  {
    botCommand = 6;
    WebSerial.println("You put your left leg in...");
  }
  else if (commandString == "linedance")
  {
    botCommand = 7;
    WebSerial.println("Where did you come from cotton eye Joe...");
  }
  else if (commandString == "range")
  {
    botCommand = 8;
    WebSerial.println("Get range");
  }
  else if (commandString == "rangerepeat")
  {
    botCommand = 9;
    WebSerial.println("Get range for 30 sec");
  }
  else if (commandString == "help")
  {
    botCommand = 10;
    WebSerial.println("Command help:-");
  }
  else if (commandString == "faceNorth")
  {
    botCommand = 11;
    WebSerial.println("facing north");
  }
  // What yo talkin' 'bout?
  else
  {
    WebSerial.println("Command not recognised.");
  }
}
/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len)
{
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++)
  {
    d += char(data[i]);
  }
  WebSerial.println(d);
  command(d);
}

// setup OTA
void setupOTA()
{

  ArduinoOTA
      .onStart([]()
               {
                 String type;
                 if (ArduinoOTA.getCommand() == U_FLASH)
                   type = "sketch";
                 else // U_SPIFFS
                   type = "filesystem";

                 // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                 Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
                 Serial.printf("Error[%u]: ", error);
                 if (error == OTA_AUTH_ERROR)
                   Serial.println("Auth Failed");
                 else if (error == OTA_BEGIN_ERROR)
                   Serial.println("Begin Failed");
                 else if (error == OTA_CONNECT_ERROR)
                   Serial.println("Connect Failed");
                 else if (error == OTA_RECEIVE_ERROR)
                   Serial.println("Receive Failed");
                 else if (error == OTA_END_ERROR)
                   Serial.println("End Failed"); });

  ArduinoOTA.begin();
}

void setupToF()
{
  Serial.println("starting VL53L0X ");
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }
}

void setupWifI()
{
  // WIFI setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, SSID_PW);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // WebSerial setup. Server is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  /* Attach Message Callback */
  WebSerial.msgCallback(recvMsg);
  server.begin();
}

void setup()
{
  delay(5000); // delay for HW to come up
  Serial.begin(115200);
  // Attach the Interrupts to their ISR's
  attachInterrupt(digitalPinToInterrupt(MOTOR_A), ISR_countA, RISING); // Increase counter A when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt(MOTOR_B), ISR_countB, RISING); // Increase counter B when speed sensor pin goes High
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  setupToF();
  setupWifI();
  setupOTA();
  Wire.begin();
  HCSR04.begin(triggerPin, echoPins, echoCount);
  compass.init();
  compass.setCalibration(-1122, 1852, -1481, 1468, -915, 1703);
  compass.setSmoothing(10, true);
  WebSerial.println("started");
  Serial.println("started");
}

void loop()
{

  ArduinoOTA.handle();
  // Read compass values
  compass.read();
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  double *distances = HCSR04.measureDistanceMm(25.2);
  int leftUsound = distances[0];
  int rightUsound = distances[1];

  if (botCommand != 0)
  {
    selectcommand(botCommand, measure, leftUsound, rightUsound, compass);
    botCommand = 0;
  }
}