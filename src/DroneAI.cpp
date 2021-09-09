#include "ofMain.h"
#include "settings.h"
#include "DroneControl.h"
#include "DroneRoom.h"
#include "Drone.h"
#include "DroneAI.h"

bool sortByDist(const HoopDistance &lhs, const HoopDistance &rhs) { return lhs.intDist < rhs.intDist; }

DroneAI::DroneAI(Drone *_drone, DroneRoom *_droneroom, DroneControl *_dronecontrol) {

  printf("Initializing AI... ");

  drone = _drone;
  droneroom = _droneroom;
  dronecontrol = _dronecontrol;

  trueAngle = 0;

  trueAngleO.setGlobalPosition(0,0,0);
  trueAngleA.setParent(trueAngleO);
  trueAngleA.move(100,0,0);

  started = false;

  startAI();

  printf("Success!\n");

}

// General Functions

void DroneAI::startAI() {

  if(!started) {

    printf("Start.\n");

    started = true;

  }

}
void DroneAI::stopAI() {

  if(started) {

    printf("Stop.\n");

    started = false;

  }

}

void DroneAI::restart() {

  printf("Restart.\n");

  droneStart();

}

void DroneAI::update() {

  //printf("AI Loop.\n");

  if(!started) return;

  // Get Drone Information

    // Get True Angle

    getTrueAngleOffset();

    // Get Hoops

    getHoops();

    // QRs

    //getQRs();

  // Calculate Position

    // 1. x,y,z,a,A

    // 2. HoopBased Position

    // 3. QR Code Based Position

  // --- Anti Collision

  if(drone->getDestinationDistance() < WAYPOINT_DISTANCE && followTmpPath && drone->getDroneMode() != MANUAL) {

    printf("Return to current destination.\n");

    followTmpPath = false;

  }

  if(drone->getFlightMode() != STEADY && drone->getDroneMode() != TAKEOFF && drone->getDroneMode() != LANDING) {

    // Check Room

    ofVec3f tmpDestination;

    if(droneroom->room.hitbox.inside(drone->getPosition(),&tmpDestination)) {

      printf("Outside Room Safetybox!\n");

      drone->setDestination(tmpDestination);

      drone->setFlightMode(NORMAL);

      if(drone->getDroneMode() != MANUAL) droneInstruct();

      tmpDestinationDrawing = tmpDestination;

      followTmpPath = true;

      //drone->AIinstruct(tmpPath);

      if(drone->getDroneMode() != MANUAL) return;

    }

    //droneroom->getRoomHitbox().inside(drone->getPosition());

    // Check All Hoops

    /*

    for(int i = 0; i < HOOP_COUNT; i++) {

      if(droneroom->hoops[i].hitbox.outside(drone->getPosition(),&tmpDestination)) {

        //printf("Inside Hoop Hitbox!\n");

        drone->setDestination(drone->getPosition()-tmpDestination);

        drone->setFlightMode(NORMAL);

        if(drone->getDroneMode() != MANUAL) droneInstruct();

        //drone->AIinstruct(nextDestination);

        tmpDestinationDrawing = tmpDestination;

        followTmpPath = true;

        if(drone->getDroneMode() != MANUAL) return;

      }

    }

    */

  }

  // --- Manual

  if(drone->getDroneMode() == MANUAL) {

    // Hold or manual

    drone->update();

    dronecontrol->manual();

    return;

  }

  // --- Autonomous

  //tmpPath.next = currentPath;

  if(ofGetElapsedTimeMillis() > DEADLINE*1000) {

    // Time run out, land!

    if(!LANDED) droneLand();

  } else if(ofGetElapsedTimeMillis() > STARTLANDING*1000) {

    // Start Landing

    if(!(drone->getDroneMode() != LANDING)) {

      // Calculate Landing Trajectory

    }

    if(!(drone->getDroneMode() != LANDED)) {

      // Get to landing waypoint

      droneLanding();

    } else {

      // Land!

      droneLand();

    }

  } else if(drone->getDroneMode() == TAKEOFF) {

    // Takeoff

    //printf("Takeoff Loop.\n");

    droneTakeoff();

  } else if(drone->getDroneMode() == LANDED) {

    // Start the Drone

    //printf("START THE FUCKING DRONE.\n");

    droneStart();

  } else {

    // If position is unknown, look for position

    // Loop

    droneLoop();

  }

  // --- Drone Instructions ---

  // Real

  //instruction();

  // Virtual

  //printf("Maybe the drone is wrong?\n");

  droneInstruct();

  //printf("Ouch...\n");

  return;

}
void DroneAI::draw() {

  drawLoopTrajectory();
  drawTakeoffTrajectory();
  drawLandingTrajectory();
  drawTrueAngle();
  if(followTmpPath)drawTmpPath();

}

// Drone AI Modes

void DroneAI::droneStart() {

  startAllTrajectories();

  looping = false;
  landing = false;
  takeoff = false;

  // Initial Takeoff Trajectory

  landingwaypoint.point = droneroom->getLandingWaypoint();
  landingwaypoint.flightmode = NORMAL;

  addTakeoffPoint(drone->node.getGlobalPosition(), NORMAL, drone->getDroneMode());
  //addTakeoffPoint(droneroom->getLandingPlatform(), NORMAL, drone->getDroneMode());
  addTakeoffPoint(droneroom->getLandingWaypoint(), NORMAL, TAKEOFF);
  addTakeoffPoint(droneroom->hoops[0].getEntryPosition(), NORMAL, AUTONOMOUS);
  finishTakeoffTrajectory();

  optimizeTrajectory(takeoffCurrent, takeoffHead, &takeoffTrajectoryCount);

  // Initial Loop Trajectory

  for(int i = 0; i < HOOP_COUNT; i++) {
    addLoopPoint(droneroom->hoops[i].getEntryPosition(), NORMAL, AUTONOMOUS);
    addLoopPoint(droneroom->hoops[i].getExitPosition(), STEADY, AUTONOMOUS);
  }

  completeLoopTrajectory();

  optimizeTrajectory(loopCurrent, loopHead, &loopTrajectoryCount);

  // Reset Timer

  // Optimize Rotation and Position (Jesper)

  printf("Taking off!\n");

  dronecontrol->takeoff();

  takeoffCurrent = takeoffTail->next;

  takeoff = true;

  drone->setDroneMode(TAKEOFF);
  drone->setFlightMode(takeoffCurrent->flightmode);
  drone->setDestination(takeoffCurrent->point);

  printf("Destination - x: %f, y: %f, y: %f.\n",drone->getDestination().x,drone->getDestination().y,drone->getDestination().z);

  printFlightMode(drone->getFlightMode());
  printDroneMode(drone->getDroneMode());

  //printf("Ouch...\n");


}
void DroneAI::droneTakeoff() {

  if(drone->getDestinationDistance() < WAYPOINT_DISTANCE) {

    if(!takeoffCurrent->end) {

      printf("Locating Takeoff Waypoint.\n");

      takeoffCurrent = takeoffCurrent->next;
      currentPath = takeoffCurrent;

      drone->setDestination(takeoffCurrent->point);
      drone->setFlightMode(takeoffCurrent->flightmode);
      drone->setDroneMode(takeoffCurrent->dronemode);

      printFlightMode(drone->getFlightMode());
      printDroneMode(drone->getDroneMode());

      printf("New Takeoff Waypoint.\n");

      printf("End Destination - x: %f, y: %f, y: %f.\n",drone->getDestination().x,drone->getDestination().y,drone->getDestination().z);

    } else {

      printf("Took Off!\n");

      loopCurrent = loopHead;
      loopCurrent = loopCurrent->next;

      takeoff = false;
      looping = true;

      drone->setFlightMode(loopCurrent->flightmode);
      drone->setDestination(loopCurrent->point);
      drone->setDroneMode(loopCurrent->dronemode);

      printFlightMode(drone->getFlightMode());
      printDroneMode(drone->getDroneMode());

      printf("Startin Loop!\n");

      printf("Destination - x: %f, y: %f, y: %f.\n",drone->getDestination().x,drone->getDestination().y,drone->getDestination().z);

    }

  }

}
void DroneAI::droneLoop() {

  //printf("Looping.\n");

  if(drone->getDestinationDistance() < WAYPOINT_DISTANCE) {

    printf("New Loop Waypoint.\n");

    loopCurrent = loopCurrent->next;

    drone->setFlightMode(loopCurrent->flightmode);
    drone->setDestination(loopCurrent->point);
    drone->setDroneMode(loopCurrent->dronemode);

    printFlightMode(drone->getFlightMode());
    printDroneMode(drone->getDroneMode());

    printf("Destination - x: %f, y: %f, y: %f.\n",drone->getDestination().x,drone->getDestination().y,drone->getDestination().z);

  }

}
void DroneAI::droneLanding() {

  if(!calculatedLandingTrajectory) {

    printf("Starting to land!\n");

    // Initial Landing Trajectory

    addLandingPoint(drone->node.getPosition(),NORMAL,AUTONOMOUS);
    addLandingPoint(droneroom->getLandingWaypoint(),NORMAL,AUTONOMOUS);
    addLandingPoint(droneroom->getLandingPlatform(),NORMAL,LANDING);
    finishLandingTrajectory();

    landingCurrent = landingTail->next;
    loopCurrent = landingCurrent;

    calculatedLandingTrajectory = true;

    looping = false;
    landing = true;

    drone->setDroneMode(landingCurrent->dronemode);
    drone->setFlightMode(landingCurrent->flightmode);
    drone->setDestination(landingCurrent->point);

  }

  if(drone->getDestinationDistance() < WAYPOINT_DISTANCE) {

    if(!landingCurrent->end) {

      printf("Landing Waypoint.\n");

      printf("Destination - x: %f, y: %f, y: %f.\n",drone->getDestination().x,drone->getDestination().y,drone->getDestination().z);

      landingCurrent = landingCurrent->next;
      currentPath = landingCurrent;

      drone->setFlightMode(landingCurrent->flightmode);
      drone->setDestination(landingCurrent->point);

    } else if(drone->getDroneMode() != LANDED) {

      landing = false;

      printf("Landed!\n");

      printf("Destination - x: %f, y: %f, y: %f.\n",drone->getDestination().x,drone->getDestination().y,drone->getDestination().z);

      dronecontrol->land();

      drone->setDroneMode(LANDED);

    }
  }
}
void DroneAI::droneLand() {

  if(drone->getDroneMode() != LANDED) {
    printf("Landing!\n");
    drone->setDroneMode(LANDED);
  } else {
    printf("Landed!\n");
  }
}
void DroneAI::droneInstruct() {

  //printf("Updating Drone.\n");

  drone->update();

  //printf("Instructing Drone.\n");

  drone->instruction();

  //printf("AI control.\n");

  dronecontrol->instruct(drone->aiVector.x, drone->aiVector.y, drone->aiVector.z, drone->aiAngle);

  //printf("Ouch...\n");

}

// --- Trajectory Functions ---

// Optimize Trajectory
void DroneAI::optimizeTrajectory(Trajectory *currentPtr, Trajectory *headPtr, int *count) {

  printf("Starting to optimize trajection\n");

  Trajectory *ptr = currentPtr;

  ofVec3f A, B, P, AB, AP;

  float APdist, APmin, ABdist, pPdist, BAPangle;

  for(int i = 0; i < *count; i++) {

    printf("Loop Iteration: %i.\n", i);

    A = ptr->point;

    //printf("Point A - x:%f, y:%f, z:%f.\n", A.x,A.y,A.z);

    ptr = ptr->next;

    B = ptr->point;

    //printf("Point B - x:%f, y:%f, z:%f.\n", B.x,B.y,B.z);

    if(ptr->flightmode == STEADY) {
      printf("Flightmode is STEADY.\n");
      continue;
    }

    AB = B - A;

    ABdist = AB.length();

    printf("Flight distance AB: %f.\n", ABdist);

    int checkHoops = 0;

    HoopDistance hoopsToBeChecked[HOOP_COUNT];

    for(int j = 0; j < HOOP_COUNT; j++) {

      hoopsToBeChecked[j].n = j;
      hoopsToBeChecked[j].intDist = 2147483647;
      hoopsToBeChecked[j].distToOrigin = 0;
      hoopsToBeChecked[j].distToTrajectory = 0;

    }

    for(int j = 0; j < HOOP_COUNT;j++) {

      P = droneroom->hoops[j].getPosition();

      AP = P - A;

      APdist = AP.length();

      APmin = sqrt(pow(ABdist,2) + pow(HOOP_SAFETYDISTANCE,2));

      if(APmin < APdist) {
        printf("Hoop %i is is too far away.\n",j);
        //checkHoops--;
        continue;
      }

      BAPangle = AP.angle(AB);

      if(BAPangle > 90) {
        printf("Hoop %i angle is too large.\n",j);
        //checkHoops--;
        continue;
      }

      float pPdist = sin(ofDegToRad(BAPangle)) * APdist;

      if(pPdist > HOOP_SAFETYDISTANCE) {
        printf("Hoop %i is is outside the safety distance.\n",j);
        //checkHoops--;
        continue;
      }

      printf("Hoop %i distance: %f, angle: %f.\n", j, AP.length(), AP.angle(AB));

      // Push to array

      hoopsToBeChecked[j].distToOrigin = APdist;
      hoopsToBeChecked[j].distToTrajectory = pPdist;
      hoopsToBeChecked[j].intDist = (int)APdist;
      checkHoops++;

    }

    sort(&hoopsToBeChecked[0], &hoopsToBeChecked[HOOP_COUNT], sortByDist);

    printf("Check Hoops Count: %i\n", checkHoops);

    for(int j = 0; j < checkHoops; j++) {

      printf("Hoop %i with %i int dist, has distance %f to origin, and distance to trajectry %f.\n", hoopsToBeChecked[j].n, hoopsToBeChecked[j].intDist, hoopsToBeChecked[j].distToOrigin, hoopsToBeChecked[j].distToTrajectory);

    }

  }

}

// Draw Trajectories
void DroneAI::drawLoopTrajectory() {

  ofSetColor(ofColor::black);

  Trajectory *ptr = loopTail;

  ofVec3f A, B;

  for(int i = 0; i < loopTrajectoryCount; i++) {

    A = ptr->point;
    B = ptr->next->point;

    ofDrawArrow(A,B);

    ptr = ptr->next;

  }
}
void DroneAI::drawTakeoffTrajectory() {

  ofSetColor(ofColor::green);

  Trajectory *ptr = takeoffTail;

  ofVec3f A;
  ofVec3f B;

  for(int i = 0; i < takeoffTrajectoryCount-1; i++) {

    A = ptr->point;
    B = ptr->next->point;

    ofDrawArrow(A,B);

    ptr = ptr->next;

  }

  ofSetColor(ofColor::black);

}
void DroneAI::drawLandingTrajectory() {

  ofSetColor(ofColor::red);

  Trajectory *ptr = landingTail;

  ofVec3f A;
  ofVec3f B;

  for(int i = 0; i < landingTrajectoryCount-1; i++) {

    A = ptr->point;
    B = ptr->next->point;

    ofDrawArrow(A,B);

    ptr = ptr->next;

  }

  ofSetColor(ofColor::black);
}
void DroneAI::drawTmpPath() {

  ofSetColor(255,0,0);
  ofDrawArrow(drone->getPosition(),drone->getPosition()-tmpDestinationDrawing,5);

}

// Start Trajectories
void DroneAI::startAllTrajectories() {

  startLoopTrajectory();
  startTakeoffTrajectory();
  startLandingTrajectory();

}
void DroneAI::startLoopTrajectory() {

  printf("Starting Loop Trajectory\n");

  // Loop Trajectory

  for(int i = 0; i < LOOP_TRAJECTORY_MAX-1; i++) {
    loopTrajectory[i].next = &loopTrajectory[i+1];
    loopTrajectory[i+1].prev = &loopTrajectory[i];
  }

  loopTrajectory[LOOP_TRAJECTORY_MAX-1].next = &loopTrajectory[0];
  loopTrajectory[0].prev = &loopTrajectory[LOOP_TRAJECTORY_MAX-1];

  loopHead = &loopTrajectory[0];
  loopTail = &loopTrajectory[0];
  loopCurrent = &loopTrajectory[0];

  loopTrajectoryCount = 0;

}
void DroneAI::startTakeoffTrajectory() {

  // Takeoff Trajectory

  for(int i = 0; i < TAKEOFF_TRAJECTORY_MAX-1; i++) {
    takeoffTrajectory[i].next = &takeoffTrajectory[i+1];
    takeoffTrajectory[i+1].prev = &takeoffTrajectory[i];
  }

  takeoffTrajectory[TAKEOFF_TRAJECTORY_MAX-1].next = &takeoffTrajectory[0];
  takeoffTrajectory[0].prev = &takeoffTrajectory[TAKEOFF_TRAJECTORY_MAX-1];

  takeoffHead = &takeoffTrajectory[0];
  takeoffTail = &takeoffTrajectory[0];
  takeoffCurrent = &takeoffTrajectory[0];

  takeoffTrajectoryCount = 0;

}
void DroneAI::startLandingTrajectory() {

  // Landing Trajectory

  for(int i = 0; i < LANDING_TRAJECTORY_MAX-1; i++) {
    landingTrajectory[i].next = &landingTrajectory[i+1];
    landingTrajectory[i+1].prev = &landingTrajectory[i];
  }

  landingTrajectory[LANDING_TRAJECTORY_MAX-1].next = &landingTrajectory[0];
  landingTrajectory[0].prev = &landingTrajectory[LANDING_TRAJECTORY_MAX-1];

  landingHead = &landingTrajectory[0];
  landingTail = &landingTrajectory[0];
  landingCurrent = &landingTrajectory[0];

  landingTrajectoryCount = 0;

}

// Loop Trajectory
int DroneAI::addLoopPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode) {

  printf("Adding Loop Trajectory Point - x: %f, y: %f, y: %f.\n",point.x,point.y,point.z);

  loopHead->point = point;
  loopHead->flightmode = flightmode;
  loopHead->dronemode = dronemode;

  loopHead = loopHead->next;

  loopTrajectoryCount++;

  return true;

}
int DroneAI::insertLoopPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode) {

  loopHead->point = point;
  loopHead->flightmode = flightmode;
  loopHead->dronemode = dronemode;

  Trajectory *tmpPtrA = loopCurrent->next;
  Trajectory *tmpPtrB = loopHead->prev;
  Trajectory *tmpPtrC = loopHead->next;

  loopCurrent->next = loopHead;
  loopHead->next = tmpPtrA;
  tmpPtrB->next = tmpPtrC;

  loopHead->prev = loopCurrent;
  tmpPtrA->prev = loopHead;
  tmpPtrC->prev = tmpPtrB;

  loopHead = tmpPtrC;

  loopTrajectoryCount++;

  return true;

}
int DroneAI::completeLoopTrajectory() {

  loopHead = loopHead->prev;

  loopHead->next = loopTail;
  loopTail->prev = loopHead;

  return true;

}

// Takeoff Trajectory
int DroneAI::addTakeoffPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode) {

  //printf("Adding Loop Trajectory Point - x: %f, y: %f, y: %f.\n",point.x,point.y,point.z);

  takeoffHead->point = point;
  takeoffHead->flightmode = flightmode;
  takeoffHead->dronemode = dronemode;

  takeoffHead = takeoffHead->next;

  takeoffTrajectoryCount++;

  return true;

}
int DroneAI::insertTakeoffPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode) {

  takeoffHead->point = point;
  takeoffHead->flightmode = flightmode;
  takeoffHead->dronemode = dronemode;

  Trajectory *tmpPtrA = takeoffCurrent->next;
  Trajectory *tmpPtrB = takeoffHead->prev;
  Trajectory *tmpPtrC = takeoffHead->next;

  takeoffCurrent->next = takeoffHead;
  takeoffHead->next = tmpPtrA;
  tmpPtrB->next = tmpPtrC;

  takeoffHead->prev = takeoffCurrent;
  tmpPtrA->prev = takeoffHead;
  tmpPtrC->prev = tmpPtrB;

  takeoffHead = tmpPtrC;

  takeoffTrajectoryCount++;

  return true;

}
int DroneAI::finishTakeoffTrajectory() {
  takeoffHead->prev->end = true;
  return true;
}

// Landing Trajectory
int DroneAI::addLandingPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode) {

  //printf("Adding Loop Trajectory Point - x: %f, y: %f, y: %f.\n",point.x,point.y,point.z);

  landingHead->point = point;
  landingHead->flightmode = flightmode;
  landingHead->dronemode = dronemode;

  landingHead = landingHead->next;

  landingTrajectoryCount++;

  return true;

}
int DroneAI::insertLandingPoint(ofVec3f point, FlightMode flightmode, DroneMode dronemode) {

  landingHead->point = point;
  landingHead->flightmode = flightmode;
  landingHead->dronemode = dronemode;

  Trajectory *tmpPtrA = landingCurrent->next;
  Trajectory *tmpPtrB = landingHead->prev;
  Trajectory *tmpPtrC = landingHead->next;

  landingCurrent->next = landingHead;
  landingHead->next = tmpPtrA;
  tmpPtrB->next = tmpPtrC;

  landingHead->prev = landingCurrent;
  tmpPtrA->prev = landingHead;
  tmpPtrC->prev = tmpPtrB;

  landingHead = tmpPtrC;

  loopTrajectoryCount++;

  return true;

}
int DroneAI::finishLandingTrajectory() {
  landingHead->prev->end = true;
  return true;
}

// --- Anti Collision ---

//ofVec3f DroneAI::antiCollision() {}

// --- Real Drone Functions ---

void DroneAI::getHoops() {

  drone->resetHoops();

  int hoopsNow = dronecontrol->askHoops();

  //printf("How Many Hoops do you see? %i\n", hoopsNow);

  int binary = 1;
  int x;
  int y;
  int h;
  int n;

  for(int i = 0; i < HOOP_COUNT * 2; i++) {

    //printf("Binary: %i.\n", binary);
    //printf("hoopsNow: %i.\n", hoopsNow);

    if(binary & hoopsNow) {

      //printf("Seeing Hoop: %i\n", i);

      x = dronecontrol->askHoopX(binary);
      y = dronecontrol->askHoopY(binary);
      h = dronecontrol->askHoopH(binary);
      n = i + 1;

      if(n > 6) n = false;

      drone->addHoop(x,y,h,n);

    }

    binary = binary * 2;

  }

}
void DroneAI::getQRs() {

  drone->resetQRs();

  int QRsNow = dronecontrol->askQRs();

  int binary = 1;
  int x;
  int y;
  int h;
  int n;

  for(int i = 0; i < (HOOP_COUNT + QR_REAL_COUNT); i++) {

    if(binary & QRsNow) {

      x = dronecontrol->askQRX(binary);
      y = dronecontrol->askQRY(binary);
      h = dronecontrol->askQRsize(binary);
      n = i + 1;

      drone->addQR(x,y,h,n);

    }

  }

}
float DroneAI::getTrueAngleOffset() {

  float newTrueAngle = (float)dronecontrol->askYaw();

  trueAngleO.rotate(trueAngle-newTrueAngle,0,1,0);

  trueAngle = newTrueAngle;

  return trueAngle;

}

// --- Draw Drone Information ---

void DroneAI::drawTrueAngle() {

  ofSetColor(255,0,0);
  ofDrawArrow(trueAngleO.getGlobalPosition(), trueAngleA.getGlobalPosition(), 0);

}
