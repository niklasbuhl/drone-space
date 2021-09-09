#ifndef WIREBOX_H
#define WIREBOX_H

#include "ofMain.h"

class Wirebox {

  public:

    Wirebox();
    void draw();
    void drawNodes();
    void set(float X,float Y,float Z);

    float inside(ofVec3f position, ofVec3f *result); // Return a vector to get back in.
    float outside(ofVec3f position, ofVec3f *result); // Return a vector to get back out.

    ofNode node;

  private:

    float X;
    float Y;
    float Z;

    static const int corners = 8;
    ofNode cornerNodes[corners];

    ofNode positionNode;
    ofNode droneNode;

};

#endif /* end of include guard: WIREBOX_H */
