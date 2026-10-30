# Troubleshooting

| Problem | Likely cause and fix |
|---|---|
| Upload fails ("Connecting...") | Use a data USB cable, hold the BOOT button while uploading, unplug wires on GPIO 0/1/3, install CP210x or CH340 driver |
| Wheels spin backward | Set `LEFT_REVERSE` / `RIGHT_REVERSE` in the firmware, or swap that side's motor wires |
| Robot turns away from the flame | Toggle `SWAP_SENSORS` |
| Motors hum but the robot does not move | Charge the battery, raise `speedLeft` / `speedRight`, increase `moveTime` |
| Robot gets too close to the flame | Lower `CLOSE_DROP` and `MAX_STEPS`, shorten `moveTime` |
| Robot sprays too far away | Raise `CLOSE_DROP` or `MAX_STEPS` |
| A side never reacts | Run `flame_sensor_test`, check that sensor's wires and the small blue potentiometer |
| Relay green LED off | Relay VCC must go to 5 V (not 3V3) and GND must be connected |
| Pump does not run | Check COM/NO wiring, pump submerged, pump battery charged, relay logic (`RELAY_ON`) |
| Servo does not move | Run `servo_test`, check the signal on GPIO 13 and the shared ground |
| ESP32 resets when the pump or servo starts | Use a separate pump battery and a 5 V buck converter with a 470-1000 uF capacitor |
