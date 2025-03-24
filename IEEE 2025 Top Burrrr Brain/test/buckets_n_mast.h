#include <Arduino.h>
#include <Servo.h>
#include <Wire.h> //I2C Arduino Library
#include "burr_brain_pins.h"


// Creating servo object for the mast servo
Servo mast_servo;


//////////////////////////////////////////// Bucket Lift Steppers //////////////////////////////////////

// Quick current set: Put the driver in your circuit, connected to the motor you are using. Apply power 
// but don't try and make the motor move. Just measure the voltage on the metallic screw-top of the 
// potentiometer. Current limit in amps = 2X the measured voltage. Example, if you measure 0.5 volts at 
// the potentiometer the current limit will be set to 1.0 amps.

// Raise both steppers all the way to the top synchronously.
void raise_steppers_to_top(){
    // Set direction
    digitalWrite(lbucket_lift_DIR, HIGH);
    digitalWrite(rbucket_lift_DIR, HIGH);

    // 
}


/////////////////////////////////////////////// Mast Servo ////////////////////////////////////////////

void mast_setup(){
    pinMode(mast_servo_PWM, OUTPUT);
    mast_servo.attach(mast_servo_PWM);  // weird thing with servos where you have to "attach" them to their pins
}

void lower_mast(){
    Serial.println("Lowering mast");
    for (int pos = 80; pos > 10; pos--) { // goes from 80 degrees to 10 degrees
        // in steps of 1 degree
        mast_servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }
}


void raise_mast(){
    Serial.println("Lowering mast");
    for (int pos = 10; pos < 80; pos++) { // goes from 10 degrees to 80 degrees
        // in steps of 1 degree
        mast_servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }
}





