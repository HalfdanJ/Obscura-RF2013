//
//  LampWalker.h
//  gloriamap
//
//  Created by Jonas Jongejan on 30/06/13.
//
//

#ifndef __gloriamap__LampWalker__
#define __gloriamap__LampWalker__

#include "ofMain.h"
#include "mapping.h"

class LampWalker {

    
public:
    void setup();
    void draw();
    void update();
    
    Corner * dst;
    Corner * src;
    Corner * lastSrc;
    
    float pct;
    
    
    Mapping * mapping;
    
};

#endif /* defined(__gloriamap__LampWalker__) */
