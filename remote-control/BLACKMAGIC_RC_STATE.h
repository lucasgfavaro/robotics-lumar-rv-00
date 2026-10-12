#ifndef BLACKMAGIC_RC_STATE_H
#define BLACKMAGIC_RC_STATE_H

#include <Arduino.h>

class BlackMagicRemoteControlState {
private:
  char leftMiniJoystickXDirection;
  String leftMiniJoystickXPower;
  char leftMiniJoystickYDirection;
  String leftMiniJoystickYPower;
  char leftMiniJoystickButtonOn;
  char leftButtonOn;
  char leftToggleSwitchOn;
  char rightMiniJoystickXDirection;
  String rightMiniJoystickXPower;
  char rightMiniJoystickYDirection;
  String rightMiniJoystickYPower;
  char rightMiniJoystickButtonOn;
  char rightButtonOn;
  char rightToggleSwitchOn;

public:
  BlackMagicRemoteControlState();
  void flushState();
  void setStateFromCommand(String state);
  void setState(String leftMiniJoystickState, char leftButtonState, char leftToggleSwitchState,
                String rightMiniJoystickState, char rightButtonState, char rightToggleSwitchState);
  String getState();
  char getLeftMiniJoystickXDirection();
  String getLeftMiniJoystickXPower();
  char getLeftMiniJoystickYDirection();  // corregido nombre
  String getLeftMiniJoystickYPower();
  boolean getLeftMiniJoystickButtonOn();
  boolean getLeftButtonOn();
  boolean getLeftToggleSwitchOn();
  char getRightMiniJoystickXDirection();
  String getRightMiniJoystickXPower();
  char getRightMiniJoystickYDirection();
  String getRightMiniJoystickYPower();
  boolean getRightMiniJoystickButtonOn();
  boolean getRightButtonOn();
  boolean getRightToggleSwitchOn();
  bool validCommand(const String &cmd);
};

#endif // BLACKMAGIC_RC_STATE_H
