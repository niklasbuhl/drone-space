#include "ofMain.h"
#include "settings.h"
#include "DroneControl.h"
#include "Drone.h"

// --- Constructor

DroneControl::DroneControl(Drone *_drone, ARDrone *_ardrone, CV *_cv) {

  printf("Initializing dronecontrol... ");

  drone = _drone;
  ardrone = _ardrone;
  cv = _cv;

  // Setup RPC... something something

  printf("Success!\n");

}

// --- RPC Functions ---

/* Made into functions pointers

// Computer Vision Func
int DroneControl::askHoops() {return 0;}
int DroneControl::askHoopX(int n) {return 0;} // X position on camera
int DroneControl::askHoopY(int n) {return 0;} // Y position on camera
int DroneControl::askHoopH(int n) {return 0;} // Vertical Radius
int DroneControl::askHoopW(int n) {return 0;} // Horizontal Radius

int DroneControl::askQRs() {return 0;}
int DroneControl::askQRX(int n) {return 0;}
int DroneControl::askQRY(int n) {return 0;}
int DroneControl::askQRsize(int n) {return 0;}

// Drone Sensors
int DroneControl::askHeight() {return 0;}
int DroneControl::askTrueAngle() {return 0;}



// Drone Instructions
int DroneControl::instruction(float x, float y, float z, float a) {return 0;}
int DroneControl::takeoff() {
  printf("Takeoff Instruction!\n");
  return 0;}
int DroneControl::land() {
  printf("Land Instruction!\n");
  return 0;
}

*/

// ---

// Manual Control

void DroneControl::manual() {

  //printf("Manuel Instruction.\n");

  drone->move(ofVec3f(forwardBackwardSpeed*DRONE_MS_TO_FPS, upDownSpeed*DRONE_MS_TO_FPS, strafeLeftRightSpeed*DRONE_MS_TO_FPS));
  drone->turn(turnLeftRightSpeed);

  instruct(forwardBackwardSpeed,upDownSpeed,strafeLeftRightSpeed,turnLeftRightSpeed * DRONE_ANGLE_FPS);

}

void DroneControl::ai(float x, float y, float z, float a) {

  ofVec3f vector = ofVec3f(x,y,z);

  vector = vector.getNormalized() * drone->getSpeed();

  instruct(vector.x, vector.y, vector.z, a);

}

void DroneControl::ai(ofVec3f vector, float a) {

  ai(vector.x, vector.y, vector.z, a);

}

int DroneControl::upToggle(bool keyPressed) {
  if(!up && !down && keyPressed) {
    //printf("Going up.\n");
    up = true;
    upDownSpeed = drone->getSpeed();
    return true;
  } else if (up && !keyPressed) {
    //printf("Stop going Up.\n");
    up = false;
    upDownSpeed = 0;
    return true;
  } else {
    //printf("Can't go up.\n");
    return false;
  }
} // Space
int DroneControl::downToggle(bool keyPressed) {
  if(!down && !up && keyPressed) {
    //printf("Going down.\n");
    upDownSpeed = -(drone->getSpeed());
    down = true;
    return true;
  } else if (down && !keyPressed) {
    //printf("Stop going down.\n");
    down = false;
    upDownSpeed = 0;
    return true;
  } else {
    //printf("Can't go down.\n");
    return false;
  }
} // Shift

int DroneControl::forwardToggle(bool keyPressed) {
  if(!forward && !backward && keyPressed) {
    //printf("Going forward.\n");
    forward = true;
    forwardBackwardSpeed = drone->getSpeed();
    return true;
  } else if(forward && !keyPressed) {
    //printf("Stop going forward.\n");
    forward = false;
    forwardBackwardSpeed = 0;
    return true;
  } else {
    //printf("Can't go forward.\n");
    return false;
  }
} // W w
int DroneControl::backwardToggle(bool keyPressed) {
  if(!backward && !forward && keyPressed) {
    //printf("Going backward.\n");
    backward = true;
    forwardBackwardSpeed = -(drone->getSpeed());
    return true;
  } else if(backward && !keyPressed) {
    //printf("Stop going backward.\n");
    backward = false;
    forwardBackwardSpeed = 0;
    return true;
  } else {
    //printf("Can't go backward.\n");
    return false;
  }
} // S s

int DroneControl::strafeLeftToggle(bool keyPressed) {
  if(!strafeLeft && !strafeRight && keyPressed) {
    //printf("Strafe Left.\n");
    strafeLeft = true;
    strafeLeftRightSpeed = drone->getSpeed();
    return true;
  } else if(strafeLeft && !keyPressed) {
    //printf("Stop strafing left.\n");
    strafeLeft = false;
    strafeLeftRightSpeed = 0;
    return true;
  } else {
    //printf("Can't strafe left.\n");
    return false;
  }
} // A a
int DroneControl::strafeRightToggle(bool keyPressed) {
  if(!strafeRight && !strafeLeft && keyPressed) {
    //printf("Strafe right.\n");
    strafeRight = true;
    strafeLeftRightSpeed = -(drone->getSpeed());
    return true;
  } else if(strafeRight && !keyPressed) {
    //printf("Stop strafing right.\n");
    strafeRight = false;
    strafeLeftRightSpeed = 0;
    return true;
  } else {
    //printf("Can't strafe Right.\n");
    return false;
  }
} // D d

int DroneControl::turnLeftToggle(bool keyPressed) {
  if(!turnLeft && !turnRight && keyPressed) {
    //printf("Turning left.\n");
    turnLeft = true;
    turnLeftRightSpeed = drone->getRotationSpeed();
    return true;
  } else if(turnLeft && !keyPressed) {
    //printf("Stop turning Left.\n");
    turnLeft = false;
    turnLeftRightSpeed = 0;
    return true;
  } else {
    //printf("Can't turn left.\n");
    return false;
  }
} // E e
int DroneControl::turnRightToggle(bool keyPressed) {
  if(!turnRight && !turnLeft && keyPressed) {
    //printf("Turning right.\n");
    turnRight = true;
    turnLeftRightSpeed = -(drone->getRotationSpeed());
    return true;
  } else if(turnRight && !keyPressed) {
    //printf("Stop turning right.\n");
    turnRight = false;
    turnLeftRightSpeed = 0.0;
    return true;
  } else {
    //printf("Can't turn right.\n");
    return false;
  }
} // Q q

void DroneControl::toggleManualMode(bool keyPressed) {

  if(drone->getDroneMode() == MANUAL && !keyPressed) {
    drone->setFlightMode(savedflightmode);
    drone->setDroneMode(saveddronemode);

    printf("Continue from manual.\n");
    return;
  }

  else if(drone->getDroneMode() != MANUAL && !keyPressed) {
    saveddronemode = drone->getDroneMode();
    savedflightmode = drone->getFlightMode();
    drone->setFlightMode(FAST);
    drone->setDroneMode(MANUAL);

    printf("Manual Mode.\n");
    return;

  }
}
