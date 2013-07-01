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
    dir.rotate(rotation.y, ofVec3f(0,1,0));
    dir.rotate(rotation.x, ofVec3f(1,0,0));
    dir.rotate(rotation.z, ofVec3f(0,0,1));
    //cout<<"Point at "<<dir.x<<"  "<<dir.y<<"  "<<dir.z<<endl;
    
    
    
    float _tiltA = RAD_TO_DEG *  asin(dir.y/dir.length());
    float _panA = RAD_TO_DEG * atan2(dir.z, dir.x);
    
    float _tiltB = -180 - _tiltA;
    float _panB = _panA + 180;
    
    float distA = fabs((_panA) - pan);
    float distB = fabs((_panB) - pan);
   /*
    if(distA > fabs(distA-360))
        distA = fabs(distA-360);

    if(distA > fabs(distA+360))
        distA = fabs(distA+360);
    
    if(distB > fabs(distB-360))
        distB = fabs(distB-360);
    
    if(distB > fabs(distB+360))
        distB = fabs(distB+360);
    */
    //cout<<distA<<"  "<<distB<<endl;
    
    float _tilt = _tiltB;
    float _pan = _panB;
    float dist = distB;
    
    
    /*if(distA < distB){
        cout<<"A"<<endl;
        _tilt = _tiltA;
        _pan = _panA;
        dist = distA;
    } else {
                cout<<"B"<<endl;
    }*/
   
    
    
   
    if(fabs((_pan + 360) - pan) < dist && (_pan + 360) < panMax){
        _pan += 360;
    }
    else if(fabs((_pan - 360) - pan) < dist && (_pan - 360) > panMin){
        _pan -= 360;
    }
    
    if(_pan > panMax){
        _pan -= 360;
    }
    if(_pan < panMin){
        _pan += 360;
    }
    
    
    float newDist = fabs(_pan - pan);
    cout<<newDist<<endl;
    
    if(newDist > 20){
        coldCountdown = 1.7;
    }
    
    
    pan = _pan;
    tilt = _tilt;
    
    dest = point;
}

void Lamp::setRotation(float _pan, float _tilt) {
    pan = _pan;
    tilt = _tilt;
    
    ofVec3f v = ofVec3f(10,0,0);
    v.rotate(tilt, ofVec3f(0,0,1));
    v.rotate(-pan, ofVec3f(0,1,0));
    
    dest = position + v;
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