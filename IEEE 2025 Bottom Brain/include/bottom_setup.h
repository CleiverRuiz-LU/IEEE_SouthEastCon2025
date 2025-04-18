#include <Arduino.h>
#include "CytronMotorDriver.h"
#include "bottom_plate_pins.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"


////////////////////////// Motor Driver Setup ///////////////////////////////////////////////

// Configure the motor driver objects
CytronMD motor1(PWM_DIR, PWM1, DIR1);  // PWM1 = Pin 17, DIR1 = Pin 16.
CytronMD motor2(PWM_DIR, PWM2, DIR2);  // PWM2 = Pin 13, DIR2 = Pin 12.
CytronMD motor3(PWM_DIR, PWM3, DIR3);  // PWM3 = Pin 18, DIR3 = Pin 19.
CytronMD motor4(PWM_DIR, PWM4, DIR4);  // PWM4 = Pin 11, DIR4 = Pin 10.


////////////////////// Gyro Setup ///////////////////////////////////////////////////////////

// MPU6050 default I2C address in 0x68, 
// but our stuff was a diva and wanted it explicitly declared
MPU6050 mpu(0x68);      // Setup accelerometer/gyroscope object and address


/*---MPU6050 Control/Status Variables---*/
bool DMPReady = false;  // Set true if DMP init was successful
uint8_t devStatus;      // Return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // Expected DMP packet size (default is 42 bytes)
uint8_t FIFOBuffer[64]; // FIFO storage buffer

/*---Orientation/Motion Variables---*/ 
Quaternion q;           // [w, x, y, z]         Quaternion container
VectorInt16 aa;         // [x, y, z]            Accel sensor measurements
VectorInt16 gy;         // [x, y, z]            Gyro sensor measurements
VectorInt16 aaReal;     // [x, y, z]            Gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            World-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            Gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   Yaw/Pitch/Roll container and gravity vector

void gyro_setup(){
    // idk what this ifelse really does but I didn't want to mess it up
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment on this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    /*Initialize device*/
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    delay(500);

    /*Verify connection*/
    Serial.println(F("Testing MPU6050 connection..."));
    if(mpu.testConnection() == false){
        Serial.println("MPU6050 connection failed");
        while(true);    // get stuck in forever loop if failed
    } else {
        Serial.println("MPU6050 connection successful");
    }

    /* Initializate and configure the DMP*/
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    /* Supply your gyro offsets here, scaled for min sensitivity */
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);

    /* Making sure it worked (returns 0 if so) */ 
    if (devStatus == 0) {
        mpu.CalibrateAccel(6);  // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateGyro(6);
        Serial.println("These are the Active offsets: ");
        mpu.PrintActiveOffsets();
        Serial.println(F("Enabling DMP..."));   //Turning ON DMP
        mpu.setDMPEnabled(true);

        /* Set the DMP Ready flag so the main loop() function knows it is okay to use it */
        DMPReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize(); //Get expected DMP packet size for later comparison
    } 
    else {
        Serial.print(F("DMP Initialization failed (code ")); //Print the error code
        Serial.print(devStatus);
        Serial.println(F(")"));
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
    }
    Serial.println(F("Gyro setup complete!"));
}