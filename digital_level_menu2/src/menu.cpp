#include <Arduino.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SPITFT.h>
//#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>


#include "OneButton.h"
#include "MenuBackend.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
const char PUSHBUTTON = 3;    // PD2/INT0 menu pushbutton pin 
bool displayChanged=false;
// instantiation of a OneButton object identified as “button”
OneButton button(PUSHBUTTON, true);  


//settings variable
// our 2 parameters to set using the menu
//desired angle for offset mode
int desiredAngle = 0;
//Overal digital level mode
String Mode = "Standard ";
//level sensitivity 
int sensitivity = 0.1;
//level offset
int offset=0;

//Menu variables
//flag if user is in menu
bool inMenu = false; 
//OLED output string
String msg;
//count long press time 
long datum;
//lastTime a button was pressed in a menu
unsigned long lastTime=millis();
//notifier to update clock graphic
bool updateClock =false;
// menu timeout duration
unsigned long menuTimeout=5000;
//
bool senSetting=false;


//graphics stuff
const uint16_t clockOriginX=119; //120;
const uint16_t clockOriginY=23; //24;
const uint16_t clockRadius=8; //24;
//Radians in a circla (PI*2)
const float RADIANS = 6.28318530; 
/* ************************************************************************************************************************************
 MENU
**************************************************************************************************************************************/
/*
  This is the structure of the menu
  
  Mode
    Standard
    Ofsset
  Desired angle
    int from 0-90
*/

void menuUseEvent(MenuUseEvent used)
{
  //MenuItem currentMenu = menu.getCurrent();
  Serial.print("Menu use ");
  Serial.println(used.item.getName());

  if (used.item == "Level Sensitivity >v") {
    //latch the sensitivity setting mode
    senSetting=true;
    Serial.print("Sensitivity mode selected ");
  }

  
  if (used.item == "Delay") //comparison agains a known item
  {
    Serial.println("menuUseEvent found Delay (D)");
}

}

void menuChangeEvent(MenuChangeEvent changed)
{
  Serial.print("Menu change ");
  Serial.print(changed.from.getName());
  Serial.print(">");
  Serial.println(changed.to.getName());
}

// Menu items instantiation
MenuBackend menu = MenuBackend(menuUseEvent,menuChangeEvent);

//beneath is list of menu items needed to build the menu
MenuItem MMode            = MenuItem("Mode              >v");
  MenuItem MStandard      = MenuItem("Standard level     v");
  MenuItem MOffset        = MenuItem("Offset level       v");
  MenuItem MProtractor    = MenuItem("Protractor         v");
MenuItem MProtractorAxis  = MenuItem("Protractor Axis   >v");
  MenuItem MPRoll         = MenuItem("Axis: Roll         v");
  MenuItem MPPitch        = MenuItem("Axis: Pitch        v");
  MenuItem MPYaw          = MenuItem("Axis: Yaw          v");
MenuItem MSensitivity     = MenuItem("Level Sensitivity >v");
MenuItem MOffsetSetting   = MenuItem("Offset setting    >v");
  MenuItem M10degrees     = MenuItem("10 degrees         v");
  MenuItem M20degrees     = MenuItem("20 degrees         v");
  MenuItem M30degrees     = MenuItem("30 degrees         v");


void menuSetup()
{
  Serial.println(F("Setting up menu..."));
  menu.getRoot().add(MMode);
    //add the first mode setting 
    MMode.addRight(MStandard);
      //loop up and down mode options
      MStandard.addBefore(MOffset);
      MStandard.addAfter(MProtractor);
      MProtractor.addAfter(MOffset);
      MOffset.addAfter(MStandard);
      //go back up menu
      MStandard.addLeft(MMode);
      MOffset.addLeft(MMode);
  MMode.addAfter(MProtractorAxis);
      MProtractorAxis.addRight(MPRoll);
      //loop up and down degree settings
      MPRoll.addBefore(MPRoll);//loop to 30
      MPRoll.addAfter(MPPitch);
      MPPitch.addAfter(MPYaw);  
      MPYaw.addAfter(MPRoll);//loopback to 10
  MProtractorAxis.addAfter(MOffsetSetting);
  //loop arond the root options      
  MProtractorAxis.addAfter(MSensitivity);

  
  MSensitivity.addAfter(MOffsetSetting);
  //sthe sensitivity setting is a small enough range to hand code all the reasonable values
    //MSensitivity.addRight(MSensSetting);  
  MMode.addBefore(MOffsetSetting);
    //add first offset setting 
    //(probably change this to some kind of iterator so we dont have to set every value 0-90 degrees  
    MOffsetSetting.addRight(M10degrees);
      //loop up and down degree settings
      M10degrees.addBefore(M10degrees);//loop to 30
      M10degrees.addAfter(M20degrees);
      M20degrees.addAfter(M30degrees);  
      M30degrees.addAfter(M10degrees);//loopback to 10
  MOffsetSetting.addAfter(MMode);
}
//draws menu lines
void updateDisplay(String message, byte row){
  if (displayChanged==true){
  display.clearDisplay();
  display.setCursor(0,row*8);
  display.print(message);
  display.display();
  //now we have done display stuff reset the display update state so we dont do it again till something else changes it.
  displayChanged=false;
  }
}

//draws numeric values
void updateSpinner(String message, byte row){
  if (displayChanged==true){
  //blank the numeric area, draw a rectangle from the text row top, x=0 to text baseline x=64 
  display. fillRect(0,row*8,64, (row+1)*8, 0);
  display.setCursor(0,row*8);
  display.print(message);
  display.display();
  //now we have done display stuff reset the display update state so we dont do it again till something else changes it.
  displayChanged=false;
  }
}
/* ************************************************************************************************************************************
BUTTON EVENT HANDLERS
**************************************************************************************************************************************/

void buttonClick() {
  //move down through menu items
     Serial.println(F("Right"));
  if (inMenu) {
    menu.moveRight();
    msg=menu.getCurrent().getName();
    displayChanged=true;
    //store the last time button was used
    lastTime=millis();
    //todo if in Offset setting use accelerating incrementor
  }
}

void buttonDoubleclick() {
  //move right into submenu
   Serial.println(F("Select"));
  if (inMenu) {
    menu.use();
    //todo if already in a leaf go left

    //If a selection is made set the timeout value to 0 so we exit menu immediately
    lastTime=0;
  }
}

void buttonPress() {
   //if already in menu, then move down, this needs to be before the 'enter menu' condition below,
   //or both condtions will be executed causing problems.
  if (inMenu) {
    Serial.println(F("Down"));
    menu.moveDown();
    msg=menu.getCurrent().getName();
    displayChanged=true;
  }
  //long press is used to enter the menu
  else{
    inMenu=true;
    //we first enter menu in MenuRoot so do a move down to get the first entry
    menu.moveDown();  
    msg=menu.getCurrent().getName();
    displayChanged=true;
  }
  //store the last time button was used
  lastTime=millis(); 
  updateDisplay(msg,0);
  Serial.println(button.getPressedTicks());
}

void longPress() {
  MenuItem currentMenu = menu.getCurrent();
  //todo use this for accelerating degree setting
 datum =datum+1;
 lastTime=millis(); 
  if (senSetting==true) {
      updateSpinner("test " + datum , 3);
      }
   Serial.println(datum);
   //Serial.println(button.isLongPressed());
}
void buttonSetup() {
  
  button.attachClick(buttonClick);
  button.attachDoubleClick(buttonDoubleclick);
  button.attachLongPressStart(buttonPress);
  button.attachDuringLongPress(longPress);
  // these are optional parameters to fine-tune the behavior. Values are in ms. Given values here are default values.
  button.setClickTicks(600);
  button.setPressTicks(1000);  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting...");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();

 // display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  menuSetup();
  buttonSetup();
  Serial.println(F("Starting navigation:\r\nUp: w   Down: -   Left: .   Right:    Use: .."));
  Serial.println(F("Starting navigation:\r\nUp: w   Down: s   Left: a   Right: d   Use: e"));
}


void drawClock(float clockRadians){
  //drawCircle(centre x, centre y, radius, colour(1 is 'white' on monchrome displays))
  //we draw the clock edge evey time in case the user changed the menu and it got cleared
  display.drawCircle(clockOriginX, clockOriginY, clockRadius, 1);
  //calculate endpoint of the clock hand
  clockRadians=clockRadians-(RADIANS/4);
  uint16_t  clockHandX = (cos(clockRadians)*clockRadius)+clockOriginX  ;
  uint16_t  clockHandY = (sin(clockRadians)*clockRadius)+clockOriginY ;
  //draw hand
  display.drawLine(clockOriginX,clockOriginY,clockHandX,clockHandY,1);
    display.display();
  //now we have done clock stuff reset the clock update state so we dont do it again immediatly
  updateClock=0;
}



void loop() {
  // put your main code here, to run repeatedly:
  button.tick(); // checks regularly if the button is pressed or not and raises events according to the finite state
  if (Serial.available()) {
    byte read = Serial.read();
    switch (read) {
      case 'w': menu.moveUp(); break;
      case 's': menu.moveDown(); break;
      case 'd': menu.moveRight(); break;
      case 'a': menu.moveLeft(); break;
      case 'e': menu.use(); break;
    }  
  }
  
  if((button.isLongPressed()==false) && (datum > 0)){
    datum=0;
  }
  
  //if we are in a menu but no button has been pressed for 5 seconds I guess the user is done with the menu
  if(inMenu && lastTime+menuTimeout < millis()){
    inMenu= false;
    //as we are leaving we need to reset to menu root.
    menu.toRoot();
    senSetting=false;
    Serial.println(F("Menu timeout"));
  }

  //If required draw menu timeout clock
  if(inMenu && lastTime+menuTimeout > millis()){
    //set clock update state so the clock function knows to update
    updateClock= true;
    //calculate elapsed time since a button was pressed
    long elapsedTime = (millis()-lastTime);
    //scale menu timeout to radians
    double clockDegrees =  (RADIANS * elapsedTime / menuTimeout);
    //draw the clock
    drawClock(clockDegrees);
  }
  
  //do operations when not in menu (running is dummy display until integrated)
  if(inMenu == false && msg != "running"){
    msg="running";
    displayChanged=true;
  }
  updateDisplay(msg,0);
}
