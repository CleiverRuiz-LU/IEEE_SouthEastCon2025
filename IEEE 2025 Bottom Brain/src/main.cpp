#include <Arduino.h>
#include "CytronMotorDriver.h"
#include "bottom_plate_pins.h"
#include "movement.h"
//#include "bottom_setup.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

char incomingChar;
int half_speed = 128;
int full_speed = 255;

void setup() {
  // Initialize serial monitor
  Serial.begin(9600);
  while (!Serial);

  gyro_setup();
}

void loop() {

  if(Serial.available() > 0){
    incomingChar = Serial.read();
    if(incomingChar == 'l'){
      rotate_left_gyro_90();

      // then need to send 'done' flag back to serial?
    } else if (incomingChar == 'r'){
      rotate_right_gyro_90();

      // then need to send 'done' flag back to serial?
    } else if (incomingChar == 'L'){
      go_left(half_speed);    // go left forever

    } else if (incomingChar == 'R'){
      go_right(half_speed);   // go right forever

    } else if (incomingChar == 'F'){
      go_forward(half_speed);  // go forward forever

    } else if (incomingChar == 'B'){
      go_backward(half_speed); // go backward forever

    } else if (incomingChar == 'S'){
      stop_motors();           // stop all motors

    }
  }
}