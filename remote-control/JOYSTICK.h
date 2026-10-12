// MiniJoystick.h
#ifndef MINIJOYSTICK_H
#define MINIJOYSTICK_H

#include <Arduino.h>

class MiniJoystick {
private:
  int xPin;
  int yPin;
  int buttonPin;
  char xDirection;
  char xPower[4];
  char yDirection;
  char yPower[4];
  boolean buttonOn = false;

  // Valores de calibración
  int xCenter;
  int xMin;
  int xMax;
  int yCenter;
  int yMin;
  int yMax;
  int deadzone;

public:
  // Constructor con calibración
  MiniJoystick(int joystickXPin, int joystickYPin, int joystickButtonPin,
               int xMinVal, int xCenterVal, int xMaxVal,
               int yMinVal, int yCenterVal, int yMaxVal,
               int deadzoneVal = 20) {
    xPin = joystickXPin;
    yPin = joystickYPin;
    buttonPin = joystickButtonPin;
    pinMode(buttonPin, INPUT_PULLUP);

    xMin = xMinVal;
    xCenter = xCenterVal;
    xMax = xMaxVal;
    yMin = yMinVal;
    yCenter = yCenterVal;
    yMax = yMaxVal;
    deadzone = deadzoneVal;
  }

  char getXDirection() { return xDirection; }
  String getXPower() { return xPower; }
  char getYDirection() { return yDirection; }
  String getYPower() { return yPower; }
  boolean buttonIsOn() { return buttonOn; }

  // Devuelve valores procesados con calibración y límite máximo
  String getState() {
    int xP = 0;
    int yP = 0;
    int x = analogRead(xPin);
    int y = analogRead(yPin);

    // X axis
    if (x >= xCenter - deadzone && x <= xCenter + deadzone) {
      xP = 0;
      xDirection = 'C';
    } else if (x < xCenter - deadzone) {
      xP = map(x, xMin, xCenter - deadzone, 255, 0);
      xDirection = 'L';
    } else {
      xP = map(x, xCenter + deadzone, xMax, 0, 255);
      xDirection = 'R';
    }
    xP = constrain(xP, 0, 255);  // Limita a 0–255
    sprintf(xPower, "%03d", xP);

    // Y axis
    if (y >= yCenter - deadzone && y <= yCenter + deadzone) {
      yP = 0;
      yDirection = 'C';
    } else if (y < yCenter - deadzone) {
      yP = map(y, yMin, yCenter - deadzone, 255, 0);
      yDirection = 'D';
    } else {
      yP = map(y, yCenter + deadzone, yMax, 0, 255);
      yDirection = 'U';
    }
    yP = constrain(yP, 0, 255);  // Limita a 0–255
    sprintf(yPower, "%03d", yP);

    // Botón
    buttonOn = digitalRead(buttonPin) == LOW;

    return String(xDirection) + String(xPower) + String(yDirection) + String(yPower) + (buttonOn ? "1" : "0");
  }

  // Devuelve valores crudos del joystick
  String getRawValues() {
    int xRaw = analogRead(xPin);
    int yRaw = analogRead(yPin);
    int buttonState = digitalRead(buttonPin);

    return "X: " + String(xRaw) + " Y: " + String(yRaw) + " Btn: " + String(buttonState);
  }
};

#endif
