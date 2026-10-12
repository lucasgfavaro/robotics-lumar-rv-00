// Rover.h
#ifndef ROVER_H
#define ROVER_H

#include <AFMotor.h>
#include "BLACKMAGIC_RC_STATE.h"

// Valor mínimo del joystick para que los motores respondan
#define MIN_INPUT_POWER 120

class Rover {
private:
  AF_DCMotor* leftMotor1;
  AF_DCMotor* leftMotor2;
  AF_DCMotor* rightMotor1;
  AF_DCMotor* rightMotor2;
  boolean debugOn;
  int frontLightsPin;
  int hornPin;


  // --- Métodos helper privados ---
  void handleMovement(BlackMagicRemoteControlState* state) {
    if (state->getRightButtonOn()) {
      if (debugOn) Serial.println("Slalom");
      startSlalom();
      return;
    }

    Stop();

    char yDir = state->getLeftMiniJoystickYDirection();
    char xDir = state->getRightMiniJoystickXDirection();
    String yPower = state->getLeftMiniJoystickYPower();
    String xPower = state->getRightMiniJoystickXPower();
    bool rightToggle = state->getRightToggleSwitchOn();

    if (yDir == 'U') forward(yPower, xDir, xPower, rightToggle);
    else if (yDir == 'D') backward(yPower, xDir, xPower, rightToggle);
    else if (xDir == 'R') right(xPower, rightToggle);
    else if (xDir == 'L') left(xPower, rightToggle);
  }

  void handleLights(BlackMagicRemoteControlState* state) {
    digitalWrite(frontLightsPin, state->getLeftToggleSwitchOn() ? HIGH : LOW);
  }

  void handleHorn(BlackMagicRemoteControlState* state) {
    if (state->getLeftButtonOn()) {
      tone(hornPin, 330, 200); // ejemplo de tono
      delay(250);
    } else {
      noTone(hornPin);
    }
  }

  int calculateLeftSpeed(int forwardSpeed, char steering, String steeringPower) {
    if (steering == 'L') {
      int sideSpeed = map(steeringPower.toInt(), 0, 255, forwardSpeed, 0);
      return sideSpeed;
    } else
      return forwardSpeed;
  }

  int calculateRightSpeed(int forwardSpeed, char steering, String steeringPower) {
    if (steering == 'R') {
      int sideSpeed = map(steeringPower.toInt(), 0, 255, forwardSpeed, 0);
      return sideSpeed;
    } else
      return forwardSpeed;
  }

  void forward(String forwardPower, char steering, String steeringPower, bool rightToggleOn) {
    int forwardSpeed = map(forwardPower.toInt(), 0, 255, MIN_INPUT_POWER, 255);
    if (!rightToggleOn) forwardSpeed = forwardSpeed / 2;

    int speedLeft = calculateLeftSpeed(forwardSpeed, steering, steeringPower);
    int speedRight = calculateRightSpeed(forwardSpeed, steering, steeringPower);

    leftMotor1->setSpeed(speedLeft);
    leftMotor2->setSpeed(speedLeft);
    rightMotor1->setSpeed(speedRight);
    rightMotor2->setSpeed(speedRight);

    leftMotor1->run(FORWARD);
    leftMotor2->run(FORWARD);
    rightMotor1->run(FORWARD);
    rightMotor2->run(FORWARD);

    if (debugOn)
      Serial.println("FORWARD: " + String(forwardSpeed) + " | " + String(speedLeft) + " | " + String(speedRight));
  }

  void backward(String backwardPower, char steering, String steeringPower, bool rightToggleOn) {
    int backwardSpeed = map(backwardPower.toInt(), 0, 255, MIN_INPUT_POWER, 255);
    if (!rightToggleOn) backwardSpeed = backwardSpeed / 2;

    int speedLeft = calculateLeftSpeed(backwardSpeed, steering, steeringPower);
    int speedRight = calculateRightSpeed(backwardSpeed, steering, steeringPower);

    leftMotor1->run(BACKWARD);
    rightMotor1->run(BACKWARD);
    leftMotor2->run(BACKWARD);
    rightMotor2->run(BACKWARD);
    leftMotor1->setSpeed(speedLeft);
    rightMotor1->setSpeed(speedRight);
    leftMotor2->setSpeed(speedLeft);
    rightMotor2->setSpeed(speedRight);

    if (debugOn)
      Serial.println("BACKWARD:" + String(backwardSpeed) + " | " + String(speedLeft) + " | " + String(speedRight));
  }

  void left(String power, bool rightToggleOn) {
    int rawSpeed = power.toInt();
    if (rawSpeed < MIN_INPUT_POWER) rawSpeed = 0;
    int speed = map(rawSpeed, 0, 255, MIN_INPUT_POWER, 255);
    if (!rightToggleOn) speed = speed / 2;

    leftMotor1->run(BACKWARD);
    leftMotor2->run(BACKWARD);
    rightMotor1->run(FORWARD);
    rightMotor2->run(FORWARD);
    leftMotor1->setSpeed(speed);
    leftMotor2->setSpeed(speed);
    rightMotor1->setSpeed(speed);
    rightMotor2->setSpeed(speed);

    if (debugOn)
      Serial.println("LEFT:" + String(speed));
  }

  void right(String power, bool rightToggleOn) {
    int rawSpeed = power.toInt();
    if (rawSpeed < MIN_INPUT_POWER) rawSpeed = 0;
    int speed = map(rawSpeed, 0, 255, MIN_INPUT_POWER, 255);
    if (!rightToggleOn) speed = speed / 2;

    leftMotor1->run(FORWARD);
    leftMotor2->run(FORWARD);
    rightMotor1->run(BACKWARD);
    rightMotor2->run(BACKWARD);
    leftMotor1->setSpeed(speed);
    leftMotor2->setSpeed(speed);
    rightMotor1->setSpeed(speed);
    rightMotor2->setSpeed(speed);

    if (debugOn)
      Serial.println("RIGHT:" + String(speed));
  }

  void Stop() {
    leftMotor1->run(RELEASE);
    rightMotor1->run(RELEASE);
    leftMotor2->run(RELEASE);
    rightMotor2->run(RELEASE);
    leftMotor1->setSpeed(0);
    rightMotor1->setSpeed(0);
    leftMotor2->setSpeed(0);
    rightMotor2->setSpeed(0);
  }

  void startSlalom() {
    String forwardSpeed = "255";
    char xPower[4];
    sprintf(xPower, "%03d", MIN_INPUT_POWER);

    forward(forwardSpeed, 'C', xPower, true);
    delay(10);

    for (int xP = 0; xP < 255; xP += 1) {
      sprintf(xPower, "%03d", xP);
      forward(forwardSpeed, 'R', xPower, true);
      delay(10);
    }

    forward(forwardSpeed, 'C', xPower, true);
    delay(10);

    for (int xP = MIN_INPUT_POWER; xP < 255; xP += 1) {
      sprintf(xPower, "%03d", xP);
      forward(forwardSpeed, 'L', xPower, true);
      delay(10);
    }
  }

public:
  Rover(AF_DCMotor* lMotor1, AF_DCMotor* lMotor2, AF_DCMotor* rMotor1, AF_DCMotor* rMotor2,
        int lightsPin, int hornPin, boolean debug) {
    leftMotor1 = lMotor1;
    leftMotor2 = lMotor2;
    rightMotor1 = rMotor1;
    rightMotor2 = rMotor2;
    this->frontLightsPin = lightsPin;
    this->hornPin = hornPin;
    debugOn = debug;
  }

  void processCommand(BlackMagicRemoteControlState* state) {
    if (debugOn) Serial.println("command:" + state->getState());
    handleMovement(state);
    handleLights(state);
    handleHorn(state);
  }
};

#endif
