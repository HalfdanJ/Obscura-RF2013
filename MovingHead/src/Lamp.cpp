//
//  Lamp.cpp
//  emptyExample
//
//  Created by Jonas Jongejan on 29/06/13.
//
//

#include "Lamp.h"


void Lamp::pointAt(ofVec3f point){
    ofVec3f dir = point - position;
    cout<<"Point at "<<dir.x<<"  "<<dir.y<<"  "<<dir.z<<endl;
    tilt = RAD_TO_DEG *  asin(dir.y/dir.length());
    
    float _pan = RAD_TO_DEG * atan2(dir.z, dir.x);
    
    float dist = fabs((_pan) - pan);
   
    if(fabs((_pan + 360) - pan) < dist && (_pan + 360) < panMax){
        _pan += 360;
    }
    else if(fabs((_pan - 360) - pan) < dist && (_pan - 360) > panMin){
        _pan -= 360;
    }
    pan = _pan;

    dest = point;
}

void Lamp::setRotation(float _pan, float _tilt) {
  /*  pan = _pan;
    tilt = _tilt;
    
    ofVec3f v = ofVec3f(10,0,0);
    v.rotate(tilt, ofVec3f(0,0,1));
    v.rotate(-pan, ofVec3f(0,1,0));
    
    dest = position + v;*/
}


float Lamp::getTilt(){
    return tilt;
}


float Lamp::getPan(){
    return pan;
}


float Lamp::getDmxPan(){
    float _pan = getPan();
    return ofMap(_pan, panMin, panMax, dmxPanMin, dmxPanMax);
}

float Lamp::getDmxTilt(){
    float _tilt = getTilt();
    return ofMap(_tilt, tiltMin, tiltMax, dmxTiltMin, dmxTiltMax);
}