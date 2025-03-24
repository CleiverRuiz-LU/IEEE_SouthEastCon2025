#include <Arduino.h>
#include <Wire.h> //I2C Arduino Library
#include "ferris_wheel_n_sorting.h"

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting...");

  ferris_wheel_n_sorting_setup();
  magnet_sensor_init();
  run_conveyor_belt();
}

void loop() {
  spin_ferris_wheel();
  while(1){
    int jam_flag = detect_ferris_wheel_jam();
    if(jam_flag != 0){
      ferris_wheel_jam_recovery();
      Serial.println("recovered");
    }
    detect_magnet();
    delay(500);
  }

  // spin_ferris_wheel();
  // delay(5000);
  // ferris_wheel_jam_recovery();
  // Serial.println("recovered");
  // delay(5000);

}
