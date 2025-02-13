#include <Arduino.h>
#include "CytronMotorDriver.h"
#include "bottom_plate_pins.h"
#include "movement.h"
#include "Wire.h"

char incomingByte;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  //go_forward_time(255,5000);    // go full speed forward for 5 seconds
  //delay(2000);

  //go_backward_time(128,4000);    // go full speed backward for 5 seconds
  //delay(2000);

  //go_left_time(128, 5000);    // go full speed left for 5 seconds
  //delay(2000);

  //go_right_time(255,5000);    // go full speed right for 5 seconds
  //delay(2000);

  //rotate_left_time(3000);    // spin left for 3 seconds
  //delay(2000);

  //rotate_right_time(3000);   // spin right for 3 seconds
  //delay(2000);

  if(Serial.available() > 0){
    // read the incoming byte
    incomingByte = Serial.read();

    Serial.print("I recieved: ");
    Serial.println(incomingByte);

    if (incomingByte == 'f'){
      go_forward(128);
    } else if (incomingByte == 's'){
      stop_motors();
    }
  }
}

