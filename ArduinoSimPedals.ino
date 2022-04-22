/*
 *  Arduino Sim Pedals by lasevi
 *
 *  https://github.com/lasevi/ArduinoSimPedals (probably not public)
 *
 *
 *  You need an arduino with the AtMega32u4 microcontroller.
 *  That means Leonardo, Micro, Pro Micro etc would work.
 *  I use a chinese Pro Micro clone, works well.
 *
 */

// Change this if you want to enable / disable serial printing.
#define DEBUG false

// ArduinoJoystickLibrary by MHeironimus
// https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <Joystick.h>

#define JOYSTICK_AUTO_SEND_STATE false

// Refer to your board's manual to find it's ADC pins
#define THROTTLE_POTENTIOMETER_PIN A0
//#define BRAKE_POTENTIOMETER_PIN A1
#define JOYSTICK_THROTTLE_MIN_VALUE 0
#define JOYSTICK_THROTTLE_MAX_VALUE 1023
//#define JOYSTICK_BRAKE_MIN_VALUE 0
//#define JOYSTICK_BRAKE_MAX_VALUE 1023

// Milliseconds per loop.
#define LOOP_TIME 1

// Only Throttle and Brake needed in this case.
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, // hidReportId
  JOYSTICK_TYPE_JOYSTICK,     // joystickType
  0,     // buttonCount
  0,     // hatSwitchCount
  false, // includeXAxis
  false, // includeYAxis 
  false, // includeZAxis
  false, // includeRxAxis
  false, // includeRyAxis
  false, // includeRzAxis
  false, // includeRudder
  true,  // includeThrottle
  false, // includeAccelerator
  false,  // includeBrake
  false  // includeSteering
); 


uint16_t throttle_position = 0;
uint16_t throttle_position_lowpass = 0;
//uint16_t brake_position = 0;
//uint16_t brake_position_lowpass = 0;


void setup(){
  Joystick.begin(JOYSTICK_AUTO_SEND_STATE);

  Joystick.setThrottleRange(JOYSTICK_THROTTLE_MIN_VALUE, JOYSTICK_THROTTLE_MAX_VALUE);
  //Joystick.setBrakeRange(JOYSTICK_BRAKE_MIN_VALUE, JOYSTICK_BRAKE_MAX_VALUE);

  // Rot really needed, as all pins are input by default
  pinMode(THROTTLE_POTENTIOMETER_PIN, INPUT);
  //pinMode(BRAKE_POTENTIOMETER_PIN, INPUT);

  #if DEBUG
    Serial.begin(9600);
  #endif
}

void loop(){
  throttle_position = analogRead(THROTTLE_POTENTIOMETER_PIN);
  //brake_position    = analogRead(BRAKE_POTENTIOMETER_PIN);

  // Super simple low pass filter to get rid of some noise
  throttle_position_lowpass = 0.5 * throttle_position + 0.5 * throttle_position_lowpass;
  //brake_position_lowpass    = 0.5 * brake_position    + 0.5 * brake_position_lowpass;

  Joystick.setThrottle(throttle_position_lowpass);
  //Joystick.setBrake(brake_position_lowpass);

  #if !JOYSTICK_AUTO_SEND_STATE
    Joystick.sendState();
  #endif

  #if DEBUG
    Serial.print("throttle: ");
    Serial.print(throttle_position);
    Serial.print(" thr_smooth: ");
    Serial.print(throttle_position_lowpass);
    //Serial.print(" brake: ");
    //Serial.print(brake_position);
    //Serial.print(" brk_smooth: ");
    //Serial.print(brake_position_lowpass);
    Serial.println();
  #endif

  delay(LOOP_TIME);
}
