// Flame sensor test - open the Serial Monitor (115200).
// No flame: values high (about 3000-4095). Flame near a sensor: its value drops.
#define FLAME_LEFT   34
#define FLAME_CENTER 35
#define FLAME_RIGHT  32

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  Serial.printf("GPIO34 (L): %d   GPIO35 (C): %d   GPIO32 (R): %d\n",
                analogRead(FLAME_LEFT), analogRead(FLAME_CENTER), analogRead(FLAME_RIGHT));
  delay(300);
}
