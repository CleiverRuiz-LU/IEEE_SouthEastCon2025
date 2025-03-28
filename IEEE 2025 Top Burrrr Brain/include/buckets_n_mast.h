#include <Arduino.h>
// #include <PWMServo.h>
//#include <Adafruit_TinyUSB.h>
#include <Wire.h> //I2C Arduino Library
#include "CytronMotorDriver.h"
#include "burr_brain_pins.h"


// Creating servo object for the mast servo
// PWMServo mast_servo;

// Object declaration for the DC bucket pinch motors
CytronMD lbucket_pinch_motor(PWM_DIR, lbucket_pinch_PWM, lbucket_pinch_DIR);
CytronMD rbucket_pinch_motor(PWM_DIR, rbucket_pinch_PWM, rbucket_pinch_DIR);


// easy access for constants to tune
const int total_travel_bucket_lift_steps = 1400;    // STILL MAY NEED TO ADJUST
const int travel_height_steps = 300;                // TEST
const int bucket_pinch_speed = 170;                 // GOOD
const int bucket_pinch_delay = 1900;                // GOOD


// Setup for tthe bucket lift steppers.
void stepper_setup(){
    pinMode(lbucket_lift_DIR, OUTPUT);
    pinMode(rbucket_lift_DIR, OUTPUT);
    pinMode(lbucket_lift_PWM, OUTPUT);
    pinMode(rbucket_lift_PWM, OUTPUT);

    digitalWrite(lbucket_lift_DIR, LOW);
    digitalWrite(rbucket_lift_DIR, LOW);
    digitalWrite(lbucket_lift_PWM, LOW);
    digitalWrite(rbucket_lift_PWM, LOW);
}



//////////////////////////////////////////// Bucket Lift Steppers //////////////////////////////////////

// Quick current set: Put the driver in your circuit, connected to the motor you are using. Apply power 
// but don't try and make the motor move. Just measure the voltage on the metallic screw-top of the 
// potentiometer. Current limit in amps = 2X the measured voltage. Example, if you measure 0.5 volts at 
// the potentiometer the current limit will be set to 1.0 amps.

// Move steppers synchronously. Direction: HIGH = up, LOW = down. Steps: # of steps for motor to move (our motors do 200/revolution)
void move_steppers(int direction, int steps){
    // Set direction: HIGH = down, LOW = up (want to keep it simple for using this function)
    digitalWrite(lbucket_lift_DIR, !direction);
    digitalWrite(rbucket_lift_DIR, !direction);

    // 
    for (int i = 0; i < steps; i++) {
        digitalWrite(lbucket_lift_PWM, HIGH);
        digitalWrite(rbucket_lift_PWM, HIGH);
        delay(1);
        digitalWrite(lbucket_lift_PWM, LOW);
        digitalWrite(rbucket_lift_PWM, LOW);
        delay(1);
    }
    Serial.println("Finished moving.");
}

// Lower buckets from very top position to very bottom position
void lower_buckets_top_to_bottom(){
    move_steppers(LOW, total_travel_bucket_lift_steps);  
}

// Lift buckets from very bottom position to very top position
void lift_buckets_bottom_to_top(){
    move_steppers(HIGH, total_travel_bucket_lift_steps); 
}

// Lift buckets from very bottom to travel height
void lift_buckets_to_travel(){
    move_steppers(HIGH, travel_height_steps);
}

// Lift buckets from travel height to very top position
void lift_buckets_travel_to_top(){
    move_steppers(HIGH, total_travel_bucket_lift_steps - travel_height_steps);
}

// Lower buckets from very top position to travel height
void lower_buckets_top_to_travel(){
    move_steppers(LOW, total_travel_bucket_lift_steps - travel_height_steps);
}

//////////////////////////////////////////// Bucket Pinchers DC ////////////////////////////////////////////

// CAM moves to pinch left bucket.
void pinch_left_bucket(){
    lbucket_pinch_motor.setSpeed(-bucket_pinch_speed);  // negative is the correct direction here
    delay(bucket_pinch_delay);
    lbucket_pinch_motor.setSpeed(0);
}

// CAM moves to "unpinch" left bucket.
void release_left_bucket(){
    lbucket_pinch_motor.setSpeed(bucket_pinch_speed);   // positive is the correct direction here
    delay(bucket_pinch_delay);
    lbucket_pinch_motor.setSpeed(0);
}

// CAM moves to pinch right bucket.
void pinch_right_bucket(){
    rbucket_pinch_motor.setSpeed(-bucket_pinch_speed);   // negative is the correct direction here
    delay(bucket_pinch_delay);
    rbucket_pinch_motor.setSpeed(0);
}

// CAM moves to "unpinch" right bucket.
void release_right_bucket(){
    rbucket_pinch_motor.setSpeed(bucket_pinch_speed);   // positive is the correct direction here
    delay(bucket_pinch_delay);
    rbucket_pinch_motor.setSpeed(0);
}


/////////////////////////////////////////////// Mast Servo ////////////////////////////////////////////

// void mast_setup(){
//     mast_servo.attach(mast_servo_PWM);  // weird thing with servos where you have to "attach" them to their pins
// }

// void lower_mast(){
//     Serial.println("Lowering mast");
//     for (int pos = 80; pos > 10; pos--) { // goes from 80 degrees to 10 degrees
//         // in steps of 1 degree
//         mast_servo.write(pos);              // tell servo to go to position in variable 'pos'
//         delay(15);                       // waits 15ms for the servo to reach the position
//     }
// }


// void raise_mast(){
//     Serial.println("Lowering mast");
//     for (int pos = 10; pos < 80; pos++) { // goes from 10 degrees to 80 degrees
//         // in steps of 1 degree
//         mast_servo.write(pos);              // tell servo to go to position in variable 'pos'
//         delay(15);                       // waits 15ms for the servo to reach the position
//     }
// }







