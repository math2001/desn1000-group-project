#include <Servo.h>
#include <SPI.h>
#include <WiFi.h>
#define PIN_ULTR_TRIG 9
#define PIN_ULTR_ECHO 10
#define PIN_LED 13
#define PIN_HSERVO 11

Servo servoh;

char ssid[] = "localnetwork";
char pass[] = "password";

int status = WL_IDLE_STATUS;
char serverip[]="192.168.1.18"; // IP address of our server
int serverport = 4444; // port on which we connect

WiFiClient client;

// hangle must be in degrees, between 0 and 180
void set_hangle(int hangle)
{
  // hangle in degrees
  /* client.print("; Rotating to"); */
  /* client.print(hangle, DEC); */
  /* client.println(); */
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

  Serial.println("Connecting to wifi...");
  status = WiFi.begin(ssid, pass);
  if (status != WL_CONNECTED) {
    Serial.println("Connection to wifi failed");
    while(true); // block
  }
  Serial.println("Connected to wifi! Now connecting to the server");
  Serial.print("Server address: ");
  Serial.println(serverip);
  bool success = client.connect(serverip, serverport)
  if (!success) {
    Serial.println("couldn't connect to server...");
    while (true); // block
  }

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
    client.print(hangle);
    client.print(" ");
    long long start = millis();
    long cm = measure_distance();

    client.print(cm);
    client.print(" ");
    client.print(millis() - global_start);
    client.println();
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
  if (client.available())
  {
    delay(200);
    // read character (the angle in [0, 180] to set the camera to)
    int base_hangle = client.readStringUntil('\r')[0] - 90;
    base_hangle = constrain(base_hangle, -80, 80);
    client.print("; base angle=");
    client.print(base_hangle, DEC);
    client.println();
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
