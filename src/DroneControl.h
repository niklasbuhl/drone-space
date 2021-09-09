#ifndef DRONECONTROL_H
#define DRONECONTROL_H

#include "ofMain.h"
#include "Drone.h"
#include "DroneRoom.h"
#include "ARDrone.h"
#include "CV.h"

class DroneControl {

public:

  DroneControl(Drone *drone, ARDrone *ardrone, CV *cv);

  // ---

  // --- RPC Functions Pointers  ---

  // CV - Hoops
  int askHoops() {
    return 1;}
    //return cv->askHoops();}
  int askHoopX(int n) {
    return 0;}
    //return cv->askHoopX(n);} // X position on camera
  int askHoopY(int n) {
    return 0;}
    //return cv->askHoopY(n);} // Y position on camera
  int askHoopH(int n) {
    return 100;}
    //return cv->askHoopH(n);} // Vertical Radius
  int askHoopW(int n) {
    return 100;}
    //return cv->askHoopW(n);} // Horizontal Radius

  // CV - QR
  int askQRs() {

    //return 1;
    return cv->askQRs();
  }
  int askQRX(int n) {
    //return 10;
    return cv->askQRX(n);
  }
  int askQRY(int n) {return cv->askQRY(n);}
  int askQRsize(int n) {return cv->askQRsize(n);}

  // Drone Sensors
  int askHeight() {return ardrone->askHeight();}
  int askYaw() {return ardrone->askYaw();}
  int askRoll() {return ardrone->askRoll();}
  int askPitch() {return ardrone->askPitch();}

  // Drone Instructions (True, ready to move, false can't move now)
  int instruct(float x, float y, float z, float a) {return ardrone->instruct(x,y,z,a);}
  int takeoff() {return ardrone->takeoff();}
  int land() {return ardrone->land();}

  // Manual Control

  void manual();

  void ai(float x, float y, float z, float a);
  void ai(ofVec3f vector, float a);

  int upToggle(bool keyPressed); // Space
  int downToggle(bool keyPressed); // Shift

  // XY Basic Control
  int forwardToggle(bool keyPressed); // w W
  int backwardToggle(bool keyPressed); // s S

  int strafeLeftToggle(bool keyPressed); // A a
  int strafeRightToggle(bool keyPressed); // D d

  int turnLeftToggle(bool keyPressed); // E e
  int turnRightToggle(bool keyPressed); // Q q

  // Set Mode

  void toggleManualMode(bool keyPressed);

private:

  Drone *drone;
  CV *cv;
  ARDrone *ardrone;

  int up; // Space
  int down; // Shift
  float upDownSpeed;

  int forward; // A a
  int backward; // S s
  float forwardBackwardSpeed;

  int turnLeft; // Q q
  int turnRight; // E e
  float turnLeftRightSpeed;

  int strafeLeft; // A a
  int strafeRight; // D d
  float strafeLeftRightSpeed;

  FlightMode savedflightmode;
  DroneMode saveddronemode;

};

#endif /* end of include guard: DRONECONTROL_H */
