//
//  Lamp.h
//  emptyExample
//
//  Created by Jonas Jongejan on 29/06/13.
//
//

#ifndef __emptyExample__Lamp__
#define __emptyExample__Lamp__

#include "ofMain.h"

class Lamp {
    
public:
    ofVec3f position;
    ofVec3f dest;
    
    int dmxPanMin;
    int dmxPanMax;
    
    int dmxTiltMin;
    int dmxTiltMax;
    
    
    void pointAt(ofVec3f dest);
    
    float getTilt();
    float getPan();
    
    float getDmxTilt();
    float getDmxPan();


};



#endif /* defined(__emptyExample__Lamp__) */
