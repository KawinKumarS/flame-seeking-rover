// Motor test - lift the wheels off the ground first.
// Buzzer beeps tell you the step: 1 = forward, 2 = backward, 3 = turn left, 4 = turn right.
#define IN1 26
#define IN2 27
#define IN3 25
#define IN4 33
#define ENA 18
#define ENB 19
#define BUZZER 21

#define TEST_SPEED 150   // raise if the wheels only hum

void leftSide(int dir) {
  analogWrite(ENA, dir == 0 ? 0 : TEST_SPEED);
  digitalWrite(IN1, dir == 1  ? HIGH : LOW);
  digitalWrite(IN2, dir == -1 ? HIGH : LOW);
}

// Right side wiring is opposite, so the pins are swapped here
void rightSide(int dir) {
  analogWrite(ENB, dir == 0 ? 0 : TEST_SPEED);
  digitalWrite(IN3, dir == -1 ? HIGH : LOW);
  digitalWrite(IN4, dir == 1  ? HIGH : LOW);
}

void stopAll() { leftSide(0); rightSide(0); }

void beep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER, HIGH); delay(250);
    digitalWrite(BUZZER, LOW);  delay(250);
  }
  delay(700);
}

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  delay(3000);
}

void loop() {
  Serial.println("1 beep: FORWARD");
  beep(1); leftSide(1);  rightSide(1);  delay(2000); stopAll(); delay(2000);

  Serial.println("2 beeps: BACKWARD");
  beep(2); leftSide(-1); rightSide(-1); delay(2000); stopAll(); delay(2000);

  Serial.println("3 beeps: TURN LEFT");
  beep(3); leftSide(-1); rightSide(1);  delay(2000); stopAll(); delay(2000);

  Serial.println("4 beeps: TURN RIGHT");
  beep(4); leftSide(1);  rightSide(-1); delay(2000); stopAll(); delay(4000);
}
