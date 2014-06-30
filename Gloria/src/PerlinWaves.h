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
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    float frequency;
    //float lineWidth;
    bool crazySpeed;
    bool reverse;
    
    float xScatter;
    float yScatter;
    
    float xSpeed;
    float ySpeed;
    
    float _xSpeed;
    float _ySpeed;
    
};
