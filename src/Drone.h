 #ifndef DRONE_H
#define DRONE_H

#include "ofMain.h"
#include "settings.h"
#include "DroneRoom.h"

enum DroneMode {READY, HOLD, AUTONOMOUS, MANUAL, LANDED, TAKEOFF, LANDING};

enum FlightMode {NOTFLYING, STEADY, NORMAL, FAST, INSANE, LUDICROUS};

void printFlightMode(FlightMode flightmode);
void printDroneMode(DroneMode dronemode);

class Drone {

public:

  Drone(DroneRoom *droneroom);

  void draw();
  void update();
  void instruction();

  // Godlike

  ofVec3f setPosition(ofVec3f position); // Return difference.
  ofVec3f getPosition(); // Return current position.

  // Autonomous

  ofVec3f setDestination(ofVec3f _destination); // Set new destination.
  ofVec3f getDestination(); // Return diffence vector.

  ofVec3f setTmpDestination(ofVec3f _tmpDestination);
  ofVec3f getTmpDestination();

  void unsetTmpDest();

  ofVec3f getDirection();
  ofVec3f getDestinationOffset();

  float getDestinationDistance();
  float getAngleOffset();

  // Set DroneMode(Speed) and FlightMode

  void setFlightMode(FlightMode _flightmode);
  FlightMode getFlightMode();
  void setDroneMode(DroneMode _dronemode);
  DroneMode getDroneMode();

  void toggleManualMode();

  // Control

  void move(ofVec3f vector);
  void turn(float a);

  float getSpeed();
  float getRotationSpeed();

  ofNode node;

  ofVec3f aiVector;
  float aiAngle;

  // What the drone sees

  void addHoop(int x, int y, int h, int n);
  void resetHoops();

  void addQR(int x, int y, int h, int n);
  void resetQRs();

  ofVec2f trueXY(int x, int y);
  ofVec3f trueAngleVector(ofVec2f trueXY);
  float getDistance(int trueSize, int perseivedSize, int x, int y);
  ofVec3f objectVector(ofVec3f trueAngle, float distance);
  ofVec3f projectedCalculation(int x, int y, int trueSize, int perseivedSize);

private:

  DroneRoom *droneroom;

  // View

  void drawView();
  void drawViewedHoops();
  void drawProjectedHoops();
  void drawViewedQRs();
  void drawProjectedQRs();

  float viewOffsetX;
  float viewOffsetY;
  float viewOffsetZ;

  int seenHoopsCount;
  int seenQRsCount;

  ofCylinderPrimitive viewedHoops[HOOP_COUNT*2];
  //ofCylinderPrimitive projectedHoops[HOOP_COUNT];
  int viewedHoopsNumber[HOOP_COUNT*2];

  ofPlanePrimitive viewedQRs[QR_COUNT];
  int viewedQRsNumber[QR_COUNT];

  ofNode view[4];

  // Appearance

  ofBoxPrimitive box;

  // Movement

  ofNode forward;
  ofNode direction;

  ofVec3f destination;
  ofVec3f destinationOffset;

  ofVec3f tmpDestination;
  int tmpDest;

  ofVec3f angleRotationDirection;

  float speed;

  FlightMode flightmode;
  DroneMode dronemode;

  float angleOffset;

};

#endif /* end of include guard: DRONE_H */
