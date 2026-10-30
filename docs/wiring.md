# Wiring

All grounds (battery -, L298N GND, ESP32 GND, sensors, relay, servo, buzzer) must be joined.
The pump has its **own separate battery**.

## ESP32 pin map

| ESP32 pin | Connects to |
|---|---|
| GPIO 34 | Left flame sensor AO |
| GPIO 35 | Center flame sensor AO |
| GPIO 32 | Right flame sensor AO |
| GPIO 26 | L298N IN1 |
| GPIO 27 | L298N IN2 |
| GPIO 25 | L298N IN3 |
| GPIO 33 | L298N IN4 |
| GPIO 18 | L298N ENA (remove the jumper cap) |
| GPIO 19 | L298N ENB (remove the jumper cap) |
| GPIO 23 | Relay IN |
| GPIO 13 | Servo signal (orange) |
| GPIO 21 | Buzzer + |
| 3V3 | VCC of the 3 flame sensors |
| VIN | 5 V supply |
| GND | Common ground |

## Motors (4WD)

| Motors | L298N terminals |
|---|---|
| Both left motors (in parallel) | OUT1, OUT2 |
| Both right motors (in parallel) | OUT3, OUT4 |

The right side is wired opposite to the left, so the firmware uses `RIGHT_REVERSE true`.

## Relay and pump (separate pump battery)

| From | To |
|---|---|
| Relay VCC | 5 V |
| Relay GND | GND |
| Relay IN | GPIO 23 |
| Pump battery + | Relay COM |
| Relay NO | Pump + (red) |
| Pump - (black) | Pump battery - |

Leave NC empty. Keep the pump circuit separate from the robot ground.

## Servo

| Servo wire | Connects to |
|---|---|
| Orange (signal) | GPIO 13 |
| Red | 5 V |
| Brown | GND |

## Power notes
- Do not connect USB and the battery to the ESP32 at the same time.
- A 5 V buck converter (LM2596 / XL4015) is recommended for the servo, relay and ESP32 if the 5 V rail resets when the motors or pump start.
- If your motors are rated 6 V, lower `speedLeft` / `speedRight` or add a buck converter before the L298N 12 V input.
