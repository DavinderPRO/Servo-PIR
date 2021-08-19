#include "Arduino.h"
#include "DFPlayerMini_Fast.h"
#include "SoftwareSerial.h"
#include <Servo.h>

#define led (uint8_t)3       // the pin that the LED is atteched to
#define pirSensor (uint8_t)9 // the pin that the pirSensor is atteched to
#define servoPin (uint8_t)10 // SHould be digital, Prefer 9,10

#define mp3Tx (uint8_t)7 // to mp3 rx
#define mp3Rx (uint8_t)6 // to mp3 tx

#define servoSpeed (uint8_t)15    // the pin that the pirSensor is atteched to
#define EQ_Preset (uint8_t)2      //Rock
#define defaultVolume (uint8_t)27 // to mp3 tx
#define fileToPlay (uint16_t)1    // to mp3 tx

Servo servo;                             // create servo object to control a servo
SoftwareSerial softSerial(mp3Rx, mp3Tx); // RX, TX
DFPlayerMini_Fast myDFPlayer;

uint8_t position = 0; // variable to store the servo position
bool state = false;   // by default, no motion detected

void setup()
{
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(pirSensor, INPUT); // initialize pirSensor as an input
  servo.attach(servoPin);    // attaches the servo on pin 9 to the servo object
  servo.write(position);
  softSerial.begin(9600);
  Serial.begin(9600); // initialize serial
  if (!myDFPlayer.begin(softSerial, true))
  {
    Serial.println("Unable to begin:");
    Serial.println("1.Please recheck the connection!");
    Serial.println("2.Please insert the SD card!");

    while (true)
    {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println("DFPlayer Mini online.");
  myDFPlayer.EQSelect(EQ_Preset);
  myDFPlayer.volume(defaultVolume); //Set volume value. From 0 to 30
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(1000); // Give motor 2 second delay to reach 0 postion on start up
}

inline void writeServoPosition()
{
  servo.write(position);
  delay(servoSpeed);
}

void loop()
{
  if ((!state && digitalRead(pirSensor) == HIGH) || (state && digitalRead(pirSensor) == LOW))
  {
    state = !state;
    digitalWrite(led, state); // Flip LED
    if (state)
    {
      myDFPlayer.wakeUp();
      Serial.println(("sending MP3 loop"));
      myDFPlayer.loop(fileToPlay);
      while (position < 180) // goes from 0 degrees to 180 degrees
      {
        position = position + 5;
        Serial.println(position);
        writeServoPosition();
      }
    }
    else
    {
      while (position > 0) // goes from 180 degrees to 0 degrees
      {
        position = position - 5;
        Serial.println(position);
        writeServoPosition();
      }
      Serial.println(("Stopping mp3"));
      myDFPlayer.stop();
      myDFPlayer.sleep();
    }
  }
}
