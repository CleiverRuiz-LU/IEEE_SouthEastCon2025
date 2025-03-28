// pin definitions for top board with all the motors
// Feather M4 Express pinout

// I2C
#define SCL 22
#define SDA 21

// UART
#define RX 0
#define TX 1

// Servos
#define sorting_servo_PWM 19
#define mast_servo_PWM 18

// Steppers
#define lbucket_lift_DIR 23
#define lbucket_lift_PWM 4

#define rbucket_lift_DIR 5
#define rbucket_lift_PWM 6

// DC Motors
#define lbucket_pinch_DIR 9
#define lbucket_pinch_PWM 10

#define rbucket_pinch_DIR 11
#define rbucket_pinch_PWM 12

#define ferriswheel_DIR 25
#define ferriswheel_PWM 13

#define convey_DIR 17
#define convey_PWM 16

// Current Sensor
#define current_sensor_read_pin 14

// Switch
#define ferriswheel_rot_switch 24

// Stepper Enable
#define stepper_enable 15