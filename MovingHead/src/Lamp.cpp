//
//  Lamp.cpp
//  emptyExample
//
//  Created by Jonas Jongejan on 29/06/13.
//
//

#include "Lamp.h"


void Lamp::pointAt(ofVec3f point){
    dest = point;
}


float Lamp::getTilt(){
    ofVec3f dir = dest - position;

    float tiltAngle = asin(dest.y/position.length());
    
}


float Lamp::getPan(){
    ofVec3f dir = dest - position;
    
    float panAngle = atan2(dir.z, dir.x);
    
}


float Lamp::getDmxPan(){
    float pan = getPan();
    return ofMap(pan, 0, PI, dmxPanMin, dmxPanMax);
}

float Lamp::getDmxTilt(){
    float tilt = getTilt();
    return ofMap(tilt, 0, TWO_PI, dmxTiltMin, dmxTiltMax);
}