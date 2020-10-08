# ThingsUpChallenge
Participated in the ThingsUP IoT Challenge and Integrated Ultrasonic Distance Sensor with ESP-32 and published its data onto ThingsPeak MQTT Server.

# Installation
1. Install Arduino IDE with ESP32 Board
2. Plug in NodeMCU board with ESP32
3. Change the WiFi and MQTT account related parameters in code
4. Compile/Download the code in NodeMCU board

# Connections
1. Connect the VCC of the ULtrasonic sensor to pin to any 5V supply
2. Connect the TRIGGER of the ULtrasonic sensor to pin to GPIO 2 of ESP32
3. Connect the ECHO pin of the ULtrasonic sensor to GPIO 4 of ESP32
4. Connect the ground pin of the ULtrasonic sensor to GND of ESP 32

# MQTT Connections
1. Give the appropriate MQTT username and password in the Code.
2. The channel ID should be same as where you want to the Result.
