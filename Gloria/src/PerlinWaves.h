//
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 02/07/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "mapping.h"


class PerlinWaves : public ContentScene {

public:

    void setup();
    void draw();
    void debugDraw();
    void update();

    Mapping * mapping;
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui(ofxUICanvas * gui, float width);
    
    float frequency;
    float lineWidth;
    
    
    // direction
    
};
