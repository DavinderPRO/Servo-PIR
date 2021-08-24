#include "Arduino.h"
#include "DFPlayerMini_Fast.h"
#include "SoftwareSerial.h"
#include <Servo.h>

#define indicator (uint8_t)3 // the pin that the LED is atteched to
#define pirSensor (uint8_t)9 // the pin that the pirSensor is atteched to
#define servoPin (uint8_t)10 // SHould be digital, Prefer 9,10

#define mp3Tx (uint8_t)7 // to mp3 rx
#define mp3Rx (uint8_t)6 // to mp3 tx

#define servoSpeed (uint8_t)15    // the pin that the pirSensor is atteched to
#define EQ_Preset (uint8_t)2      //Rock
#define defaultVolume (uint8_t)27 // to mp3 tx
#define fileToPlay (uint16_t)1    // to mp3 tx
#define skipStep (uint16_t)2      // to mp3 tx

Servo servo;                             // create servo object to control a servo
SoftwareSerial softSerial(mp3Rx, mp3Tx); // RX, TX
DFPlayerMini_Fast myDFPlayer;
// Global varibles
uint8_t position = 0;           // Store the servo position
bool state = false;             // Store sensor state
bool mp3PlayerAvailable = true; // check mp3 player connected

inline void writeServoPosition()
{
  servo.write(position);
  delay(servoSpeed);
  // Serial.println(position);
}

inline void setupIndicators()
{
  pinMode(indicator, OUTPUT);   // initalize LED as an output
  pinMode(LED_BUILTIN, OUTPUT); // initalize LED as an output
}
inline void setIndicatorLed(bool level)
{
  digitalWrite(indicator, level);
  digitalWrite(LED_BUILTIN, level);
}

inline void setLedServoMp3player(bool state)
{
  setIndicatorLed(state);
  if (state)
  {
    setPlayerMp3Player(state);
    while (position < 180) // goes from 0 degrees to 180 degrees
    {
      position = position + skipStep;
      writeServoPosition();
    }
  }
  else
  {
    while (position > 0) // goes from 180 degrees to 0 degrees
    {
      position = position - skipStep;
      writeServoPosition();
    }
    setPlayerMp3Player(state);
  }
}

inline void blinkIndicator()
{
  setIndicatorLed(true);
  delay(500);
  setIndicatorLed(false);
  delay(500);
}

inline void setupMp3Player()
{
  softSerial.begin(9600); // initialize soft serial for Mp3 player
  if (!myDFPlayer.begin(softSerial, true))
  {
    mp3PlayerAvailable = false;
    // Serial.println("Unable to begin:");
    // Serial.println("1.Please recheck the connection!");
    // Serial.println("2.Please insert the SD card!");
  }
  else
  {
    myDFPlayer.EQSelect(EQ_Preset);
    myDFPlayer.volume(defaultVolume);
    setPlayerMp3Player(false);
    // Serial.println("DFPlayer Mini online.");
  }
}

inline void setPlayerMp3Player(bool play)
{
  if (mp3PlayerAvailable)
  {
    if (play)
    {
      myDFPlayer.wakeUp();
      myDFPlayer.loop(fileToPlay);
      // Serial.println("Looping mp3 file no.: ");
      // Serial.print(fileToPlay);
    }
    else
    {
      myDFPlayer.stop();
      myDFPlayer.sleep();
      // Serial.println(("Stopping mp3"));
    }
  }
}

// setup servo
inline void setupServo()
{
  servo.attach(servoPin);
  servo.write(position);
} // setup servo
inline void setupPIR()
{
  pinMode(pirSensor, INPUT); // initialize pirSensor as an input
}

void setup()
{
  // Serial.begin(9600); // initialize serial for debugging
  setupIndicators();
  setupPIR();
  setupServo();
  setupMp3Player();
  blinkIndicator();
}

void loop()
{
  if ((!state && digitalRead(pirSensor) == HIGH) || (state && digitalRead(pirSensor) == LOW))
  {
    state = !state;
    setLedServoMp3player(state);
  }
}
