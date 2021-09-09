#ifndef DRONEROOM_H
#define DRONEROOM_H

#include "ofMain.h"
#include "settings.h"
#include "ofxNBxyz/ofxNBxyzWirebox.h"

class WayPoint {

public:

  WayPoint();

  void draw();

  ofNode node;

private:

  Wirebox wirebox;
  ofColor color = ofColor::black;

};

class QR {

public:

  QR();

  void draw();

  void setPosition(float x, float y,float z);

  ofNode node;

private:

  ofPlanePrimitive plane;

};

class Hoop {

public:

  Hoop();

  int number;
  int radius;
  ofVec3f orientation;

  void draw();

  void setPosition(float x,float y,float z);
  void setRadius(int radius);
  void setOrientation(ofVec3f orientation);

  ofVec3f getEntryPosition();
  ofVec3f getExitPosition();
  ofVec3f getPosition();

  ofNode node;

  ofCylinderPrimitive hoop;
  ofCylinderPrimitive stand;

  Wirebox hitbox;
  Wirebox passage;

  WayPoint entrypoint;
  WayPoint exitpoint;

private:

};

class LandingPlatform {

public:

  LandingPlatform();

  void draw();

  ofVec3f getWayPointPosition();

  ofNode node;

  ofPlanePrimitive platform;

  WayPoint waypoint;

private:

};

class Room {

public:

  Room();

  void draw();

  ofNode node;

  Wirebox wirebox;
  Wirebox hitbox;

private:

};

class DroneRoom {

public:

  DroneRoom();

  void draw();

  ofNode node;

  // Room

  Room room;

  // Landing Platform

  LandingPlatform landingplatform;
  ofVec3f getLandingWaypoint();
  ofVec3f getLandingPlatform();

  // Hoops

  Hoop hoops[HOOP_COUNT];
  int hoopCount = HOOP_COUNT;

  // QR

  QR QRs[QR_COUNT];
  int qrCount = QR_COUNT;

private:

};

#endif /* end of include guard: DRONEROOM_H */
