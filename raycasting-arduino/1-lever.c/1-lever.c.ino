#include <Servo.h>
#define PIN_ULTR_TRIG 9
#define PIN_ULTR_ECHO 10
#define PIN_LED 13
#define PIN_HSERVO 11

Servo servoh;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, 13);
  servoh.attach(PIN_HSERVO);

  pinMode(PIN_ULTR_TRIG, OUTPUT);
  pinMode(PIN_ULTR_ECHO, INPUT);
  

  Serial.println("Init");

  // long duration, inches, cm;
  // for (int hangle = 0; hangle <= 180; hangle += 10)
  // {
  //   set_hangle(hangle);

  //   digitalWrite(PIN_ULTR_TRIG, LOW);
  //   delayMicroseconds(2);
  //   digitalWrite(PIN_ULTR_TRIG, HIGH);
  //   delayMicroseconds(10);
  //   digitalWrite(PIN_ULTR_TRIG, LOW);

  //   duration = pulseIn(PIN_ULTR_ECHO, HIGH);
  //   cm = duration * 0.034 / 2;
  //   Serial.print(cm);
  //   Serial.print("cm");
  //   Serial.println();

  //   delay(1000);
  // }
  // Serial.println("Done");
}

int measure_distance()
{
  digitalWrite(PIN_ULTR_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTR_TRIG, LOW);

  long duration = pulseIn(PIN_ULTR_ECHO, HIGH);
  return duration * 0.034 / 2;
}

void set_hangle(int hangle)
{
  // hangle in degrees
  // Serial.print("Rotating to");
  // Serial.print(hangle, DEC);
  // Serial.println();
  servoh.write(hangle);
}

void loop() {
  // return;
  for (int hangle = 0; hangle <= 360; hangle += 10)
  {
    if (hangle >= 180)
    {
      set_hangle(360 - hangle);
      Serial.print(360 - hangle);
    }
    else
    {
      set_hangle(hangle);
      Serial.print(hangle);
    }

    int cm = 0;
    
    Serial.print(" ");
    int num = 10;
    for (int i = 0; i < num; i++)
      cm += measure_distance();

    Serial.print(cm / num);
    // Serial.print("cm");
    Serial.println();
    delay(200);
  }
  
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
