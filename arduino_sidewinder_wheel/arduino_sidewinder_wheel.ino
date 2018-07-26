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
// axis
int pin_axis_wheel = A0;
int pin_axis_throttle = A1;
int pin_axis_brake = A2;
int val_axis_wheel = 0;
int val_axis_throttle = 0;
int val_axis_brake = 0;

// default values for axis calibration
#define WHEEL_LOW 70
#define WHEEL_HIGH 950
#define THROTTLE_LOW 280
#define THROTTLE_HIGH 750
#define BRAKE_LOW 305
#define BRAKE_HIGH 735

// enable debug mode to get read values and timing from serial connection
boolean DEBUG_MODE = false;
unsigned long time;

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
  mJoystick.setAcceleratorRange(THROTTLE_LOW, THROTTLE_HIGH);
  mJoystick.setBrakeRange(BRAKE_HIGH, BRAKE_LOW);
}

void loop() 
{
  if(DEBUG_MODE)
  {
    time = millis();
  }
  
  // read buttons (multiplexed)
  digitalWrite(pin_multiplex_1, 1);
  digitalWrite(pin_multiplex_2, 0);
  val_button_4 = !digitalRead(pin_button_1);
  val_button_5 = !digitalRead(pin_button_2);
  val_button_6 = !digitalRead(pin_button_3);
  val_button_8 = !digitalRead(pin_button_4);
  digitalWrite(pin_multiplex_1, 0);
  digitalWrite(pin_multiplex_2, 1);
  val_button_1 = !digitalRead(pin_button_1);
  val_button_2 = !digitalRead(pin_button_2);
  val_button_3 = !digitalRead(pin_button_3);
  val_button_7 = !digitalRead(pin_button_4);

  // read axis
  val_axis_wheel = analogRead(pin_axis_wheel);
  val_axis_throttle = analogRead(pin_axis_throttle);
  val_axis_brake = analogRead(pin_axis_brake);
  
  // set buttons
  mJoystick.setButton(0, val_button_1);
  mJoystick.setButton(1, val_button_2);
  mJoystick.setButton(2, val_button_3);
  mJoystick.setButton(3, val_button_4);
  mJoystick.setButton(4, val_button_5);
  mJoystick.setButton(5, val_button_6);
  mJoystick.setButton(6, val_button_7);
  mJoystick.setButton(7, val_button_8);

  // set axis
  mJoystick.setSteering(val_axis_wheel);
  mJoystick.setAccelerator(val_axis_throttle);
  mJoystick.setBrake(val_axis_brake);

  // joystick logic takes between 8 and 11 ms from loop start until here

  // dump raw values in debug mode
  if(DEBUG_MODE)
  {
    Serial.println("time for joystick logic in ms");
    Serial.println(millis() - time);
    
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
    Serial.println(val_axis_throttle);
    Serial.println(val_axis_brake); 

    // add a delay of 1 second to make output easy readable
    delay(1000);
  }
}

