# Rover Project - Sam Lovering
## Last updated: 12/3/2023
This repository is my source files for a ESP32 based rover for my EGEN310 class.

## About roverProject.ino
The roverProject.ino is bootstrapped with the following libraries:
1. PS4Controller.h \
This library provides a series of C function to connect and control a PS4 controller using its bluetooth MAC address.
2. Adafruit_PWMServoDriver.h \
This library handles the generation of a PWM (Pulse Width Modulation) signal to control the PCA9685 Servo Driver that steers and changes gears on the rover.
3. TB6612_ESP32.h \
This library contains functions that handle the operation of the TB6612 Motor Driver that controls the main motor on the rover. \ \ 

The .ino file polls for PS4 inputs, and outputs to the drivers based on the input. Besides the gear shift, these controls are analog and can be operated at lower values based on user input.

## How to control the rover with PS4 Controller
Left Stick to steer \
Left Trigger to drive in reverse \
Right Trigger to drive forwards \
Triangle Button to change gears
