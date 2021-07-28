#include <Servo.h>
Servo myservo; // create servo object to control a servo

#define led (uint8_t)13           // the pin that the LED is atteched to
#define pirSensor (uint8_t)2      // the pin that the pirSensor is atteched to
#define servoPin (uint8_t)9       // the pin that the pirSensor is atteched to
#define stepper_speed (uint8_t)15 // the pin that the pirSensor is atteched to

int sensorValue = 0;  // variable to store the pirSensor status (value)
uint8_t position = 0; // variable to store the servo position
int state = LOW;      // by default, no motion detected

void setup()
{
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(pirSensor, INPUT); // initialize pirSensor as an input
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(position);
  // Serial.begin(9600); // initialize serial
}

void setServoMotor(int state)
{

  while (state == HIGH && position < 180) // goes from 0 degrees to 180 degrees
  {
    position++;
    // Serial.println(position);
    myservo.write(position); // tell servo to go to position in variable 'position'
    delay(stepper_speed);
  }

  while (state == LOW && position > 0) // goes from 180 degrees to 0 degrees
  {
    position--;
    // Serial.println(position);
    myservo.write(position);
    delay(stepper_speed);
  }
}

void loop()
{
  sensorValue = digitalRead(pirSensor); // read pirSensor value
  // check if the pirSensor is HIGH
  if (sensorValue == HIGH)
  {
    digitalWrite(led, HIGH); // turn LED ON
    delay(100);              // delay 100 milliseconds

    if (state == LOW)
    {
      // Serial.println("Motion detected!");
      state = HIGH; // update variable state to HIGH
      setServoMotor(state);
    }
  }
  else
  {
    digitalWrite(led, LOW); // turn LED OFF
    delay(200);             // delay 200 milliseconds
    if (state == HIGH)
    {
      // Serial.println("Motion stopped!");
      state = LOW; // update variable state to LOW
      setServoMotor(state);
    }
  }
}