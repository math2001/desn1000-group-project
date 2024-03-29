#include <Servo.h>
#define PIN_ULTR_TRIG 9
#define PIN_ULTR_ECHO 10
#define PIN_LED 13
#define PIN_HSERVO 11

Servo servoh;

// hangle must be in degrees, between 0 and 180
void set_hangle(int hangle)
{
  // hangle in degrees
  /* Serial.print("; Rotating to"); */
  /* Serial.print(hangle, DEC); */
  /* Serial.println(); */
  hangle = constrain(hangle, 0, 180);

  servoh.write(hangle);
}

// the reason we use a global start is so that we know we won't wrap around
// before 49 days. Otherwise we might get unlucky and wrap during our run!
unsigned long global_start = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, 13);
  servoh.attach(PIN_HSERVO);

  pinMode(PIN_ULTR_TRIG, OUTPUT);
  pinMode(PIN_ULTR_ECHO, INPUT);

  Serial.println("reset");
  set_hangle(0);
  delay(1000);

  global_start = millis();
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


void measure(int hangle)
{
    Serial.print(hangle);
    Serial.print(" ");
    long long start = millis();
    long cm = measure_distance();

    Serial.print(cm);
    Serial.print(" ");
    Serial.print(millis() - global_start);
    Serial.println();
    long dt = 50 - (millis() - start);
    if (dt > 0) delay(dt);
}

#define ANGLE_LOWER (-75)
#define ANGLE_UPPER (75)
int hangle = 75;
int d = 3;

void loop() {
  hangle += d;
  if (hangle < ANGLE_LOWER || hangle > ANGLE_UPPER)
  {
    hangle = constrain(hangle, ANGLE_LOWER, ANGLE_UPPER);
    d *= -1;
  }

  set_hangle(90 + hangle);
  measure(hangle);
  if (Serial.available())
  {
    delay(200);
    int base_hangle = Serial.read() - 90;
    base_hangle = constrain(base_hangle, -80, 80);
    Serial.print("; base angle=");
    Serial.print(base_hangle, DEC);
    Serial.println();
    for (int hangle = base_hangle - 10; hangle <= base_hangle + 10; hangle += 1)
    {
      set_hangle(90 + hangle);
      measure(hangle);
    }
    delay(200);
  }
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
