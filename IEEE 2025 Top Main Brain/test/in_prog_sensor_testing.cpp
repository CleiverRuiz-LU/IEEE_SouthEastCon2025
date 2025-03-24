#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cx_class.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include "dist_sensors.h"

#define DEV_I2C Wire
//#define SerialPort Serial

VL53L4CX_MultiRangingData_t *front_rangeData;


/* Setup ---------------------------------------------------------------------*/

void setup()
{
  // Initialize serial for output.
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting...");

  // Initialize I2C bus.
  DEV_I2C.begin();

  distsensor_setup();
  Serial.println("should be done with setup in main");
}

void loop()
{
  for(int i = 0; i < 50; i++){
    front_distsensor_GetMeasurement();
    for(int j = 0; j < pRangingData->NumberOfObjectsFound; j++){
      if (j != 0) {
        Serial.print("\r\n                               ");
      }
      Serial.print("status=");
      Serial.print(pRangingData->RangeData[j].RangeStatus);
      Serial.print(", D=");
      Serial.print(pRangingData->RangeData[j].RangeMilliMeter);
      Serial.print("mm");
    }
    
  }

  while(1){

  }
}

#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cx_class.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include "dist_sensors.h"

#define DEV_I2C Wire
//#define SerialPort Serial

//VL53L4CX_MultiRangingData_t *front_rangeData;


/* Setup ---------------------------------------------------------------------*/

void setup()
{
  // Initialize serial for output.
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting...");

  // Initialize I2C bus.
  DEV_I2C.begin();

  front_distsensor.begin();
    //frontleft_distsensor.begin();
    //backleft_distsensor.begin();
    //backright_distsensor.begin();

    // Switch off distance sensors (via XSHUT pins).
    front_distsensor.VL53L4CX_Off();
    //frontleft_distsensor.VL53L4CX_Off();
    //backleft_distsensor.VL53L4CX_Off();
    //backright_distsensor.VL53L4CX_Off();

    // Initialize distance sensors.
    // Arguement being passed is the I2C address we are giving each one. Hello world 
    // example had 0x12 as the address and the datasheet says there is a difference 
    // between odd/even address value because of the LSB, so we are doing all even addresses.
    front_distsensor.InitSensor(0x12);
    //frontleft_distsensor.InitSensor(0x14);
    //backleft_distsensor.InitSensor(0x16);
    //backright_distsensor.InitSensor(0x18);

    // Start Measurements
    front_distsensor.VL53L4CX_StartMeasurement();
    //frontleft_distsensor.VL53L4CX_StartMeasurement();
    //backleft_distsensor.VL53L4CX_StartMeasurement();
    //backright_distsensor.VL53L4CX_StartMeasurement();

    Serial.println("Should be done with setup.");
  Serial.println("should be done with setup in main");
}

// void loop()
// {
//   for(int i = 0; i < 50; i++){
//     front_distsensor_GetMeasurement();
//     // for(int j = 0; j < pRangingData->NumberOfObjectsFound; j++){
//     //   if (j != 0) {
//     //     Serial.print("\r\n                               ");
//     //   }
//     //   Serial.print("status=");
//     //   Serial.print(pRangingData->RangeData[j].RangeStatus);
//     //   Serial.print(", D=");
//     //   Serial.print(pRangingData->RangeData[j].RangeMilliMeter);
//     //   Serial.print("mm");
//     // }
//     delay(35);
//   }

//   while(1){

//   }
// }

void loop()
{
  VL53L4CX_MultiRangingData_t MultiRangingData;
  VL53L4CX_MultiRangingData_t *pMultiRangingData = &MultiRangingData;
  uint8_t NewDataReady;
  int no_of_object_found = 0, j;
  char report[64];
  int status;
  
  // ---------------------------------- Sensor 1 -----------------------------------------------------
  //sensor_vl53l4cx_sat1.VL53L4CX_StartMeasurement();
  if (status == 0) {
    status = front_distsensor.VL53L4CX_ClearInterruptAndStartMeasurement();
  }
  do {
    status = front_distsensor.VL53L4CX_GetMeasurementDataReady(&NewDataReady);
  } while (!NewDataReady);


  if ((!status) && (NewDataReady != 0)) {
    status = front_distsensor.VL53L4CX_GetMultiRangingData(pMultiRangingData);
    no_of_object_found = pMultiRangingData->NumberOfObjectsFound;
    snprintf(report, sizeof(report), "VL53L4CX Satellite1: Count=%d, #Objs=%1d ", pMultiRangingData->StreamCount, no_of_object_found);
    Serial.print(report);
    for (j = 0; j < no_of_object_found; j++) {
      if (j != 0) {
        Serial.print("\r\n                               ");
      }
      Serial.print("status=");
      Serial.print(pMultiRangingData->RangeData[j].RangeStatus);
      Serial.print(", D=");
      Serial.print(pMultiRangingData->RangeData[j].RangeMilliMeter);
      Serial.print("mm");
      Serial.print(", Signal=");
      Serial.print((float)pMultiRangingData->RangeData[j].SignalRateRtnMegaCps / 65536.0);
      Serial.print(" Mcps, Ambient=");
      Serial.print((float)pMultiRangingData->RangeData[j].AmbientRateRtnMegaCps / 65536.0);
      Serial.print(" Mcps");
    }
    Serial.println("");
    //if (status == 0) {
    //  status = sensor_vl53l4cx_sat1.VL53L4CX_ClearInterruptAndStartMeasurement();
    //}
    //sensor_vl53l4cx_sat1.VL53L4CX_StopMeasurement();
  }

  delay(50);
}
