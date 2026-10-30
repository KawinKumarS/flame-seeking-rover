// Servo test - the nozzle servo should swing between 60 and 120 degrees.
#include <ESP32Servo.h>

Servo s;

void setup() {
  s.setPeriodHertz(50);
  s.attach(13, 500, 2400);
}

void loop() {
  s.write(60);  delay(1000);
  s.write(120); delay(1000);
}
