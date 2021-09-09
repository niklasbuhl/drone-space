#include "ofMain.h"
#include "settings.h"
#include "DroneRoom.h"

WayPoint::WayPoint() {

  wirebox.node.setParent(node);
  wirebox.set(WAYPOINT_X, WAYPOINT_Y, WAYPOINT_Z);

}

void WayPoint::draw() {

  //ofSetColor(color);
  wirebox.draw();
  node.draw();

}

QR::QR() {

  plane.rotate(90,1,0,0);
  plane.rotate(90,0,0,1);
  plane.set(QR_SIZE_Y, QR_SIZE_X);
  plane.setParent(node);

}

void QR::draw() {

  node.draw();
  plane.draw();

}

void QR::setPosition(float x, float y, float z) {

  node.setPosition(x,y,z);

}

Hoop::Hoop() {

  radius = HOOP_RADIUS_C;
  orientation = ofVec3f(0,0,1);

  // Hoop
  hoop.rotate(90,0,0,1);
  hoop.set(radius, HOOP_THICKNESS,false);
  hoop.setParent(node);

  // Stand
  //stand.setParent(node);

  // Hitbox
  hitbox.set(DRONE_SAFETYDISTANCE*2, (DRONE_SAFETYDISTANCE+radius)*2, (DRONE_SAFETYDISTANCE+radius)*2);
  hitbox.node.setParent(node);

  // Passage
  passage.set(HOOP_PASSAGE_X, HOOP_PASSAGE_Y, HOOP_PASSAGE_Z);
  passage.node.setParent(node);

  // Entry
  entrypoint.node.move(HOOP_WAYPOINT_OFFSET,0,0);
  entrypoint.node.setParent(node);

  // Exit
  exitpoint.node.move(-HOOP_WAYPOINT_OFFSET,0,0);
  exitpoint.node.setParent(node);

}

void Hoop::draw() {

  node.draw();
  hoop.draw();
  stand.draw();
  hitbox.draw();
  passage.draw();
  entrypoint.draw();
  exitpoint.draw();

}

void Hoop::setRadius(int _radius) {

  radius = _radius;
  hoop.set(radius, HOOP_THICKNESS,false);

}

void Hoop::setOrientation(ofVec3f _orientation) {

  float angle = orientation.angle(_orientation);

  orientation = _orientation;

  node.rotate(angle,0,1,0);

}

ofVec3f Hoop::getEntryPosition() {
  return entrypoint.node.getGlobalPosition();
}

ofVec3f Hoop::getExitPosition() {
  return exitpoint.node.getGlobalPosition();
}

ofVec3f Hoop::getPosition() {
  return node.getGlobalPosition();
}

void Hoop::setPosition(float x, float y, float z) {

  node.setPosition(x,y,z);
  stand.setPosition(x,(y-radius)/2,z);
  stand.set(HOOP_THICKNESS,y-radius);

}

LandingPlatform::LandingPlatform() {

  // Plane
  platform.rotate(90,1,0,0);
  platform.set(LANDINGPLATFORM_X, LANDINGPLATFORM_Z);
  platform.setParent(node);

  // Wirebox
  waypoint.node.move(0,LANDINGPLATFORM_POS_Y,0);
  waypoint.node.setParent(node);

}

void LandingPlatform::draw() {

  node.draw();
  platform.draw();
  waypoint.draw();

}

Room::Room() {

  wirebox.node.move(0,ROOM_SIZE_Y/2,0);
  hitbox.node.move(0,ROOM_SIZE_Y/2,0);
  wirebox.node.setParent(node);
  hitbox.node.setParent(node);
  wirebox.set(ROOM_SIZE_X,ROOM_SIZE_Y,ROOM_SIZE_Z);
  hitbox.set(
    ROOM_SIZE_X-2*DRONE_SAFETYDISTANCE,
    ROOM_SIZE_Y-2*DRONE_SAFETYDISTANCE,
    ROOM_SIZE_Z-2*DRONE_SAFETYDISTANCE
  );

}

void Room::draw() {

  node.draw();
  wirebox.draw();
  hitbox.draw();

}

DroneRoom::DroneRoom() {

  printf("Initializing DroneRoom... ");

  room.node.setParent(node);

  // Landing Platform

  landingplatform.node.setParent(node);
  landingplatform.node.move(LANDINGPLATFORM_POS_X,0,LANDINGPLATFORM_POS_Z);

  // Hoop

  if(!HOOP_RANDOM) {

    hoops[0].setPosition(HOOP_A_X,HOOP_A_Y,HOOP_A_Z);
    hoops[1].setPosition(HOOP_B_X,HOOP_B_Y,HOOP_B_Z);
    hoops[2].setPosition(HOOP_C_X,HOOP_C_Y,HOOP_C_Z);
    hoops[3].setPosition(HOOP_D_X,HOOP_D_Y,HOOP_D_Z);
    hoops[4].setPosition(HOOP_E_X,HOOP_E_Y,HOOP_E_Z);
    hoops[5].setPosition(HOOP_F_X,HOOP_F_Y,HOOP_F_Z);

    hoops[0].radius = HOOP_A_R;
    hoops[0].node.rotate(HOOP_A_A,0,1,0);
    hoops[1].radius = HOOP_B_R;
    hoops[1].node.rotate(HOOP_B_A,0,1,0);
    hoops[2].radius = HOOP_C_R;
    hoops[2].node.rotate(HOOP_C_A,0,1,0);
    hoops[3].radius = HOOP_D_R;
    hoops[3].node.rotate(HOOP_D_A,0,1,0);
    hoops[4].radius = HOOP_E_R;
    hoops[4].node.rotate(HOOP_E_A,0,1,0);
    hoops[4].radius = HOOP_F_R;
    hoops[4].node.rotate(HOOP_F_A,0,1,0);

    hoops[0].setRadius(hoops[0].radius);
    hoops[1].setRadius(hoops[1].radius);
    hoops[2].setRadius(hoops[2].radius);
    hoops[3].setRadius(hoops[3].radius);
    hoops[4].setRadius(hoops[4].radius);
    hoops[5].setRadius(hoops[5].radius);

  }

  for(int i = 0; i < HOOP_COUNT; i++) {

    printf("Adding hoop: %i. ", i);

    if(HOOP_RANDOM) {

      int aRand = ofRandom(0,359);

      int xRand = ofRandom(-449,449);
      int yRand = ofRandom(101,299);
      int zRand = ofRandom(-449,449);

      hoops[i].setPosition(xRand,yRand,zRand);

      hoops[i].node.rotate(aRand,0,1,0);

    }

    hoops[i].number = i;

    hoops[i].node.setParent(node);

  }

  // QR

  QRs[0].setPosition(QR_W00_X,QR_REAL_H,QR_W00_Z);
  QRs[1].setPosition(QR_W01_X,QR_REAL_H,QR_W01_Z);
  QRs[2].setPosition(QR_W02_X,QR_REAL_H,QR_W02_Z);
  QRs[3].setPosition(QR_W03_X,QR_REAL_H,QR_W03_Z);
  QRs[4].setPosition(QR_W04_X,QR_REAL_H,QR_W04_Z);
  QRs[5].setPosition(QR_W10_X,QR_REAL_H,QR_W10_Z);
  QRs[6].setPosition(QR_W11_X,QR_REAL_H,QR_W11_Z);
  QRs[7].setPosition(QR_W12_X,QR_REAL_H,QR_W12_Z);
  QRs[8].setPosition(QR_W13_X,QR_REAL_H,QR_W13_Z);
  QRs[9].setPosition(QR_W14_X,QR_REAL_H,QR_W14_Z);
  QRs[10].setPosition(QR_W20_X,QR_REAL_H,QR_W20_Z);
  QRs[11].setPosition(QR_W21_X,QR_REAL_H,QR_W21_Z);
  QRs[12].setPosition(QR_W22_X,QR_REAL_H,QR_W22_Z);
  QRs[13].setPosition(QR_W23_X,QR_REAL_H,QR_W23_Z);
  QRs[14].setPosition(QR_W24_X,QR_REAL_H,QR_W24_Z);
  QRs[15].setPosition(QR_W30_X,QR_REAL_H,QR_W30_Z);
  QRs[16].setPosition(QR_W31_X,QR_REAL_H,QR_W31_Z);
  QRs[17].setPosition(QR_W32_X,QR_REAL_H,QR_W32_Z);
  QRs[18].setPosition(QR_W33_X,QR_REAL_H,QR_W33_Z);
  QRs[19].setPosition(QR_W34_X,QR_REAL_H,QR_W34_Z);

  QRs[1].node.rotate(180,0,1,0);
  QRs[2].node.rotate(180,0,1,0);
  QRs[3].node.rotate(180,0,1,0);
  QRs[4].node.rotate(180,0,1,0);
  QRs[5].node.rotate(180,0,1,0);

  QRs[5].node.rotate(90,0,1,0);
  QRs[6].node.rotate(90,0,1,0);
  QRs[7].node.rotate(90,0,1,0);
  QRs[8].node.rotate(90,0,1,0);
  QRs[9].node.rotate(90,0,1,0);

  QRs[15].node.rotate(-90,0,1,0);
  QRs[16].node.rotate(-90,0,1,0);
  QRs[17].node.rotate(-90,0,1,0);
  QRs[18].node.rotate(-90,0,1,0);
  QRs[19].node.rotate(-90,0,1,0);

  QRs[0].node.setParent(node);
  QRs[1].node.setParent(node);
  QRs[2].node.setParent(node);
  QRs[3].node.setParent(node);
  QRs[4].node.setParent(node);
  QRs[5].node.setParent(node);
  QRs[6].node.setParent(node);
  QRs[7].node.setParent(node);
  QRs[8].node.setParent(node);
  QRs[9].node.setParent(node);
  QRs[10].node.setParent(node);
  QRs[11].node.setParent(node);
  QRs[12].node.setParent(node);
  QRs[13].node.setParent(node);
  QRs[14].node.setParent(node);
  QRs[15].node.setParent(node);
  QRs[16].node.setParent(node);
  QRs[17].node.setParent(node);
  QRs[18].node.setParent(node);
  QRs[19].node.setParent(node);

  printf("Done Adding Hoops. ");

  printf("Success!\n");

}

void DroneRoom::draw() {

  ofSetColor(ofColor::black);

  node.draw();
  room.draw();
  landingplatform.draw();

  for(int i = 0; i < HOOP_COUNT; i++) {

    hoops[i].draw();

  }

  for(int i = 0; i < QR_COUNT; i++) {

    QRs[i].draw();

  }
}

ofVec3f DroneRoom::getLandingWaypoint() {
  return landingplatform.waypoint.node.getGlobalPosition();
}

ofVec3f DroneRoom::getLandingPlatform() {
  return landingplatform.node.getGlobalPosition();
}
