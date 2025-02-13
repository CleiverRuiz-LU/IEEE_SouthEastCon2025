#include <Arduino.h>
#include "CytronMotorDriver.h"
#include "bottom_plate_pins.h"
#include "Wire.h"
#include "bottom_setup.h"


// ---------------------------------------------------------------------------------------------------
// Movement functions specifically for a 4 mecanum wheel drivetrain
// 
// SPEEDS are integers set from -255 to 255, where one is forward and the other is reverse
// TIME is set in milliseconds, so 1 second would be entered as 1000
// ---------------------------------------------------------------------------------------------------

// Configure the motor driver objects
// CytronMD motor1(PWM_DIR, PWM1, DIR1);  // PWM1 = Pin 17, DIR1 = Pin 16.
// CytronMD motor2(PWM_DIR, PWM2, DIR2);  // PWM2 = Pin 13, DIR2 = Pin 12.
// CytronMD motor3(PWM_DIR, PWM3, DIR3);  // PWM3 = Pin 18, DIR3 = Pin 19.
// CytronMD motor4(PWM_DIR, PWM4, DIR4);  // PWM4 = Pin 11, DIR4 = Pin 10.


void stop_motors(){
    motor1.setSpeed(0);  // Stop.
    motor2.setSpeed(0);  // Stop.
    motor3.setSpeed(0);  // Stop.
    motor4.setSpeed(0);  // Stop.
}



////////////////////// Time-based Movement functions ////////////////////////////////////////////////////!SECTION


void go_forward_time(int speed, int time){
    // expecting 0 < speed <= 255
    motor1.setSpeed(speed);  // Run forward at speed.
    motor2.setSpeed(speed);  // Run forward at speed.
    motor3.setSpeed(speed);  // Run forward at speed.
    motor4.setSpeed(speed);  // Run forward at speed.

    delay(time);

    stop_motors();
}


void go_backward_time(int speed, int time){
    // expecting 0 < speed <= 255
    int reverse_speed = speed * -1;      // make speed negative for setSpeed method
    motor1.setSpeed(reverse_speed);  // Run forward at speed.
    motor2.setSpeed(reverse_speed);  // Run forward at speed.
    motor3.setSpeed(reverse_speed);  // Run forward at speed.
    motor4.setSpeed(reverse_speed);  // Run forward at speed.

    delay(time);

    stop_motors();
}


void go_left_time(int speed, int time){
    // expecting 0 < speed <= 255
    int reverse_speed = speed * -1;
    motor1.setSpeed(reverse_speed);  // Run backward at speed.
    motor2.setSpeed(speed);          // Run forward at speed.
    motor3.setSpeed(speed);          // Run forward at speed.
    motor4.setSpeed(reverse_speed);  // Run backward at speed.

    delay(time);

    stop_motors();
}


void go_right_time(int speed, int time){
    // expecting 0 < speed <= 255
    int reverse_speed = speed * -1;
    motor1.setSpeed(speed);          // Run backward at speed.
    motor2.setSpeed(reverse_speed);  // Run forward at speed.
    motor3.setSpeed(reverse_speed);  // Run forward at speed.
    motor4.setSpeed(speed);          // Run backward at speed.

    delay(time);

    stop_motors();
}


void rotate_right_time(int time){
    int speed = 128;
    int reverse_speed = speed * -1;
    motor1.setSpeed(speed);          // Run forward at 50% speed.
    motor2.setSpeed(speed);          // Run forward at 50% speed.
    motor3.setSpeed(reverse_speed);  // Run backward at 50% speed.
    motor4.setSpeed(reverse_speed);  // Run backward at 50% speed.

    delay(time);

    stop_motors();
}


void rotate_left_time(int time){
    int speed = 128;
    int reverse_speed = speed * -1;
    motor1.setSpeed(reverse_speed);  // Run backward at 50% speed.
    motor2.setSpeed(reverse_speed);  // Run backward at 50% speed.
    motor3.setSpeed(speed);          // Run forward at 50% speed.
    motor4.setSpeed(speed);          // Run forward at 50% speed.

    delay(time);

    stop_motors();
}


//////////////////////////// Movement without stopping //////////////////////////////////////////////////////////

void go_forward(int speed){
    // expecting 0 < speed <= 255
    motor1.setSpeed(speed);  // Run forward at speed.
    motor2.setSpeed(speed);  // Run forward at speed.
    motor3.setSpeed(speed);  // Run forward at speed.
    motor4.setSpeed(speed);  // Run forward at speed.

}


void go_backward(int speed){
    // expecting 0 < speed <= 255
    int reverse_speed = speed * -1;      // make speed negative for setSpeed method
    motor1.setSpeed(reverse_speed);  // Run forward at speed.
    motor2.setSpeed(reverse_speed);  // Run forward at speed.
    motor3.setSpeed(reverse_speed);  // Run forward at speed.
    motor4.setSpeed(reverse_speed);  // Run forward at speed.

}

void go_left(int speed){
    // expecting 0 < speed <= 255
    int reverse_speed = speed * -1;
    motor1.setSpeed(reverse_speed);  // Run backward at speed.
    motor2.setSpeed(speed);          // Run forward at speed.
    motor3.setSpeed(speed);          // Run forward at speed.
    motor4.setSpeed(reverse_speed);  // Run backward at speed.

}

void go_right(int speed){
    // expecting 0 < speed <= 255
    int reverse_speed = speed * -1;
    motor1.setSpeed(speed);          // Run backward at speed.
    motor2.setSpeed(reverse_speed);  // Run forward at speed.
    motor3.setSpeed(reverse_speed);  // Run forward at speed.
    motor4.setSpeed(speed);          // Run backward at speed.

}

void rotate_right(){
    int speed = 128;
    int reverse_speed = speed * -1;
    motor1.setSpeed(speed);          // Run forward at 50% speed.
    motor2.setSpeed(speed);          // Run forward at 50% speed.
    motor3.setSpeed(reverse_speed);  // Run backward at 50% speed.
    motor4.setSpeed(reverse_speed);  // Run backward at 50% speed.

}

void rotate_left(){
    int speed = 128;
    int reverse_speed = speed * -1;
    motor1.setSpeed(reverse_speed);  // Run backward at 50% speed.
    motor2.setSpeed(reverse_speed);  // Run backward at 50% speed.
    motor3.setSpeed(speed);          // Run forward at 50% speed.
    motor4.setSpeed(speed);          // Run forward at 50% speed.

}


//////////////////////////// Rotating with Gyro //////////////////////////////////////////////////////////

// Takes a gyro yaw (rotation about z-axis) reading, stores it and starts rotating right 
// until the difference between the original stored reading and the new readings reaches
// 90 degrees, then the motors stop.
void rotate_right_gyro_90(){
    float yaw_ref = 0;
    float new_yaw;
    float angle = 90;

    // gets data from the gyro and calculates angles
    Serial.println(F("getting gyro data..."));
    mpu.dmpGetCurrentFIFOPacket(FIFOBuffer);    // grab the current data
    mpu.dmpGetQuaternion(&q, FIFOBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);  // yaw = rotation about z, pitch = rotation about y, roll = rotation about x
    yaw_ref = ypr[0] * 180/M_PI;                // convert reference yaw value to degrees
    new_yaw = yaw_ref;                          // initialize new_yaw as yaw_ref
    Serial.println(yaw_ref);
    delay(200);                                 // just in case


    // robot turns right until stop_motors() is called
    Serial.println(F("saying rotate right"));
    rotate_right();

    while(abs(new_yaw - yaw_ref) < angle){      // keep updating the gyro until the robot has turned 90 degrees right
        Serial.println(F("getting gyro data..."));
        mpu.dmpGetCurrentFIFOPacket(FIFOBuffer);
        mpu.dmpGetQuaternion(&q, FIFOBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        new_yaw = ypr[0] * 180/M_PI;            // store current reading in new_yaw
        // Serial.println(new_yaw);
    }
    Serial.println(F("calling stop_motors()"));
    stop_motors();

}


// Takes a gyro yaw (rotation about z-axis) reading, stores it and starts rotating left 
// until the difference between the original stored reading and the new readings reaches
// 90 degrees, then the motors stop.
void rotate_left_gyro_90(){
    float yaw_ref = 0;
    float new_yaw;
    float angle = 90;

    // gets data from the gyro and calculates angles
    Serial.println(F("getting gyro data..."));
    mpu.dmpGetCurrentFIFOPacket(FIFOBuffer);    // grab the current data
    mpu.dmpGetQuaternion(&q, FIFOBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);  // yaw = rotation about z, pitch = rotation about y, roll = rotation about x
    yaw_ref = ypr[0] * 180/M_PI;                // convert reference yaw value to degrees
    new_yaw = yaw_ref;                          // initialize new_yaw as yaw_ref
    Serial.println(yaw_ref);
    delay(200);                                 // just in case


    // robot turns left until stop_motors() is called
    Serial.println(F("saying rotate left"));
    rotate_left();

    while(abs(new_yaw - yaw_ref) < angle){      // keep updating the gyro until the robot has turned 90 degrees left
        Serial.println(F("getting gyro data..."));
        mpu.dmpGetCurrentFIFOPacket(FIFOBuffer);
        mpu.dmpGetQuaternion(&q, FIFOBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        new_yaw = ypr[0] * 180/M_PI;            // store current reading in new_yaw
        // Serial.println(new_yaw);
    }
    Serial.println(F("calling stop_motors()"));
    stop_motors();

}