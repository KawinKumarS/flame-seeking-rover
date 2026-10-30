// Relay + pump test - the relay should click and the pump should run 3 s on, 3 s off.
// Put the pump in water first. Change the levels if your relay is active-HIGH.
#define RELAY_PIN 23

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);   // off
}

void loop() {
  digitalWrite(RELAY_PIN, LOW);  delay(3000);   // pump on
  digitalWrite(RELAY_PIN, HIGH); delay(3000);   // pump off
}
