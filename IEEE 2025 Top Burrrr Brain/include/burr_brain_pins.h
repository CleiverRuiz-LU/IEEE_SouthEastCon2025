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
#define trap_servo_PWM 17

// Steppers
#define lbucket_step_DIR 23
#define lbucket_step_PWM 4

#define rbucket_step_DIR 5
#define rbucket_step_PWM 6

// DC Motors
#define lbucket_pinch_DC_DIR 9
#define lbucket_pinch_DC_PWM 10

#define rbucket_pinch_DC_DIR 11
#define rbucket_pinch_DC_PWM 12

#define ferriswheel_DC_DIR 25
#define ferriswheel_DC_PWM 13

// Current Sensor
#define current_sensor_read_pin 15

// Switch
#define ferriswheel_rot_switch 24