/*
  AFMotor Library https://learn.adafruit.com/adafruit-motor-shield/library-install
*/
#include "ROVER.h"
#include <AFMotor.h>
#include <SoftwareSerial.h>
#include "BLACKMAGIC_RC_STATE.h"

SoftwareSerial bluetoothSerial(9, 10);  // RX, TX
AF_DCMotor leftMotor1(1, MOTOR12_64KHZ);
AF_DCMotor leftMotor2(2, MOTOR12_64KHZ);
AF_DCMotor rightMotor1(3, MOTOR34_64KHZ);
AF_DCMotor rightMotor2(4, MOTOR34_64KHZ);
const int frontLightsPin = A0;
const int hornPin = A1;


boolean debug = true;
BlackMagicRemoteControlState remoteControlState;
  Rover rover( &leftMotor1, &leftMotor2, &rightMotor1, &rightMotor2, frontLightsPin, hornPin, debug );

void setup() {
  if (debug) {
    Serial.begin(9600);
  }
  bluetoothSerial.begin(38400);
}

void loop() {
  if (bluetoothSerial.available()) {
    String command = bluetoothSerial.readStringUntil('\n');

    if (remoteControlState.validCommand(command)) {
      remoteControlState.setStateFromCommand(command);
    }

   rover.processCommand(&remoteControlState);
  }
}


