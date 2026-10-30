/*
  Autonomous Fire-Fighting Robot - ESP32
  Library needed: ESP32Servo (ESP32 board package 3.x)

  How it works:
  1. At power-on the robot measures a baseline for each flame sensor (keep flame away).
  2. In the loop it computes the "drop" from baseline. Bigger drop = stronger flame.
  3. It steers toward the side with the bigger drop and drives forward in short steps.
  4. When the flame is close enough (or after MAX_STEPS) it stops, runs the pump
     and sweeps the servo nozzle left and right.
*/

#include <ESP32Servo.h>

// ================ MOTOR PINS (L298N) ================
#define IN1 26
#define IN2 27
#define IN3 25
#define IN4 33
#define ENA 18
#define ENB 19

// ============ DIRECTION SWITCHES ============
#define LEFT_REVERSE  false   // true if left wheels spin the wrong way
#define RIGHT_REVERSE true    // true if right wheels spin the wrong way
#define SWAP_SENSORS  true    // true if robot turns AWAY from the flame

// ================ FLAME SENSORS (analog out) =======
#define FLAME_LEFT   34
#define FLAME_CENTER 35
#define FLAME_RIGHT  32

// ================ SERVO, RELAY, BUZZER =============
#define SERVO_PIN 13
#define RELAY_PIN 23
#define BUZZER    21

// ================ FIRE SETTINGS ====================
// "Drop" = baseline reading (no flame) minus current reading.
#define DETECT_DROP  400   // drop bigger than this = fire seen
#define CLOSE_DROP  1500   // drop bigger than this = close enough to spray
#define TOLERANCE    300   // left/right difference smaller than this = flame ahead
#define MIN_STEPS      1   // always drive at least this many steps toward the flame
#define MAX_STEPS      4   // spray after this many steps no matter what
#define LOST_LIMIT    10   // readings without flame before the step counter resets

#define RELAY_ON  LOW      // change to HIGH if your relay is active-HIGH
#define RELAY_OFF HIGH

// ================ MOVEMENT SETTINGS ================
int speedLeft  = 230;
int speedRight = 230;
int turnTime   = 120;      // ms, short correction turn
int moveTime   = 150;      // ms, forward step

// ================ SERVO SETTINGS ===================
int servoLeft   = 60;
int servoCenter = 90;
int servoRight  = 120;

Servo fireServo;
int stepsTaken = 0;
int lostCount  = 0;
int baseL, baseC, baseR;

// ---------- read a sensor (average of 5 samples) ----------
int readFlame(int pin) {
  long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(pin);
    delay(2);
  }
  return sum / 5;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RELAY_PIN, RELAY_OFF);
  digitalWrite(BUZZER, LOW);

  fireServo.setPeriodHertz(50);
  fireServo.attach(SERVO_PIN, 500, 2400);
  fireServo.write(servoCenter);

  stopCar();

  // Calibrate with NO flame nearby
  Serial.println("Calibrating - keep flame away...");
  delay(2000);
  long sL = 0, sC = 0, sR = 0;
  for (int i = 0; i < 20; i++) {
    sL += readFlame(FLAME_LEFT);
    sC += readFlame(FLAME_CENTER);
    sR += readFlame(FLAME_RIGHT);
    delay(20);
  }
  baseL = sL / 20;
  baseC = sC / 20;
  baseR = sR / 20;

  Serial.printf("Baseline  L:%d  C:%d  R:%d\n", baseL, baseC, baseR);
  Serial.println("Fire Fighter Robot Ready");

  digitalWrite(BUZZER, HIGH); delay(200);   // one short beep = ready
  digitalWrite(BUZZER, LOW);
}

void loop() {
  int dropL = baseL - readFlame(FLAME_LEFT);
  int dropC = baseC - readFlame(FLAME_CENTER);
  int dropR = baseR - readFlame(FLAME_RIGHT);

  int leftDrop   = SWAP_SENSORS ? dropR : dropL;
  int rightDrop  = SWAP_SENSORS ? dropL : dropR;
  int centerDrop = dropC;

  Serial.printf("DropL:%d  DropC:%d  DropR:%d  Steps:%d\n",
                leftDrop, centerDrop, rightDrop, stepsTaken);

  int strongest = max(leftDrop, max(centerDrop, rightDrop));

  // No fire
  if (strongest < DETECT_DROP) {
    stopCar();
    pumpOff();
    digitalWrite(BUZZER, LOW);
    lostCount++;
    if (lostCount >= LOST_LIMIT) stepsTaken = 0;
    delay(100);
    return;
  }

  lostCount = 0;
  digitalWrite(BUZZER, HIGH);   // fire alarm

  bool closeByReading = (stepsTaken >= MIN_STEPS) && (strongest >= CLOSE_DROP);
  bool closeBySteps   = (stepsTaken >= MAX_STEPS);

  if (closeByReading || closeBySteps) {
    stopCar();
    Serial.println("FIRE CLOSE - SPRAYING");
    sprayWater();
    stepsTaken = MIN_STEPS;
    delay(300);
    return;
  }

  // Steer toward the side with the bigger drop
  int diff = leftDrop - rightDrop;
  if (diff > TOLERANCE) {
    Serial.println("Fire LEFT - turn left, then forward");
    turnLeft();  delay(turnTime); stopCar();
  } else if (diff < -TOLERANCE) {
    Serial.println("Fire RIGHT - turn right, then forward");
    turnRight(); delay(turnTime); stopCar();
  } else {
    Serial.println("Fire AHEAD - forward");
  }

  forward(); delay(moveTime); stopCar();
  stepsTaken++;
  delay(200);
}

// ================= MOTOR FUNCTIONS =================
// dir: 1 = forward, -1 = backward, 0 = stop
void driveLeft(int dir) {
  if (LEFT_REVERSE) dir = -dir;
  analogWrite(ENA, dir == 0 ? 0 : speedLeft);
  digitalWrite(IN1, dir == 1  ? HIGH : LOW);
  digitalWrite(IN2, dir == -1 ? HIGH : LOW);
}

void driveRight(int dir) {
  if (RIGHT_REVERSE) dir = -dir;
  analogWrite(ENB, dir == 0 ? 0 : speedRight);
  digitalWrite(IN3, dir == 1  ? HIGH : LOW);
  digitalWrite(IN4, dir == -1 ? HIGH : LOW);
}

void forward()   { driveLeft(1);  driveRight(1);  }
void backward()  { driveLeft(-1); driveRight(-1); }
void turnLeft()  { driveLeft(-1); driveRight(1);  }
void turnRight() { driveLeft(1);  driveRight(-1); }
void stopCar()   { driveLeft(0);  driveRight(0);  }

// ================= PUMP / RELAY ===================
void pumpOn() {
  digitalWrite(RELAY_PIN, RELAY_ON);
  Serial.println("PUMP ON");
}

void pumpOff() {
  digitalWrite(RELAY_PIN, RELAY_OFF);
  fireServo.write(servoCenter);
}

// ================= WATER SPRAY ====================
void sprayWater() {
  Serial.println("START SPRAYING");
  stopCar();
  pumpOn();

  for (int i = 0; i < 6; i++) {
    fireServo.write(servoLeft);  delay(300);
    fireServo.write(servoRight); delay(300);
  }

  fireServo.write(servoCenter);
  delay(100);
  pumpOff();
  Serial.println("SPRAY COMPLETE");
  delay(500);
}
