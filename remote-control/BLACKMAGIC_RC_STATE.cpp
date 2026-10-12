
#include "BLACKMAGIC_RC_STATE.h"

BlackMagicRemoteControlState::BlackMagicRemoteControlState() {}

void BlackMagicRemoteControlState::flushState() {
  setState("C000C0000", '0', '0', "C000C0000", '0', '0');
}

void BlackMagicRemoteControlState::setStateFromCommand(String state) {
  setState(state.substring(0, 9), state.charAt(9), state.charAt(10),
           state.substring(11, 20), state.charAt(20), state.charAt(21));
}

void BlackMagicRemoteControlState::setState(String leftMiniJoystickState, char leftButtonState, char leftToggleSwitchState,
                                            String rightMiniJoystickState, char rightButtonState, char rightToggleSwitchState) {
  leftMiniJoystickXDirection = leftMiniJoystickState.charAt(0);
  leftMiniJoystickXPower = leftMiniJoystickState.substring(1, 4);
  leftMiniJoystickYDirection = leftMiniJoystickState.charAt(4);
  leftMiniJoystickYPower = leftMiniJoystickState.substring(5, 8);
  leftMiniJoystickButtonOn = leftMiniJoystickState.charAt(8);
  leftButtonOn = leftButtonState;
  leftToggleSwitchOn = leftToggleSwitchState;

  rightMiniJoystickXDirection = rightMiniJoystickState.charAt(0);
  rightMiniJoystickXPower = rightMiniJoystickState.substring(1, 4);
  rightMiniJoystickYDirection = rightMiniJoystickState.charAt(4);
  rightMiniJoystickYPower = rightMiniJoystickState.substring(5, 8);
  rightMiniJoystickButtonOn = rightMiniJoystickState.charAt(8);
  rightButtonOn = rightButtonState;
  rightToggleSwitchOn = rightToggleSwitchState;
}

String BlackMagicRemoteControlState::getState() {
  return String(leftMiniJoystickXDirection) + String(leftMiniJoystickXPower) + String(leftMiniJoystickYDirection) + String(leftMiniJoystickYPower) +
         leftMiniJoystickButtonOn + leftButtonOn + leftToggleSwitchOn +
         String(rightMiniJoystickXDirection) + String(rightMiniJoystickXPower) + String(rightMiniJoystickYDirection) + String(rightMiniJoystickYPower) +
         rightMiniJoystickButtonOn + rightButtonOn + rightToggleSwitchOn;
}

char BlackMagicRemoteControlState::getLeftMiniJoystickXDirection() { return leftMiniJoystickXDirection; }
String BlackMagicRemoteControlState::getLeftMiniJoystickXPower() { return leftMiniJoystickXPower; }
char BlackMagicRemoteControlState::getLeftMiniJoystickYDirection() { return leftMiniJoystickYDirection; }
String BlackMagicRemoteControlState::getLeftMiniJoystickYPower() { return leftMiniJoystickYPower; }
boolean BlackMagicRemoteControlState::getLeftMiniJoystickButtonOn() { return leftMiniJoystickButtonOn == '1'; }
boolean BlackMagicRemoteControlState::getLeftButtonOn() { return leftButtonOn=='1'; }
boolean BlackMagicRemoteControlState::getLeftToggleSwitchOn() { return leftToggleSwitchOn == '1'; }
char BlackMagicRemoteControlState::getRightMiniJoystickXDirection() { return rightMiniJoystickXDirection; }
String BlackMagicRemoteControlState::getRightMiniJoystickXPower() { return rightMiniJoystickXPower; }
char BlackMagicRemoteControlState::getRightMiniJoystickYDirection() { return rightMiniJoystickYDirection; }
String BlackMagicRemoteControlState::getRightMiniJoystickYPower() { return rightMiniJoystickYPower; }
boolean BlackMagicRemoteControlState::getRightMiniJoystickButtonOn() { return rightMiniJoystickButtonOn == '1'; }
boolean BlackMagicRemoteControlState::getRightButtonOn() { return rightButtonOn == '1' ; }
boolean BlackMagicRemoteControlState::getRightToggleSwitchOn() { return rightToggleSwitchOn == '1'; }

bool BlackMagicRemoteControlState::validCommand(const String &cmd) {
  if (cmd.length() != 22) {
    return false;
  }

  // 1. Validar posiciones de letras
  const int letterPos[] = {0, 4, 11, 15}; // índice base 0
  for (int i = 0; i < 4; i++) {
    char c = cmd.charAt(letterPos[i]);
    if (c != 'C' && c != 'U' && c != 'D' && c != 'R' && c != 'L') {
      return false;
    }
  }

  // 2. Validar grupos numéricos (000–255)
  const int numGroups[][3] = {
    {1, 2, 3},   // grupo 1: posiciones 2,3,4
    {5, 6, 7},   // grupo 2: posiciones 6,7,8
    {12, 13, 14},// grupo 3: posiciones 13,14,15
    {16, 17, 18} // grupo 4: posiciones 17,18,19
  };
  for (int g = 0; g < 4; g++) {
    String numStr = "";
    for (int j = 0; j < 3; j++) {
      char c = cmd.charAt(numGroups[g][j]);
      if (!isDigit(c)) {
        return false;
      }
      numStr += c;
    }
    int val = numStr.toInt();
    if (val < 0 || val > 255) {
      return false;
    }
  }

  // 3. Validar bits (0 o 1)
  const int bitPos[] = {9, 10, 20, 21}; // índices base 0
  for (int i = 0; i < 4; i++) {
    char c = cmd.charAt(bitPos[i]);
    if (c != '0' && c != '1') {
      return false;
    }
  }

  return true; // Todo correcto
}
