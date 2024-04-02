#include <Servo.h>

// TODO: fix up pin numbers (right now they overlap/go to zero)

#define PIN_ULTR_TRIG 9
#define PIN_ULTR_ECHO 10
#define PIN_LED 13
#define PIN_HSERVO 11

// no enable pins
#define MOTOR_FRONT_LEFT_1 10
#define MOTOR_FRONT_LEFT_2 11

#define MOTOR_FRONT_RIGHT_1 9
#define MOTOR_FRONT_RIGHT_2 3

#define MOTOR_BACK_LEFT_1 0
#define MOTOR_BACK_LEFT_2 0

#define MOTOR_BACK_RIGHT_1 0
#define MOTOR_BACK_RIGHT_2 0

// movement types
#define MOVEMENT_STOP 0
#define MOVEMENT_FORWARDS 1
#define MOVEMENT_BACKWARDS 2
#define MOVEMENT_TURN_LEFT 3
#define MOVEMENT_TURN_RIGHT 4
#define MOVEMENT_SLIDE_LEFT 5
#define MOVEMENT_SLIDE_RIGHT 6

Servo servoh;

int speed_percentage = 50;
int movement = MOVEMENT_STOP;

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

  Serial.print("PIN_ULTR_TRIG=");
  Serial.println(PIN_ULTR_TRIG, DEC);
  pinMode(PIN_ULTR_TRIG, OUTPUT);
  Serial.print("PIN_ULTR_ECHO=");
  Serial.println(PIN_ULTR_ECHO, DEC);
  pinMode(PIN_ULTR_ECHO, INPUT);

  Serial.print("MOTOR_FRONT_LEFT_1=");
  Serial.println(MOTOR_FRONT_LEFT_1, DEC);
  Serial.print("MOTOR_FRONT_LEFT_2=");
  Serial.println(MOTOR_FRONT_LEFT_2, DEC);

  Serial.print("MOTOR_FRONT_RIGHT_1=");
  Serial.println(MOTOR_FRONT_RIGHT_1, DEC);
  Serial.print("MOTOR_FRONT_RIGHT_2=");
  Serial.println(MOTOR_FRONT_RIGHT_2, DEC);

  Serial.print("Initialising the motors... ");
  pinMode(MOTOR_FRONT_LEFT_1, OUTPUT);
  pinMode(MOTOR_FRONT_LEFT_2, OUTPUT);
  pinMode(MOTOR_FRONT_RIGHT_1, OUTPUT);
  pinMode(MOTOR_FRONT_RIGHT_2, OUTPUT);
  Serial.println("Done");

  pinMode(MOTOR_BACK_LEFT_1, OUTPUT);
  pinMode(MOTOR_BACK_LEFT_2, OUTPUT);
  pinMode(MOTOR_BACK_RIGHT_1, OUTPUT);
  pinMode(MOTOR_BACK_RIGHT_2, OUTPUT);

  Serial.print("Turning everything off... ");
  digitalWrite(MOTOR_FRONT_LEFT_1, LOW);
  digitalWrite(MOTOR_FRONT_LEFT_2, LOW);
  digitalWrite(MOTOR_FRONT_RIGHT_1, LOW);
  digitalWrite(MOTOR_FRONT_RIGHT_2, LOW);

  digitalWrite(MOTOR_BACK_LEFT_1, LOW);
  digitalWrite(MOTOR_BACK_LEFT_2, LOW);
  digitalWrite(MOTOR_BACK_RIGHT_1, LOW);
  digitalWrite(MOTOR_BACK_RIGHT_2, LOW);
  Serial.println("Done");

  Serial.println("Initialisation done");

  set_hangle(0);
  delay(1000);

  Serial.println("Ready");

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


long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void front_left_forwards()
{
  analogWrite(MOTOR_FRONT_LEFT_1, speed_percentage * 255 / 100);
  digitalWrite(MOTOR_FRONT_LEFT_2, LOW);
  Serial.print("  => motor front left pin 1: ");
  Serial.print(speed_percentage * 255 / 100, DEC);
  Serial.println("  motor front left pin 2: LOW");
}

void front_left_backwards()
{
  digitalWrite(MOTOR_FRONT_LEFT_1, LOW);
  analogWrite(MOTOR_FRONT_LEFT_2, speed_percentage * 255 / 100);
  Serial.print("  => motor front left pin 1: LOW   motor front left pin 2: ");
  Serial.println(speed_percentage * 255 / 100, DEC);
}

void front_left_stop()
{
  digitalWrite(MOTOR_FRONT_LEFT_1, LOW);
  digitalWrite(MOTOR_FRONT_LEFT_2, LOW);
  Serial.println("  => motor front left pin 1: LOW   motor front left pin 2: LOW");
}

void front_right_forwards()
{
  analogWrite(MOTOR_FRONT_RIGHT_1, speed_percentage * 255 / 100);
  digitalWrite(MOTOR_FRONT_RIGHT_2, LOW);
  Serial.print("  => motor front right pin 1: ");
  Serial.print(speed_percentage * 255 / 100, DEC);
  Serial.println("  motor front right pin 2: LOW");
}

void front_right_backwards()
{
  digitalWrite(MOTOR_FRONT_RIGHT_1, LOW);
  analogWrite(MOTOR_FRONT_RIGHT_2, speed_percentage * 255 / 100);
  Serial.print("  => motor front right pin 1: LOW   motor front right pin 2: ");
  Serial.println(speed_percentage * 255 / 100, DEC);
}

void front_right_stop()
{
  digitalWrite(MOTOR_FRONT_RIGHT_1, LOW);
  digitalWrite(MOTOR_FRONT_RIGHT_2, LOW);
  Serial.println("  => motor front right pin 1: LOW   motor front right pin 2: LOW");
}

void back_left_forwards()
{
  analogWrite(MOTOR_BACK_LEFT_1, speed_percentage * 255 / 100);
  digitalWrite(MOTOR_BACK_LEFT_2, LOW);
  Serial.print("  => motor back left pin 1: ");
  Serial.print(speed_percentage * 255 / 100, DEC);
  Serial.println("  motor back left pin 2: LOW");
}

void back_left_backwards()
{
  digitalWrite(MOTOR_BACK_LEFT_1, LOW);
  analogWrite(MOTOR_BACK_LEFT_2, speed_percentage * 255 / 100);
  Serial.print("  => motor back left pin 1: LOW   motor back left pin 2: ");
  Serial.println(speed_percentage * 255 / 100, DEC);
}

void back_left_stop()
{
  digitalWrite(MOTOR_BACK_LEFT_1, LOW);
  digitalWrite(MOTOR_BACK_LEFT_2, LOW);
  Serial.println("  => motor back left pin 1: LOW   motor back left pin 2: LOW");
}

void back_right_forwards()
{
  analogWrite(MOTOR_BACK_RIGHT_1, speed_percentage * 255 / 100);
  digitalWrite(MOTOR_BACK_RIGHT_2, LOW);
  Serial.print("  => motor back right pin 1: ");
  Serial.print(speed_percentage * 255 / 100, DEC);
  Serial.println("  motor back right pin 2: LOW");
}

void back_right_backwards()
{
  digitalWrite(MOTOR_BACK_RIGHT_1, LOW);
  analogWrite(MOTOR_BACK_RIGHT_2, speed_percentage * 255 / 100);
  Serial.print("  => motor back right pin 1: LOW   motor back right pin 2: ");
  Serial.println(speed_percentage * 255 / 100, DEC);
}

void back_right_stop()
{
  digitalWrite(MOTOR_BACK_RIGHT_1, LOW);
  digitalWrite(MOTOR_BACK_RIGHT_2, LOW);
  Serial.println("  => motor back right pin 1: LOW   motor back right pin 2: LOW");
}

void implement_movement()
{
  if (movement == MOVEMENT_STOP)
  {
    front_left_stop();
    front_right_stop();
    back_left_stop();
    back_right_stop();
  }
  else if (movement == MOVEMENT_FORWARDS)
  {
    front_left_forwards();
    front_right_forwards();
    back_left_forwards();
    back_right_forwards();
  }
  else if (movement == MOVEMENT_BACKWARDS)
  {
    front_left_backwards();
    front_right_backwards();
    back_left_backwards();
    back_right_backwards();
  }
  else if (movement == MOVEMENT_TURN_LEFT)
  {
    front_left_backwards();
    back_left_backwards();

    front_right_forwards();
    back_right_forwards();

  }
  else if (movement == MOVEMENT_TURN_RIGHT)
  {
    front_right_backwards();
    back_right_backwards();

    front_left_forwards();
    back_left_forwards();
  }
  else if (movement == MOVEMENT_SLIDE_LEFT)
  {
    front_right_forwards();
    back_right_backwards();

    front_left_backwards();
    back_left_forwards();
  }
  else if (movement == MOVEMENT_SLIDE_RIGHT)
  {
    front_right_backwards();
    back_right_forwards();

    front_left_forwards();
    back_left_backwards();
  }
  else
  {
    Serial.println("assert false; unknown movement");
  }
}

// read the a positive number between 0 and 180 followed by a dot
// returns -1 on invalid input
int read_int_till_dot()
{
    int n = 0;
    while (true) {
        while (!Serial.available());
        int c = Serial.read();
        if (c == '.') {
            return n;
        }
        if ('0' <= c && c <= '9') {
            n = n * 10 + (c - '0');
        } else {
            return -1;
        }

    }
    return n;
}

void handle_input(int byte)
{
    if (byte == 'f')
    {
        movement = MOVEMENT_FORWARDS;
        Serial.println("Forwards");
    }
    else if (byte == 'b')
    {
        movement = MOVEMENT_BACKWARDS;
        Serial.println("Backwards");
    }
    else if (byte == 'l')
    {
        movement = MOVEMENT_TURN_LEFT;
        Serial.println("Turn Left");
    }
    else if (byte == 'r')
    {
        movement = MOVEMENT_TURN_RIGHT;
        Serial.println("Turn Right");
    }
    else if (byte == 'L')
    {
        movement = MOVEMENT_SLIDE_LEFT;
        Serial.println("Slide Left");
    }
    else if (byte == 'R')
    {
        movement = MOVEMENT_SLIDE_RIGHT;
        Serial.println("Slide Right");
    }
    else if (byte == '0')
    {
        movement = MOVEMENT_STOP;
        Serial.println("Stop");
    }
    else if ('1' <= byte && byte <= '5')
    {
        // bit of ASCII trickery
        speed_percentage = (byte - '0') * 10;
        Serial.print("Speed = ");
        Serial.print(speed_percentage, DEC);
        Serial.println("%");
    }
    else if (byte == 's')
    {
        // Stop the vehicle
        movement = MOVEMENT_STOP;
        implement_movement();

        // set sensor rotation
        int angle = read_int_till_dot();
        if (angle < 0)
        {
            Serial.println("; invalid angle");
        }
        else
        {
            int base_hangle = constrain(angle - 90, -80, 80);
            Serial.print("; base angle=");
            Serial.print(base_hangle, DEC);
            Serial.println();

            // scan 10 deg around specified angle
            delay(200);
            for (int hangle = base_hangle - 10; hangle <= base_hangle + 10; hangle += 1)
            {
                set_hangle(90 + hangle);
                measure(hangle);
            }
            delay(200);
        }
    }
    else
    {
        Serial.print("Unknown command '");
        Serial.print(byte);
        Serial.println("' For safety, the vehicule was stopped");
        movement = MOVEMENT_STOP;
    }

    implement_movement();
}

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
    handle_input(Serial.read());
  }
  delay(200);
}