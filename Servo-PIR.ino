#include <Servo.h>
Servo myservo; // create servo object to control a servo

#define led (uint8_t)13           // the pin that the LED is atteched to
#define pirSensor (uint8_t)2      // the pin that the pirSensor is atteched to
#define servoPin (uint8_t)9       // the pin that the pirSensor is atteched to
#define stepper_speed (uint8_t)15 // the pin that the pirSensor is atteched to

uint8_t position = 0; // variable to store the servo position
bool state = false;   // by default, no motion detected

void setup()
{
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(pirSensor, INPUT); // initialize pirSensor as an input
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(position);
  delay(1000); // Give motor 1 second delay to reach 0 postion on start up
  // Serial.begin(9600); // initialize serial
}
void writeServoPosition()
{
  myservo.write(position);
  delay(stepper_speed);
}
void loop()
{
  if ((!state && digitalRead(pirSensor) == HIGH) || (state && digitalRead(pirSensor) == LOW))
  {
    state = !state;
    digitalWrite(led, state);       // Flip LED
    while (state && position < 180) // goes from 0 degrees to 180 degrees
    {
      position++;
      // Serial.println(position);
      writeServoPosition();
    }
    while (!state && position > 0) // goes from 180 degrees to 0 degrees
    {
      position--;
      // Serial.println(position);
      writeServoPosition();
    }
  }
}