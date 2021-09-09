#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ofMain.h"
#include "settings.h"
#include "DroneControl.h"
#include "DroneAI.h"

class UserInterface {

public:

  UserInterface(DroneControl *dronecontrol, DroneAI *droneai);

  int keyboard(int i, bool keyPressed);

  void draw();

private:

  DroneControl *dronecontrol;
  DroneAI *droneai;

};

#endif /* end of include guard: USERINTERFACE_H */
