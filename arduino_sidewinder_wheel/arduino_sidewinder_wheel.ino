#include <Joystick.h>

// buttons
int pin_multiplex_1 = 2;
int pin_multiplex_2 = 3;
int pin_button_1 = 4;
int pin_button_2 = 5;
int pin_button_3 = 6;
int pin_button_4 = 7;
int val_button_1 = 0;
int val_button_2 = 0;
int val_button_3 = 0;
int val_button_4 = 0;
int val_button_5 = 0;
int val_button_6 = 0;
int val_button_7 = 0;
int val_button_8 = 0;
int val_button_1_old = 0;
int val_button_2_old = 0;
int val_button_3_old = 0;
int val_button_4_old = 0;
int val_button_5_old = 0;
int val_button_6_old = 0;
int val_button_7_old = 0;
int val_button_8_old = 0;
// axis
int pin_axis_wheel = A0;
int pin_axis_accelerator = A1;
int pin_axis_brake = A2;
int val_axis_wheel = 0;
int val_axis_accelerator = 0;
int val_axis_brake = 0;

// default values for axis calibration
#define WHEEL_LOW 70
#define WHEEL_HIGH 950
#define ACCELERATOR_LOW 280
#define ACCELERATOR_HIGH 750
#define BRAKE_LOW 305
#define BRAKE_HIGH 735

// enable debug mode to get read values and timing from serial connection
boolean DEBUG_MODE = true;
unsigned long time_in_us;

// create a joystick instance (here steering wheel with pedals and buttons)
Joystick_ mJoystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
  8, 0,                   // button count and hat switch count
  false, false, false,    // X, Y and Z axis
  false, false, false,    // rx, ry, or rz
  false, false,           // rudder or throttle
  true, true, true        // accelerator, brake and steering axis
);

void setup() 
{
  // setup serial connection
  Serial.begin(9600);

  // setup multplex buttons
  pinMode(pin_multiplex_1, OUTPUT);
  pinMode(pin_multiplex_2, OUTPUT);
  
  // setup input buttons
  pinMode(pin_button_1, INPUT_PULLUP);
  pinMode(pin_button_2, INPUT_PULLUP);
  pinMode(pin_button_3, INPUT_PULLUP);
  pinMode(pin_button_4, INPUT_PULLUP);

  // setup joystick
  mJoystick.begin();
  mJoystick.setSteeringRange(WHEEL_HIGH, WHEEL_LOW);
  mJoystick.setAcceleratorRange(ACCELERATOR_LOW, ACCELERATOR_HIGH);
  mJoystick.setBrakeRange(BRAKE_HIGH, BRAKE_LOW);
}

void inline updateButton(int pin, int* val, int* val_old)
{
  // save old value
  *val_old = *val;
  
  // read new value
  *val = !digitalRead(pin);

  // only send to host if value has changed
  if(*val_old != *val)
  {
    mJoystick.setButton(pin, *val);
  }
}

void loop() 
{
  if(DEBUG_MODE)
  {
    time_in_us = micros();
  }
  
  // read und update buttons (multiplexed)
  digitalWrite(pin_multiplex_1, 1);
  digitalWrite(pin_multiplex_2, 0);
  updateButton(pin_button_1, &val_button_4, &val_button_4_old);
  updateButton(pin_button_2, &val_button_5, &val_button_5_old);
  updateButton(pin_button_3, &val_button_6, &val_button_6_old);
  updateButton(pin_button_4, &val_button_8, &val_button_8_old);
  
  digitalWrite(pin_multiplex_1, 0);
  digitalWrite(pin_multiplex_2, 1);
  updateButton(pin_button_1, &val_button_1, &val_button_1_old);
  updateButton(pin_button_2, &val_button_2, &val_button_2_old);
  updateButton(pin_button_3, &val_button_3, &val_button_3_old);
  updateButton(pin_button_4, &val_button_7, &val_button_7_old);

  // read axis
  val_axis_wheel = analogRead(pin_axis_wheel);
  val_axis_accelerator = analogRead(pin_axis_accelerator);
  val_axis_brake = analogRead(pin_axis_brake);

  // set axis
  mJoystick.setSteering(val_axis_wheel);
  mJoystick.setAccelerator(val_axis_accelerator);
  mJoystick.setBrake(val_axis_brake);

  // joystick logic takes between 1.1 and 2.2 ms from loop start until here

  // dump raw values in debug mode
  if(DEBUG_MODE)
  {
    Serial.println("time for joystick logic in us");
    Serial.println(micros() - time_in_us);

    Serial.println("buttons");
    Serial.println(val_button_1);
    Serial.println(val_button_2);
    Serial.println(val_button_3);
    Serial.println(val_button_4);
    Serial.println(val_button_5);
    Serial.println(val_button_6);
    Serial.println(val_button_7);
    Serial.println(val_button_8);
    
    Serial.println("axis");
    Serial.println(val_axis_wheel);
    Serial.println(val_axis_accelerator);
    Serial.println(val_axis_brake); 

    // add a delay of 1 second to make output easy readable
    delay(1000);
  }
}

