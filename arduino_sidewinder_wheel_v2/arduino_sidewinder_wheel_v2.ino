#include <Joystick.h>

// create a joystick instance (here steering wheel with pedals and buttons)
Joystick_ joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
  9, 0,                   // button count and hat switch count
  false, false, false,    // X, Y and Z axis
  false, false, false,    // rx, ry, or rz
  false, false,           // rudder or throttle
  true, true, true        // accelerator, brake and steering axis
);

// timing variables
long last_millis = 0;
long delta_millis = 20;

// 9 buttons (filled from LSB with button state)
int buttons = 0x0000;
int count_buttons = 9;
// using pins 2-10
int button_pin_offset = 2;
int pin_multiplex_1 = 14;
int pin_multiplex_2 = 15;

// three axis (calibration done in OS)
int axis_pin_wheel = A3;
int axis_pin_accelerator = A1;
int axis_pin_brake = A2;
int axis_wheel_val = 0;
int axis_accelerator_val = 0;
int axis_brake_val = 0;

inline void read_inputs() { 

  // reset button state
  buttons = 0x0000;

  // multiplex 1
  digitalWrite(pin_multiplex_1, 1);
  digitalWrite(pin_multiplex_2, 0);
  for(int i = 0; i <= 4; i++) {
    buttons |= !digitalRead(i + button_pin_offset) << i;
  }
  // multiplex 2
  digitalWrite(pin_multiplex_1, 0);
  digitalWrite(pin_multiplex_2, 1);
  for(int i = 5; i < count_buttons; i++) {
    buttons |= !digitalRead(i + button_pin_offset) << i;
  }

  axis_wheel_val = analogRead(axis_pin_wheel);
  axis_accelerator_val = analogRead(axis_pin_accelerator);
  axis_brake_val = analogRead(axis_pin_brake);
}

inline void send_state_to_host() {

  // set all buttons
  for(int i = 0; i < count_buttons; i++) {
    // get bit for buttons and set
    bool button_bit = (buttons & ( 1 << i )) >> i;
    joystick.setButton(i, button_bit);
  }
  
  // set all axis
  joystick.setSteering(axis_wheel_val);
  joystick.setAccelerator(axis_accelerator_val);
  joystick.setBrake(axis_brake_val);

  // send state to host once here (minimal overhead solution)
  joystick.sendState();
}

void setup() {
   Serial.begin(115200);
  
  // setup buttons
  pinMode(pin_multiplex_1, OUTPUT);
  pinMode(pin_multiplex_2, OUTPUT);
  for(int i = 0; i <= count_buttons; i++) {
    pinMode(i + button_pin_offset, INPUT_PULLUP);
  }

  // setup joystick (do not auto send state)
  joystick.begin(false);
}

void loop() {
  if(millis() - last_millis >= delta_millis) {
    last_millis = millis();

    // read buttons
    read_inputs();

    // send state to host
    send_state_to_host();
  }
}
