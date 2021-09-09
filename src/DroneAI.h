#ifndef DRONEAI_H
#define DRONEAI_H

#include "ofMain.h"
#include "Drone.h"
#include "DroneRoom.h"
#include "DroneControl.h"

struct Trajectory {
  ofVec3f point;
  Trajectory *next;
  Trajectory *prev;
  FlightMode flightmode;
  DroneMode dronemode;
  int end = false;
};

struct HoopDistance {

  float distToTrajectory;
  float distToOrigin;
  int intDist;
  int n;

};

class DroneAI {

public:

  // Constructor
  DroneAI(Drone *_drone, DroneRoom *_droneroom, DroneControl *_dronecontrol);

  // Update and Draw
  void update();
  void draw();

  void startAI();
  void stopAI();
  void restart();

private:

  Trajectory *currentPath;
  ofVec3f tmpDestinationDrawing;
  int followTmpPath = false;

  Drone *drone;
  DroneRoom *droneroom;
  DroneControl *dronecontrol;

  // AI control

  int started;

  // Drone Information

  void getHoops();
  void getQRs();

  float getTrueAngleOffset();
  void drawTrueAngle();

  ofNode trueAngleO;
  ofNode trueAngleA;
  float trueAngle;

  // Calculate Position

  ofVec3f calculatePosition(); // Master

  ofVec3f caluclatePositionXYZAoffset();
  ofVec3f calculatePositionQRVector();
  ofVec3f calculatePositionHoopVector();

  // Drone control

  void droneStart(); // From LANDED to TAKEOFF
  void droneTakeoff(); // From TAKEOFF to AUTONOMOUS
  void droneLoop(); // AUTONOMOUS
  void droneLanding(); // From AUTONOMOUS to LANDING
  void droneLand(); // From LANDING to LANDED
  void droneInstruct(); // Intruct the drone to turn // move;

  // --- Anti Collision ---

  ofVec3f antiCollision();

  // Calculate Path

  ofVec3f path();

  int looping;
  int takeoff;
  int landing;

  // Trajectory Functions
  int originalTrajectory();
  void optimizeTrajectory(Trajectory *currentPtr, Trajectory *headPtr, int *count);
  void startAllTrajectories();

  // Draw
  void drawTakeoffTrajectory();
  void drawLoopTrajectory();
  void drawLandingTrajectory();
  void drawTmpPath();

  // Loop
  void startLoopTrajectory();
  int addLoopPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode);
  int insertLoopPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode);
  int completeLoopTrajectory();

  // Takeoff
  void startTakeoffTrajectory();
  int addTakeoffPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode);
  int insertTakeoffPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode);
  int finishTakeoffTrajectory();

  // Landing
  void startLandingTrajectory();
  int addLandingPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode);
  int insertLandingPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode);
  int finishLandingTrajectory();

  // Loop Trajectory
  Trajectory loopTrajectory[LOOP_TRAJECTORY_MAX]; // The Round
  Trajectory *loopHead;
  Trajectory *loopTail;
  Trajectory *loopCurrent;
  int loopTrajectoryCount = 0;

  // Landing / Takeoff Trajectory
  Trajectory landingwaypoint;

  Trajectory takeoffTrajectory[TAKEOFF_TRAJECTORY_MAX]; // Takeoff into the Round
  Trajectory *takeoffHead;
  Trajectory *takeoffTail;
  Trajectory *takeoffCurrent;
  int takeoffTrajectoryCount = 0;

  Trajectory landingTrajectory[LANDING_TRAJECTORY_MAX]; // Landing from anywhere in on the Round
  Trajectory *landingHead;
  Trajectory *landingTail;
  Trajectory *landingCurrent;
  int landingTrajectoryCount = 0;

  ofVec3f landingplatform = ofVec3f(DRONE_START_X, DRONE_START_Y, DRONE_START_Z);
  int calculatedLandingTrajectory = false;

};

#endif /* end of include guard: DRONEAI_H */
