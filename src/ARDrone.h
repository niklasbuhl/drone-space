#ifndef FLIGHT_H
#define FLIGHT_H
//#include "Klient\RPC_drone.h"

class ARDrone{

public:

		int askYaw() {}
		int askRoll() {}
		int askPitch() {}
		int askHeight() {}
		int instruct(float x, float y, float z, float a) {}
		int land() {}
		int takeoff() {}

};

#endif /* end of include guard: FLIGHT_H */
