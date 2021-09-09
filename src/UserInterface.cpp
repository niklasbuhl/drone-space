#include "ofMain.h"
#include "settings.h"
#include "UserInterface.h"
#include "DroneControl.h"
#include "DroneAI.h"

UserInterface::UserInterface(DroneControl *_dronecontrol, DroneAI *_droneai) {

  printf("Initializing User Interface... ");

  dronecontrol = _dronecontrol;
  droneai = _droneai;

  printf("Success!\n");

}

int UserInterface::keyboard(int key, bool keyPressed) {

  switch (key) {
    case 'W':
    case 'w':
      dronecontrol->forwardToggle(keyPressed);
      break;
    case 'A':
    case 'a':
      dronecontrol->strafeLeftToggle(keyPressed);
      break;
    case 'S':
    case 's':
      dronecontrol->backwardToggle(keyPressed);
      break;
    case 'D':
    case 'd':
      dronecontrol->strafeRightToggle(keyPressed);
      break;
    case 'Q':
    case 'q':
      dronecontrol->turnLeftToggle(keyPressed);
      break;
    case 'E':
    case 'e':
      dronecontrol->turnRightToggle(keyPressed);
      break;
    case MAC_SPACE:
      // Or start the program.
      dronecontrol->upToggle(keyPressed);
    break;
    case MAC_SHIFT_A:
    case MAC_SHIFT_B:
      dronecontrol->downToggle(keyPressed);
      break;
    case 'M':
    case 'm':
      dronecontrol->toggleManualMode(keyPressed);
      break;
    case 'H':
    case 'h':
      dronecontrol->toggleManualMode(keyPressed);
      break;
    case 'L':
    case 'l':
      dronecontrol->land();
      break;
    case 'R':
    case 'r':
      droneai->restart();
      break;
    case 'X':
    case 'x':
      droneai->startAI();
      break;
    default:
      return false;
      break;
  }

  return true;

}
