#include <Servo.h>
Servo myservo; // create servo object to control a servo

#define led (uint8_t)13           // the pin that the LED is atteched to
#define pirSensor (uint8_t)2      // the pin that the pirSensor is atteched to
#define servoPin (uint8_t)9       // the pin that the pirSensor is atteched to
#define stepper_speed (uint8_t)15 // the pin that the pirSensor is atteched to

int sensorValue = 0;  // variable to store the pirSensor status (value)
uint8_t position = 0; // variable to store the servo position
bool state = LOW;     // by default, no motion detected

void setup()
{
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(pirSensor, INPUT); // initialize pirSensor as an input

  myservo.attach(servoPin); // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);       // initialize serial
}

void setServoMotor(bool state)
{
  if (state)
  {
    for (position = 0; position <= 180; position += 1) // goes from 0 degrees to 180 degrees
    {
      myservo.write(position); // tell servo to go to position in variable 'position'
      delay(stepper_speed);
    }
  }
  else
  {
    for (position = 180; position >= 0; position -= 1) // goes from 180 degrees to 0 degrees
    {
      myservo.write(position); // tell servo to go to position in variable 'position'
      delay(stepper_speed);
    }
  }
}

void loop()
{
  sensorValue = digitalRead(pirSensor); // read pirSensor value
  // check if the pirSensor is HIGH
  if (sensorValue == HIGH)
  {
    digitalWrite(led, HIGH); // turn LED ON
    delay(500);              // delay 100 milliseconds
    if (state == LOW)
    {
      Serial.println("Motion detected!");
      state = HIGH; // update variable state to HIGH
      setServoMotor(state);
    }
  }
  else
  {
    digitalWrite(led, LOW); // turn LED OFF
    delay(500);             // delay 200 milliseconds
    if (state == HIGH)
    {
      Serial.println("Motion stopped!");
      state = LOW; // update variable state to LOW
      setServoMotor(state);
    }
  }
}