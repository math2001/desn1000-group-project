#include <Servo.h>

#define LED_PIN 13

Servo servo1;

#define SERVO_PIN 11

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Finished init");

  servo1.attach(SERVO_PIN, 544, 2400);

  pinMode(LED_PIN, INPUT);
}

bool f = 0;

void loop() {
  if (Serial.available())
  {
    int c = Serial.read();
    if (c == 'a')
    {
      Serial.println("parsing number");
      int v = Serial.parseInt();
      // v = map(v, 0, 100, 0, 180);
      // v = v % 181;

      Serial.print("set servo to ");
      Serial.print(v, DEC);
      Serial.println();
      servo1.write(v);
    }
    
  }
  delay(10);

}
