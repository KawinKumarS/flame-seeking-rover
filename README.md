# ESP32 Autonomous Fire-Fighting Robot

A 4WD robot that detects a flame with three sensors, drives toward it, and puts it out with a servo-aimed water pump. Built with an ESP32 for the *Introduction to Robotics* course at SRM Institute of Science and Technology.

> Add a photo or GIF of your robot here: `media/robot.jpg`

## Features

* Flame direction detection with three analog flame sensors (left, center, right)
* Automatic sensor calibration at power-on (works even if the sensors differ in sensitivity)
* Differential-drive steering toward the strongest flame
* Stops at a set distance, runs the pump and sweeps the nozzle with a servo
* Buzzer alarm while a fire is detected
* Tunable settings and direction switches for easy setup

## Hardware

ESP32, 3 flame sensors, L298N motor driver, 4WD chassis, relay module, mini submersible pump (own battery), SG90/MG90S servo, buzzer. Full list and approximate cost (about Rs. 2,500) in [docs/components\_list.md](docs/components_list.md).

## How it works

1. At power-on the robot measures a baseline reading for each sensor (keep the flame away).
2. The sensor that drops the most from its baseline shows where the fire is.
3. The robot turns toward that side and drives forward in short steps.
4. When the flame is close enough (or after a maximum number of steps) it stops, switches the pump on through the relay, and sweeps the servo nozzle left and right.
5. When the flame is gone, the pump and buzzer turn off and the robot waits.

Flowchart: [docs/flowchart.md](docs/flowchart.md)

## Wiring

Pin map, motors, relay, pump and servo connections: [docs/wiring.md](docs/wiring.md)

|ESP32 pin|Connects to|
|-|-|
|GPIO 34 / 35 / 32|Flame sensors (left / center / right) AO|
|GPIO 26, 27, 25, 33|L298N IN1, IN2, IN3, IN4|
|GPIO 18 / 19|L298N ENA / ENB|
|GPIO 23|Relay IN|
|GPIO 13|Servo signal|
|GPIO 21|Buzzer +|

## Getting started

1. Install **Arduino IDE 2.x**.
2. In Boards Manager, install **esp32 by Espressif Systems** (version 3.x).
3. In Library Manager, install **ESP32Servo**.
4. Open `firmware/fire\_fighting\_robot/fire\_fighting\_robot.ino`.
5. Select **Tools > Board > ESP32 Dev Module** and your COM port.
6. Upload, then open the Serial Monitor at **115200** baud.
7. Switch on the robot **with no flame nearby**. One short beep means it is calibrated and ready.

## Test sketches

Test each part on its own first (wheels lifted for the motor test):

|Sketch|Checks|
|-|-|
|`tests/flame\_sensor\_test`|Sensor readings|
|`tests/motor\_test`|Forward, backward, left, right (buzzer beeps tell the step)|
|`tests/servo\_test`|Nozzle servo sweep|
|`tests/relay\_pump\_test`|Relay click and pump|

## Settings you can tune (top of the firmware)

|Setting|Meaning|
|-|-|
|`DETECT\_DROP`|Drop from baseline that counts as a fire (lower = more sensitive)|
|`CLOSE\_DROP`|Drop at which the robot stops and sprays (lower = sprays from farther)|
|`MIN\_STEPS` / `MAX\_STEPS`|Minimum and maximum forward steps before spraying|
|`speedLeft` / `speedRight`|Motor speed (0-255)|
|`LEFT\_REVERSE` / `RIGHT\_REVERSE`|Flip a side if its wheels spin backward|
|`SWAP\_SENSORS`|Flip if the robot turns away from the flame|
|`RELAY\_ON` / `RELAY\_OFF`|Swap if your relay is active-HIGH|

See [docs/troubleshooting.md](docs/troubleshooting.md) for common problems.

## Safety

* Test only with a small candle flame on a tray, on a non-flammable surface, with water nearby.
* Keep water away from the electronics, and never run the pump dry.
* Handle lithium cells carefully and never leave the robot unattended while powered.
* Water is not suitable for oil or electrical fires. This is a student prototype, not a certified safety device.

## Limitations and future work

* Flame sensors have a limited range and angle, and are affected by sunlight.
* No obstacle avoidance yet, and the tank holds only a small amount of water.
* Ideas: ultrasonic obstacle avoidance, smoke/gas sensor, ESP32-CAM flame detection, Wi-Fi/Telegram alerts, gesture or phone control.

## Team

* \[KAWIN KUMAR S]

,= SRM Institute of Science and Technology, Kattankulathur

## License

MIT License. See [LICENSE](LICENSE).

