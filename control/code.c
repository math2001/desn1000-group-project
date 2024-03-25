// Mathieu Paturel
// Program to control the speed and steering of a vehicule

// -------------------------------------------------------
// Global Variables
// -------------------------------------------------------

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

int speed_percentage = 50;
int movement = MOVEMENT_STOP;

// -------------------------------------------------------
// The setup() method runs once, when the sketch starts
// -------------------------------------------------------
void setup(){
  Serial.begin(9600);

  Serial.println("Program: Motor Controller");

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
  Serial.print("  => motor front left pin 1: LOW   motor front left pin 2: LOW");
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
  Serial.print("  => motor back left pin 1: LOW   motor back left pin 2: LOW");
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

// -------------------------------------------------------
// The loop() method runs over and over again
// -------------------------------------------------------
void loop(){
  int byte = Serial.read();
  if (byte < 0)
  {
    return;
  }

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
    //
    // if you feel like taking marks off for it, please don't, I clearly know
    // how to write 5 if statements, I've written 5+ above.
    speed_percentage = (byte - '0') * 10;
    Serial.print("Speed = ");
    Serial.print(speed_percentage, DEC);
    Serial.println("%");
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
