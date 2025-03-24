#include <Arduino.h>
#include <Wire.h> //I2C Arduino Library
#include "ferris_wheel_n_sorting.h"
#include "buckets_n_mast.h"
#include "routines.h"

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting...");

  stepper_setup();
}

void loop() {
  move_steppers(HIGH, 1400);
  delay(3000);
  move_steppers(LOW, 1400);
  while(1){

  }
}
