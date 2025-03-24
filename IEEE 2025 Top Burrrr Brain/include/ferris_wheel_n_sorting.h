// #include <Servo.h>
//#include <Adafruit_TinyUSB.h>  // Required for SAMD boards
#include <Wire.h> //I2C Arduino Library
#include <MechaQMC5883.h>   // library for magnet sensor (not the sensor we have, but only library that works for it)
#include "burr_brain_pins.h"

// Creating object for the magnet sensor
MechaQMC5883 mag_sensor;    // library has 0x1D as the automatic I2C address for magnet sensor

// Creating object for the sorting servo
// Servo sort_servo;

// easy access for constants to tune
const int current_sensor_jam_threshold = 20;   // Analog read output threshold parameter to determine if there is a jam in the ferris wheel. 
const int conveyor_speed = 180;         // speed can be 0-255
const int ferris_wheel_speed = 180;     // speed can be 0-255
const int jam_reverse_time = 1000;      // amount of time (in ms) we will spin the ferris wheel in reverse because of a jam


// Sets up ferris wheel motor, conveyor belt motor, the sorting servo, and current sensor.
void ferris_wheel_n_sorting_setup(){
    // Ferris wheel DC motor pins
    pinMode(ferriswheel_DIR, OUTPUT);
    pinMode(ferriswheel_PWM, OUTPUT);
    digitalWrite(ferriswheel_DIR, LOW); // choose LOW as default state
    digitalWrite(ferriswheel_PWM, LOW); // choose 0 as default speed

    // Conveyor belt DC motor pins 
    pinMode(convey_DIR, OUTPUT);
    pinMode(convey_PWM, OUTPUT);
    digitalWrite(convey_DIR, LOW);      // choose LOW s default state
    digitalWrite(convey_PWM, LOW);      // choose 0 as default speed

    // Sorting servo pin & weird "attach" thing you have to do for servos
    // pinMode(sorting_servo_PWM, OUTPUT);
    // sort_servo.attach(sorting_servo_PWM);

    // Current sensor pin
    pinMode(current_sensor_read_pin, INPUT);

    Serial.println("Ferris wheel and sorting setup complete");
}


/////////////////////////////////////////////// Current Sensor ////////////////////////////////////////

// Take a current sensor reading and compare to threshold. 
// Returns a jam_flag: 1 if the threshold has been breached, 0 if everything is chill.
int detect_ferris_wheel_jam(){
    int jam_flag = 0;
    int sensor_value = analogRead(current_sensor_read_pin);
    Serial.print("Analog Reading from current sensor: ");
    Serial.println(sensor_value);

    if (sensor_value >= current_sensor_jam_threshold){
        jam_flag = 1;
        Serial.println("Ferris wheel jam detected");
    }
    return jam_flag;
}


/////////////////////////////////////////// Magnet Sensor ///////////////////////////////////////////////////////

// Initializes the magnet sensor.
void magnet_sensor_init(){
    mag_sensor.init();  // initializing magnet sensor via library function
    Serial.println("Magnet sensor initialized");
}


// Take a magnet sensor reading and compare to threshold to decide if we have a magnet or not.
// Returns magnet_flag: 0 if no magnet, and 1 if yes magnet.
int detect_magnet(){
    int x, y, z;
    int azimuth;
    int magnet_flag = 0;

    mag_sensor.read(&x, &y, &z, &azimuth);                  // take a sensor reading

    // Check if any of the sensors are above 2000
    if(x > abs(2000) || y > abs(2000) || z > abs(2000)){    // 2000 was the threshold that seems to work best for us in this case
        magnet_flag = 1;                                    // found a magnet, set this flag high
        Serial.println("I saw a magnet!!!!!!!!!!!!!");
    } else{
        Serial.println("No magnet detected");
    }

    delay(50);          // delay for stability

    return magnet_flag; // let the outside know if we found a magnet or not
}



/////////////////////////////////////////// Sorting Servo ///////////////////////////////////////////////////////

// // Moves sorting servo to direct magnet rocks to their bucket
// void sort_magnet_rocks(){
//     for(int pos = 5; pos < 55; pos++){  // goes from 5 degrees to 55 degrees 
//         // in steps of 1 degree
//         sort_servo.write(pos);          // tell servo to go to position in variable 'pos'
//         delay(15);                      // waits 15ms for the servo to reach the position
//     }
// }

// // Moves sorting servo to direct lame rocks to their bucket
// void sort_lame_rocks(){
//     for(int pos = 55; pos > 5; pos--){  // goes from 55 degrees to 5 degrees 
//         // in steps of 1 degree
//         sort_servo.write(pos);          // tell servo to go to position in variable 'pos'
//         delay(15);                      // waits 15ms for the servo to reach the position
//     }
// }


/////////////////////////////////////////// Conveyor Belt ///////////////////////////////////////////////////////

// Run conveyor belt forward. This is a non-blocking function (this will run no matter what until you call stop).
void run_conveyor_belt(){
    Serial.println("Start conveyor belt");
    digitalWrite(convey_DIR, HIGH);                // NEED TO TEST DIRECTION
    analogWrite(convey_PWM, conveyor_speed);       // NEED TO TEST SPEED
}

// Stop conveyor belt.
void stop_conveyor_belt(){
    Serial.println("Stop conveyor belt");
    digitalWrite(convey_DIR, LOW);      // set motor direction (best to drive pin LOW if stopping)
    analogWrite(convey_PWM, 0);         // set speed to 0
}


///////////////////////////////////////// Ferris Wheel ////////////////////////////////////////////////////////////

// Starts the ferris wheel spinning. This is a non-blocking function.
void spin_ferris_wheel(){
    Serial.println("Start ferris wheel");
    digitalWrite(ferriswheel_DIR, LOW);                   // set motor direction
    analogWrite(ferriswheel_PWM, ferris_wheel_speed);      // set motor speed
}

// Stops the ferris wheel.
void stop_ferris_wheel(){
    digitalWrite(ferriswheel_DIR, LOW);     // set motor direction (best to drive pin LOW if stopping)
    analogWrite(ferriswheel_PWM, 0);        // set speed to 0
    Serial.println("Stop ferris wheel");
}

// Used for if the jam_flag goes high.
// Runs ferris wheel in reverse for jam_reverse_time and then resumes normal ferris wheel operation.
void ferris_wheel_jam_recovery(){
    stop_ferris_wheel();
    delay(500);                                  // stops motors in between each direction change for motor safety
    digitalWrite(ferriswheel_DIR, HIGH);                   // set motor direction in reverse
    analogWrite(ferriswheel_PWM, ferris_wheel_speed);     // set motor speed
    delay(jam_reverse_time);                              // amount of time (in ms) for ferris wheel to go in reverse
    stop_ferris_wheel();    
    delay(500);                              // stops motors in between each direction change for motor safety
    spin_ferris_wheel();                                  // go back to normal
}