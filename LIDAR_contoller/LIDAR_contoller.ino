//www.elegoo.com
//2017.12.12

/************************
Exercise the motor using
the L293D chip
************************/

#define ENABLE 4
#define DIRA 5
#define DIRB 6

#define NOTE_C5 523
#define NOTE_E5 659
#define NOTE_G5 784

#define BUTTON 2
#define BUZZER 8
int pot = A0; // assigns analog input A0 to variable pot
int pwmPin = 3; // assigns pin 12 to variable pwm
int melody[] = {
  NOTE_E5, NOTE_E5, 0, NOTE_E5, 0, NOTE_C5, NOTE_E5, 0, NOTE_G5
};

int noteDurations[] = {
  10, 10, 10, 10, 10, 10, 10, 10, 10
};
int i;
 
void setup() {
    for (int thisNote = 0; thisNote < 9; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER);
     pinMode(pot, INPUT);  
  }
  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);
   analogWrite(ENABLE,256); //enable on
    Serial.println("init done");
}

void loop() {
int c1;
int c2;
  //Serial.println("PWM");
  //---PWM example, full speed then slow
    c2= analogRead(pot); 
  c1= 1024-c2;         // subtracts c2 from 1000 ans saves the result in c1
  digitalWrite(pwmPin, HIGH); 
  delayMicroseconds(c1);   
  digitalWrite(pwmPin, LOW);  
  delayMicroseconds(c2);   
  Serial.println(c1);
 analogWrite(ENABLE,256); //enable on
   digitalWrite(DIRA,LOW); //one way
  delayMicroseconds(c1);   
   analogWrite(ENABLE,0); //enable on
  digitalWrite(DIRA,HIGH); //one way
    delayMicroseconds(c2); 
//  digitalWrite(DIRB,LOW);
//  delay(2000);
//  analogWrite(ENABLE,1); //half speed
//  delay(2000);
//  analogWrite(ENABLE,128); //half speed
//  delay(2000);
//  analogWrite(ENABLE,50); //half speed
//  delay(2000);
//  analogWrite(ENABLE,128); //half speed
//  delay(2000);
//  analogWrite(ENABLE,180); //half speed
//  delay(2000);
//  analogWrite(ENABLE,255); //half speed
//  delay(2000);
//  digitalWrite(ENABLE,LOW); //all done
 
}
   
