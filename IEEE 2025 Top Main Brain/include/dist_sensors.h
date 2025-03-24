#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cx_class.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include "main_brain_pins.h"

// -----------------------------------------------------------------------------------------
// library used for the VL53L4CX distance sensors: stm32duino/STM32duino VL53L4CX@^1.1.0
// 
// setup and get measurement functions for use of the distance sensors
// -----------------------------------------------------------------------------------------

#define DEV_I2C Wire

// Object definitions for each sensor
VL53L4CX front_distsensor(&DEV_I2C, XSHUT2);
VL53L4CX frontleft_distsensor(&DEV_I2C, XSHUT3);
VL53L4CX backleft_distsensor(&DEV_I2C, XSHUT4);
VL53L4CX backright_distsensor(&DEV_I2C, XSHUT5);

// Globals that are being shared by all the distance sensors.
// this currently seems to work fine without any issues.
VL53L4CX_MultiRangingData_t rangingData;
VL53L4CX_MultiRangingData_t *pRangingData = &rangingData;
uint8_t NewDataReady;
int status;

void distsensor_setup(){
    // Configure VL53L4CX distance sensors.
    front_distsensor.begin();
    frontleft_distsensor.begin();
    backleft_distsensor.begin();
    backright_distsensor.begin();

    // Switch off distance sensors (via XSHUT pins).
    front_distsensor.VL53L4CX_Off();
    frontleft_distsensor.VL53L4CX_Off();
    backleft_distsensor.VL53L4CX_Off();
    backright_distsensor.VL53L4CX_Off();

    // Initialize distance sensors.
    // Arguement being passed is the I2C address we are giving each one. Hello world 
    // example had 0x12 as the address and the datasheet says there is a difference 
    // between odd/even address value because of the LSB, so we are doing all even addresses.
    front_distsensor.InitSensor(0x12);
    frontleft_distsensor.InitSensor(0x14);
    backleft_distsensor.InitSensor(0x16);
    backright_distsensor.InitSensor(0x18);

    // Start Measurements
    front_distsensor.VL53L4CX_StartMeasurement();
    frontleft_distsensor.VL53L4CX_StartMeasurement();
    backleft_distsensor.VL53L4CX_StartMeasurement();
    backright_distsensor.VL53L4CX_StartMeasurement();

}



// Takes 1 reading from the front sensor when the function is called.
// Returns a struct pointer to a struct that holds status, distances, stream count, (and a couple of things),
// for each object it "saw".
VL53L4CX_MultiRangingData_t* front_distsensor_GetMeasurement(){
    int num_objects_found = 0;
    if (status == 0){
        // This has to be called to start new measurment (even though we are technically not using interrupts)
        status = front_distsensor.VL53L4CX_ClearInterruptAndStartMeasurement();
    }
    do {
        status = front_distsensor.VL53L4CX_GetMeasurementDataReady(&NewDataReady);
    } while (!NewDataReady);    // Waits for onboard driver on sensor to do handshake process and say data is ready

    if ((!status) && (NewDataReady != 0)) {
        // Actually get measurement data and store in rangingData struct via struct* pRangingData
        // This is a little funky how they soley use the struct pointer, but I assume it is for a specific reason with how the library API works.
        status = front_distsensor.VL53L4CX_GetMultiRangingData(pRangingData);
        num_objects_found = pRangingData->NumberOfObjectsFound;     // Put number of objects from the reading in variable for ease of use (example did this too)
        Serial.printf("Front Sensor:  Count=%d, #Objs=%d ", pRangingData->StreamCount, num_objects_found);  // If no objects, only this part gets printed.
        for (int j = 0; j < num_objects_found; j++) {
            if (j != 0){    // If more than 1 object, print a large indentation.
                Serial.print("\r\n                               ");
            }
            Serial.print("status=");
            Serial.print(pRangingData->RangeData[j].RangeStatus);   // Print status
            Serial.print(", D=");
            Serial.print(pRangingData->RangeData[j].RangeMilliMeter);   // Print distance of object
            Serial.print("mm");

            // These aren't super relevant to us:
            //Serial.print(", Signal=");
            //Serial.print((float)pRangingData->RangeData[j].SignalRateRtnMegaCps / 65536.0);   
            //Serial.print(" Mcps, Ambient=");
            //Serial.print((float)pRangingData->RangeData[j].AmbientRateRtnMegaCps / 65536.0);
            //Serial.print(" Mcps");
        }
        Serial.println("");
    }
    return pRangingData;
}

// Takes 1 reading from the front left sensor when the function is called.
// Returns a struct pointer to a struct that holds status, distances, stream count, (and a couple of things),
// for each object it "saw".
VL53L4CX_MultiRangingData_t* frontleft_distsensor_GetMeasurement(){
    int num_objects_found = 0;
    if (status == 0){
        // This has to be called to start new measurment (even though we are technically not using interrupts)
        status = frontleft_distsensor.VL53L4CX_ClearInterruptAndStartMeasurement();
    }
    do {
        status = frontleft_distsensor.VL53L4CX_GetMeasurementDataReady(&NewDataReady);
    } while (!NewDataReady);    // Waits for onboard driver on sensor to do handshake process and say data is ready

    if ((!status) && (NewDataReady != 0)) {
        // Actually get measurement data and store in rangingData struct via struct* pRangingData
        // This is a little funky how they soley use the struct pointer, but I assume it is for a specific reason with how the library API works.
        status = frontleft_distsensor.VL53L4CX_GetMultiRangingData(pRangingData);
        num_objects_found = pRangingData->NumberOfObjectsFound;     // Put number of objects from the reading in variable for ease of use (example did this too)
        Serial.printf("FrontLeft Sensor:  Count=%d, #Objs=%d ", pRangingData->StreamCount, num_objects_found);  // If no objects, only this part gets printed.
        for (int j = 0; j < num_objects_found; j++) {
            if (j != 0){    // If more than 1 object, print a large indentation.
                Serial.print("\r\n                               ");
            }
            Serial.print("status=");
            Serial.print(pRangingData->RangeData[j].RangeStatus);   // Print status
            Serial.print(", D=");
            Serial.print(pRangingData->RangeData[j].RangeMilliMeter);   // Print distance of object
            Serial.print("mm");

            // These aren't super relevant to us:
            //Serial.print(", Signal=");
            //Serial.print((float)pRangingData->RangeData[j].SignalRateRtnMegaCps / 65536.0);   
            //Serial.print(" Mcps, Ambient=");
            //Serial.print((float)pRangingData->RangeData[j].AmbientRateRtnMegaCps / 65536.0);
            //Serial.print(" Mcps");
        }
        Serial.println("");
    }
    return pRangingData;
}

// Takes 1 reading from the back left sensor when the function is called.
// Returns a struct pointer to a struct that holds status, distances, stream count, (and a couple of things),
// for each object it "saw".
VL53L4CX_MultiRangingData_t* backleft_distsensor_GetMeasurement(){
    int num_objects_found = 0;
    if (status == 0){
        // This has to be called to start new measurment (even though we are technically not using interrupts)
        status = backleft_distsensor.VL53L4CX_ClearInterruptAndStartMeasurement();
    }
    do {
        status = backleft_distsensor.VL53L4CX_GetMeasurementDataReady(&NewDataReady);
    } while (!NewDataReady);    // Waits for onboard driver on sensor to do handshake process and say data is ready

    if ((!status) && (NewDataReady != 0)) {
        // Actually get measurement data and store in rangingData struct via struct* pRangingData
        // This is a little funky how they soley use the struct pointer, but I assume it is for a specific reason with how the library API works.
        status = backleft_distsensor.VL53L4CX_GetMultiRangingData(pRangingData);
        num_objects_found = pRangingData->NumberOfObjectsFound;     // Put number of objects from the reading in variable for ease of use (example did this too)
        Serial.printf("BackLeft Sensor:  Count=%d, #Objs=%d ", pRangingData->StreamCount, num_objects_found);   // If no objects, only this part gets printed.
        for (int j = 0; j < num_objects_found; j++) {
            if (j != 0){    // If more than 1 object, print a large indentation.
                Serial.print("\r\n                               ");
            }
            Serial.print("status=");
            Serial.print(pRangingData->RangeData[j].RangeStatus);   // Print status
            Serial.print(", D=");
            Serial.print(pRangingData->RangeData[j].RangeMilliMeter);   // Print distance of object
            Serial.print("mm");

            // These aren't super relevant to us:
            //Serial.print(", Signal=");
            //Serial.print((float)pRangingData->RangeData[j].SignalRateRtnMegaCps / 65536.0);   
            //Serial.print(" Mcps, Ambient=");
            //Serial.print((float)pRangingData->RangeData[j].AmbientRateRtnMegaCps / 65536.0);
            //Serial.print(" Mcps");
        }
        Serial.println("");
    }
    return pRangingData;
}

// Takes 1 reading from the back right sensor when the function is called.
// Returns a struct pointer to a struct that holds status, distances, stream count, (and a couple of things),
// for each object it "saw".
VL53L4CX_MultiRangingData_t* backright_distsensor_GetMeasurement(){
    int num_objects_found = 0;

    if (status == 0){
        // This has to be called to start new measurment (even though we are technically not using interrupts) 
        status = backleft_distsensor.VL53L4CX_ClearInterruptAndStartMeasurement();
    }
    do {
        status = backleft_distsensor.VL53L4CX_GetMeasurementDataReady(&NewDataReady);
    } while (!NewDataReady);    // Waits for onboard driver on sensor to do handshake process and say data is ready

    if ((!status) && (NewDataReady != 0)) {
        // Actually get measurement data and store in rangingData struct via struct* pRangingData
        // This is a little funky how they soley use the struct pointer, but I assume it is for a specific reason with how the library API works.
        status = backleft_distsensor.VL53L4CX_GetMultiRangingData(pRangingData);    
        num_objects_found = pRangingData->NumberOfObjectsFound; // Put number of objects from the reading in variable for ease of use (example did this too)
        Serial.printf("BackRight Sensor:  Count=%d, #Objs=%d ", pRangingData->StreamCount, num_objects_found);  // If no objects, only this part gets printed.
        for (int j = 0; j < num_objects_found; j++) {
            if (j != 0){    // If more than 1 object, print a large indentation.
                Serial.print("\r\n                               ");
            }
            Serial.print("status=");
            Serial.print(pRangingData->RangeData[j].RangeStatus);   // Print status
            Serial.print(", D=");
            Serial.print(pRangingData->RangeData[j].RangeMilliMeter);   // Print distance of object
            Serial.print("mm");

            // These aren't super relevant to us:
            //Serial.print(", Signal=");
            //Serial.print((float)pRangingData->RangeData[j].SignalRateRtnMegaCps / 65536.0);   
            //Serial.print(" Mcps, Ambient=");
            //Serial.print((float)pRangingData->RangeData[j].AmbientRateRtnMegaCps / 65536.0);
            //Serial.print(" Mcps");
        }
        Serial.println("");
    }
    return pRangingData;    // Return struct pointer to the struct that holds all the data from reading.
}



// ------------------- Example Usage -------------------------------------

// #include <Arduino.h>
// #include <Wire.h>
// #include <vl53l4cx_class.h>
// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <stdint.h>
// #include <assert.h>
// #include <stdlib.h>
// #include "dist_sensors.h"

// #define DEV_I2C Wire
// //#define SerialPort Serial

// VL53L4CX_MultiRangingData_t *front_rangeData;
// VL53L4CX_MultiRangingData_t *frontleft_rangeData;
// VL53L4CX_MultiRangingData_t *backleft_rangeData;
// VL53L4CX_MultiRangingData_t *backright_rangeData;


// /* Setup ---------------------------------------------------------------------*/

// void setup()
// {
//   // Initialize serial for output.
//   Serial.begin(9600);
//   while (!Serial);
//   Serial.println("Starting...");

//   // Initialize I2C bus.
//   DEV_I2C.begin();

//   distsensor_setup();

// }

// void loop()
// {
//   for(int i = 0; i < 50; i++){
//     front_rangeData = front_distsensor_GetMeasurement();
//     frontleft_rangeData = frontleft_distsensor_GetMeasurement();
//     backleft_rangeData = backleft_distsensor_GetMeasurement();
//     backright_rangeData = backright_distsensor_GetMeasurement();
//     delay(35);
//   }

//   while(1){

//   }
// }