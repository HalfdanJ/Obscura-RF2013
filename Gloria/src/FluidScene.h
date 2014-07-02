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
#include "ofxFluid.h"

class FluidScene : public ContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    float	lastTime;
	float   deltaTime;
    
    float	lastSyphonFrame = 0;
	float   deltaTimeSyphonFrame = 0;
    
    float flowWidth;
    float flowHeight;
    float drawWidth;
    float drawHeight;

    ofFbo inputFbo;
    
    ofFbo obstacles;
    
    ofxFluid fluid;
    
    ofVec2f oldM;
    bool    bPaint, bObstacle, bBounding, bClear, bUpdate;
    
    ofVec2f gravity;
    ofVec2f emitPos;
    
    bool clear;
    
    bool drawObstacles;
    
};
