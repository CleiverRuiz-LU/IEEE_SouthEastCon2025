#include <Arduino.h>
#include "CytronMotorDriver.h"
#include "bottom_plate_pins.h"

// Configure the motor drivers
CytronMD motor1(PWM_DIR, PWM1, DIR1);  // PWM1 = Pin 17, DIR1 = Pin 16.
CytronMD motor2(PWM_DIR, PWM2, DIR2);  // PWM2 = Pin 13, DIR2 = Pin 12.
CytronMD motor3(PWM_DIR, PWM3, DIR3);  // PWM3 = Pin 18, DIR3 = Pin 19.
CytronMD motor4(PWM_DIR, PWM4, DIR4);  // PWM4 = Pin 11, DIR4 = Pin 10.


// The setup routine runs once when you press reset.
void setup() {
  
}

// The loop routine runs over and over again forever.
void loop() {
  motor1.setSpeed(128);  // Run forward at 50% speed.
  delay(1000);
  
  motor1.setSpeed(255);  // Run forward at full speed.
  delay(1000);

  motor1.setSpeed(0);    // Stop.
  delay(1000);

  motor1.setSpeed(-128);  // Run backward at 50% speed.
  delay(1000);
  
  motor1.setSpeed(-255);  // Run backward at full speed.
  delay(1000);

  motor1.setSpeed(0);    // Stop.
  delay(1000);

}