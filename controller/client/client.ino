#include <Servo.h>
#define COMMENT true

// TODO: fix up pin numbers (right now they overlap/go to zero)

#define PIN_ULTR_TRIG 51
#define PIN_ULTR_ECHO 53
#define PIN_LED 13

#define PIN_CAMERA_HORIZONTAL_SERVO 49
#define PIN_CLAW 46
#define PIN_Z_AXIS_1 47
#define PIN_Z_AXIS_2 48

// no enable pins
#define MOTOR_FRONT_LEFT_1 4
#define MOTOR_FRONT_LEFT_2 5

#define MOTOR_FRONT_RIGHT_1 2
#define MOTOR_FRONT_RIGHT_2 3

#define MOTOR_BACK_LEFT_1 9
#define MOTOR_BACK_LEFT_2 8

#define MOTOR_BACK_RIGHT_1 7
#define MOTOR_BACK_RIGHT_2 6

// movement types
#define MOVEMENT_STOP 0
#define MOVEMENT_FORWARDS 1
#define MOVEMENT_BACKWARDS 2
#define MOVEMENT_TURN_LEFT 3
#define MOVEMENT_TURN_RIGHT 4
#define MOVEMENT_SLIDE_LEFT 5
#define MOVEMENT_SLIDE_RIGHT 6


#define Z_AXIS_TIME_RISE 200 // in ms

Servo servo_camera;
Servo servo_claw;

int speed_percentage = 100;
int movement = MOVEMENT_STOP;
bool slow_mode = false;
bool observe = false;

// hangle must be in degrees, between 0 and 180
void set_hangle(int hangle)
{
  // hangle in degrees
  /* Serial.print("; Rotating to"); */
  /* Serial.print(hangle, DEC); */
  /* Serial.println(); */
  hangle = constrain(hangle, 0, 180);

  servo_camera.write(hangle);
}

// the reason we use a global start is so that we know we won't wrap around
// before 49 days. Otherwise we might get unlucky and wrap during our run!
unsigned long global_start = 0;


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

#define CLAW_ANGLE_MIN (0)  // closed
#define CLAW_ANGLE_MAX (90) // wide open
#define CLAW_ANGLE_DELTA (2)
// moves between -90 and 90
int claw_angle = 0;


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
  #ifdef COMMENT
  Serial.print(";  => motor front left pin 1: ");
  Serial.print(speed_percentage * 255 / 100, DEC);
  Serial.println(";  motor front left pin 2: LOW");
  #endif
}

void front_left_backwards()
{
  digitalWrite(MOTOR_FRONT_LEFT_1, LOW);
  analogWrite(MOTOR_FRONT_LEFT_2, speed_percentage * 255 / 100);
  #ifdef COMMENT
  Serial.print(";  => motor front left pin 1: LOW   motor front left pin 2: ");
  Serial.println(speed_percentage * 255 / 100, DEC);
  #endif
}

void front_left_stop()
{
  digitalWrite(MOTOR_FRONT_LEFT_1, LOW);
  digitalWrite(MOTOR_FRONT_LEFT_2, LOW);
  #ifdef COMMENT
  Serial.println(";  => motor front left pin 1: LOW   motor front left pin 2: LOW");
  #endif
}

void front_right_forwards()
{
  analogWrite(MOTOR_FRONT_RIGHT_1, speed_percentage * 255 / 100);
  digitalWrite(MOTOR_FRONT_RIGHT_2, LOW);
  #ifdef COMMENT
  Serial.print(";  => motor front right pin 1: ");
  Serial.print(speed_percentage * 255 / 100, DEC);
  Serial.println("  motor front right pin 2: LOW");
  #endif
}

void front_right_backwards()
{
  digitalWrite(MOTOR_FRONT_RIGHT_1, LOW);
  analogWrite(MOTOR_FRONT_RIGHT_2, speed_percentage * 255 / 100);
  #ifdef COMMENT
  Serial.print(";  => motor front right pin 1: LOW   motor front right pin 2: ");
  Serial.println(speed_percentage * 255 / 100, DEC);
  #endif
}

void front_right_stop()
{
  digitalWrite(MOTOR_FRONT_RIGHT_1, LOW);
  digitalWrite(MOTOR_FRONT_RIGHT_2, LOW);
  #ifdef COMMENT
    Serial.println(";  => motor front right pin 1: LOW   motor front right pin 2: LOW");
  #endif
}

void back_left_forwards()
{
  analogWrite(MOTOR_BACK_LEFT_1, speed_percentage * 255 / 100);
  digitalWrite(MOTOR_BACK_LEFT_2, LOW);
  #ifdef COMMENT
  Serial.print(";  => motor back left pin 1: ");
  Serial.print(speed_percentage * 255 / 100, DEC);
  Serial.println("  motor back left pin 2: LOW");
  #endif
}

void back_left_backwards()
{
  digitalWrite(MOTOR_BACK_LEFT_1, LOW);
  analogWrite(MOTOR_BACK_LEFT_2, speed_percentage * 255 / 100);
  #ifdef COMMENT
  Serial.print(";  => motor back left pin 1: LOW   motor back left pin 2: ");
  Serial.println(speed_percentage * 255 / 100, DEC);
  #endif
}

void back_left_stop()
{
  digitalWrite(MOTOR_BACK_LEFT_1, LOW);
  digitalWrite(MOTOR_BACK_LEFT_2, LOW);
  #ifdef COMMENT
  Serial.println(";  => motor back left pin 1: LOW   motor back left pin 2: LOW");
  #endif
}

void back_right_forwards()
{
  analogWrite(MOTOR_BACK_RIGHT_1, speed_percentage * 255 / 100);
  digitalWrite(MOTOR_BACK_RIGHT_2, LOW);
  #ifdef COMMENT
  Serial.print(";  => motor back right pin 1: ");
  Serial.print(speed_percentage * 255 / 100, DEC);
  Serial.println("  motor back right pin 2: LOW");
  #endif
}

void back_right_backwards()
{
  digitalWrite(MOTOR_BACK_RIGHT_1, LOW);
  analogWrite(MOTOR_BACK_RIGHT_2, speed_percentage * 255 / 100);
  #ifdef COMMENT
  Serial.print(";  => motor back right pin 1: LOW   motor back right pin 2: ");
  Serial.println(speed_percentage * 255 / 100, DEC);
  #endif
}

void back_right_stop()
{
  digitalWrite(MOTOR_BACK_RIGHT_1, LOW);
  digitalWrite(MOTOR_BACK_RIGHT_2, LOW);
  #ifdef COMMENT
  Serial.println(";  => motor back right pin 1: LOW   motor back right pin 2: LOW");
  #endif
}

void implement_claw()
{
  servo_claw.write(claw_angle);
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
    }
    else if (byte == 'b')
    {
        movement = MOVEMENT_BACKWARDS;
    }
    else if (byte == 'l')
    {
        movement = MOVEMENT_TURN_LEFT;
    }
    else if (byte == 'r')
    {
        movement = MOVEMENT_TURN_RIGHT;
    }
    else if (byte == 'L')
    {
        movement = MOVEMENT_SLIDE_LEFT;
    }
    else if (byte == 'R')
    {
        movement = MOVEMENT_SLIDE_RIGHT;
    }
    else if (byte == '0')
    {
        movement = MOVEMENT_STOP;
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
    else if (byte == 'm')
    {
      slow_mode = !slow_mode;
    }
    else if (byte == 'o')
    {
      Serial.println("; start observing");
      if (observe)
      {
        observe = false;
        hangle = 0;
        set_hangle(90 + hangle);
      }
      else
      {
        observe = true;
        hangle = 75;
      }
    }
    else if (byte == 'c')
    {
      int claw_angle = read_int_till_dot();
      if (claw_angle < CLAW_ANGLE_MIN)
      {
        claw_angle = CLAW_ANGLE_MIN;
      }
      if (claw_angle > CLAW_ANGLE_MAX)
      {
        claw_angle = CLAW_ANGLE_MAX;
      }
      servo_claw.write(claw_angle);
      #ifdef COMMENT
      Serial.print("; update claw angle");
      Serial.println(claw_angle);
      #endif
    }
    else if (byte == 'i')
    {
      digitalWrite(PIN_Z_AXIS_1, HIGH);
      digitalWrite(PIN_Z_AXIS_2, LOW);
      delay(Z_AXIS_TIME_RISE);
      digitalWrite(PIN_Z_AXIS_1, LOW);
      digitalWrite(PIN_Z_AXIS_2, LOW);
      #ifdef COMMENT
      Serial.println("; rise");
      #endif
    }
    else if (byte == 'k')
    {
      digitalWrite(PIN_Z_AXIS_1, LOW);
      digitalWrite(PIN_Z_AXIS_2, HIGH);
      delay(Z_AXIS_TIME_RISE);
      digitalWrite(PIN_Z_AXIS_1, LOW);
      digitalWrite(PIN_Z_AXIS_2, LOW);
      #ifdef COMMENT
      Serial.println("; lower");
      #endif
    }
    else
    {
        Serial.print("Unknown command '");
        Serial.print(byte);
        Serial.println("' For safety, the vehicule was stopped");
        movement = MOVEMENT_STOP;
    }

    implement_movement();
    if (slow_mode)
    {
      delay(100);
      movement = MOVEMENT_STOP;
      implement_movement();
    }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, 13);
  servo_camera.attach(PIN_CAMERA_HORIZONTAL_SERVO);
  servo_claw.attach(PIN_CLAW);

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

  pinMode(PIN_Z_AXIS_1, OUTPUT);
  pinMode(PIN_Z_AXIS_2, OUTPUT);

  Serial.print("Initialising the motors... ");
  pinMode(MOTOR_FRONT_LEFT_1, OUTPUT);
  pinMode(MOTOR_FRONT_LEFT_2, OUTPUT);
  pinMode(MOTOR_FRONT_RIGHT_1, OUTPUT);
  pinMode(MOTOR_FRONT_RIGHT_2, OUTPUT);

  pinMode(MOTOR_BACK_LEFT_1, OUTPUT);
  pinMode(MOTOR_BACK_LEFT_2, OUTPUT);
  pinMode(MOTOR_BACK_RIGHT_1, OUTPUT);
  pinMode(MOTOR_BACK_RIGHT_2, OUTPUT);
  Serial.println("Done");

  Serial.print("Turning everything off... ");
  digitalWrite(MOTOR_FRONT_LEFT_1, LOW);
  digitalWrite(MOTOR_FRONT_LEFT_2, LOW);
  digitalWrite(MOTOR_FRONT_RIGHT_1, LOW);
  digitalWrite(MOTOR_FRONT_RIGHT_2, LOW);

  digitalWrite(MOTOR_BACK_LEFT_1, LOW);
  digitalWrite(MOTOR_BACK_LEFT_2, LOW);
  digitalWrite(MOTOR_BACK_RIGHT_1, LOW);
  digitalWrite(MOTOR_BACK_RIGHT_2, LOW);

  digitalWrite(PIN_Z_AXIS_1, LOW);
  digitalWrite(PIN_Z_AXIS_2, LOW);
  Serial.println("Done");

  Serial.println("Initialisation done");

  set_hangle(0);
  delay(1000);

  Serial.println("Ready");

  global_start = millis();

  servo_camera.write(0);

  #ifdef COMMENT
  Serial.println("; reset");
  #endif

  observe = false;
  slow_mode = false;


  Serial.println("; hello world");

  // digitalWrite(MOTOR_FRONT_LEFT_1, HIGH);
  // digitalWrite(MOTOR_FRONT_LEFT_2, LOW);
  // front_left_forwards();
  // front_right_forwards();
  // back_left_forwards();
  // back_right_forwards();

}
void loop() {
  if (observe)
  {
    hangle += d;
    if (hangle < ANGLE_LOWER || hangle > ANGLE_UPPER)
    {
      hangle = constrain(hangle, ANGLE_LOWER, ANGLE_UPPER);
      d *= -1;
    }
    set_hangle(90 + hangle);
  }

  measure(hangle);
  if (Serial.available())
  {
    handle_input(Serial.read());
  }
  delay(200);
}